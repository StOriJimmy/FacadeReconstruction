#include <QtOpenGL>
#include "panzoomer.hpp"
#include "gl_photo.hpp"

PanZoomer::PanZoomer(GLPhoto *buddy)
{
  ZOOM_BASE = 1.2f;
  MIN_ZOOM_LEVEL = -40;
  MAX_ZOOM_LEVEL =  40;

  canvas = buddy;

  currently_panning = false;
  panning_enabled = false;

  zoom_factor = 1.0f;
  zoom_level = 0.0f;
  last_mouse_pos = QVector2D(0.0f, 0.0f);
  zoom_pos_world_space = QVector2D(0.0f, 0.0f);
  zoom_pos_mouse_space = QVector2D(0.0f, 0.0f);
}

PanZoomer::~PanZoomer()
{
}

void PanZoomer::reset(QRectF zoom_rect)
{
  zoom_factor = 1.0f;
  zoom_level = 0.0f;
  last_mouse_pos = QVector2D(0.0f, 0.0f);
  zoom_pos_world_space = QVector2D(0.0f, 0.0f);
  zoom_pos_mouse_space = QVector2D(0.0f, 0.0f);
  panning_enabled = false;
  currently_panning = false;

  // compute an appropriate zoom level
  float zoom_x = viewport_width() / zoom_rect.width();
  float zoom_y = viewport_height() / zoom_rect.height();

  // figure out what we need to set the zoomLevel and zoomFactor to
  zoom_factor = qMin(zoom_x, zoom_y);
  zoom_level = (float)(qLn(zoom_factor) / qLn(ZOOM_BASE));

  // Center image
  QVector2D center( zoom_rect.x() + 0.5f * zoom_rect.width(),
                    zoom_rect.y() + 0.5f * zoom_rect.height());

  // Pan to center of drawing
  last_mouse_pos = 0.5f * QVector2D(viewport_width(), viewport_height()); // zoom to middle of the screen

  center = center - ( (1.0f/zoom_factor) * last_mouse_pos );

  zoom_pos_world_space = center;
}

// Convert world (object) coordinates into display coordinates
QVector2D PanZoomer::world_2_screen_space(QVector2D world_coords)
{
  return zoom_pos_mouse_space + zoom_factor * (world_coords - zoom_pos_world_space);
}

// Convert display coordinates into world (object) coordinates
QVector2D PanZoomer::screen_2_world_space(QVector2D screen_coords)
{
  return zoom_pos_world_space + ((screen_coords - zoom_pos_mouse_space) / zoom_factor);
}

QVector2D PanZoomer::screen_2_world_space(QPoint screen_coords)
{
  return screen_2_world_space(QVector2D(screen_coords));
}

void PanZoomer::push_GL_transform()
{
  canvas->makeCurrent();

  // 0.) Remember the original transformations...
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // 3.) Finally, translate so that current zoom object position goes to current zoom mouse position
  glTranslatef(zoom_pos_mouse_space.x(), zoom_pos_mouse_space.y(), .0f);

  // 2.) Now, apply the zoom factor, effectively scaling things around the ZERO position
  glScalef(zoom_factor, zoom_factor, 1.0f);

  // 1.) We want to zoom around the current zoom object position, so center this position at ZERO
  glTranslatef(-zoom_pos_world_space.x(), -zoom_pos_world_space.y(), .0f);

}

void PanZoomer::pop_GL_transform()
{
  canvas->makeCurrent();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}


void PanZoomer::start_panning(QPoint p)
{
  last_mouse_pos = QVector2D(p.x(), p.y());
  enable_panning(true);
}

void PanZoomer::pan(QPoint p)
{
  if(!currently_panning)
    return;

  // do the panning
  QVector2D new_mouse_pos(p.x(), p.y());
  if (panning_enabled)
  {
    QVector2D delta = new_mouse_pos - last_mouse_pos;
    zoom_pos_mouse_space += delta;  // WL: current zoom mouse position encodes the pan
  }
  last_mouse_pos = new_mouse_pos;
}

void PanZoomer::end_panning()
{
  enable_panning(false);
}

void PanZoomer::zoom(float delta)
{
  float new_zoom_level = zoom_level + delta;
  if (new_zoom_level < MIN_ZOOM_LEVEL)
    new_zoom_level = MIN_ZOOM_LEVEL;
  else if(new_zoom_level > MAX_ZOOM_LEVEL)
    new_zoom_level = MAX_ZOOM_LEVEL;
  set_zoom_level(new_zoom_level);
}

bool PanZoomer::can_zoom_in()
{
  return zoom_level < MAX_ZOOM_LEVEL;
}

bool PanZoomer::can_zoom_out()
{
  return zoom_level > MIN_ZOOM_LEVEL;
}

qreal PanZoomer::get_zoom_factor()
{
  return zoom_factor;
}

void PanZoomer::zoom_2_original()
{
  set_zoom_level(0.0);
}

// Get the latest mouse position in screen coordinates
void PanZoomer::update_mouse_position()
{
  QPoint mouse = canvas->mapFromGlobal(QCursor::pos());
  if (!canvas->rect().contains(mouse)){
    // TODO: fix side effects of this if enabling pan/zoom when mouse isn't on control

    // zoom to middle of the screen by default (if mouse isn't on control)
    last_mouse_pos = 0.5f * QVector2D(viewport_width(), viewport_height());
  }else{
    // otherwise, zoom to where mouse is
    last_mouse_pos= QVector2D(canvas->invert_y(mouse));
  }
}

void PanZoomer::enable_panning(bool value)
{

  if (panning_enabled != value)
  { // only change stuff if we have anything to change
    panning_enabled = value;
    if (panning_enabled)
    {
      currently_panning = true;
      // TODO: fix side effects of this if enabling pan/zoom when mouse isn't on control
      //update_mouse_position();
    }
    else
      currently_panning = false;
  }
}

// Get/Set the current Zoom Level. 0 is "no zoom". Positive values magnify.
void PanZoomer::set_zoom_level(float value)
{
  if (zoom_level != value)
  {
    update_mouse_position();
    zoom_level = value;

    zoom_pos_world_space = screen_2_world_space((last_mouse_pos));
    zoom_pos_mouse_space = last_mouse_pos;

    zoom_factor = (float)qPow(ZOOM_BASE, value);  // assign new value
  }
}

int PanZoomer::viewport_width()
{
  return canvas->vp.width();
}

int PanZoomer::viewport_height()
{
  return canvas->vp.height();
}


