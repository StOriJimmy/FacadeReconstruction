#include "gl_photo.hpp"

#include "user_input.hpp"
#include "spline.hpp"
#include "glutils.hpp"
#include "vcgutils.hpp"

GLPhoto::GLPhoto(const QGLFormat &format, QWidget *parent)
  : QGLWidget(format, parent)
  ,ds(DataStore::data())
  ,engine(Engine::engine())
  ,splat_tex_id(0)
  ,splat_tex_size(500)
{
  pz = new PanZoomer(this);


  connect(&timer, SIGNAL(timeout()), this, SLOT(refresh()));
  timer.setSingleShot(false);
  timer.setInterval(100);
  timer.start();
}

GLPhoto::~GLPhoto()
{
  delete pz;
}


void GLPhoto::initializeGL()
{
  initializeOpenGLFunctions();
  QOpenGLFunctions funcs(QOpenGLContext::currentContext());

  if(! funcs.hasOpenGLFeature(QOpenGLFunctions::NPOTTextures)) {
    qWarning("Cannot enable NPOT textures!");
    exit(EXIT_FAILURE);
  }

  LOG_INFO(QString("Using OpenGL v%1.%2")
           .arg(format().majorVersion())
           .arg(format().minorVersion()));
  log_gl_params();
  LOG_INFO() << "devicePixelRatio" << devicePixelRatio();

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);



}

void GLPhoto::resizeGL(int width, int height)
{
  const int w = QTLogicalToDevice(this,width);
  const int h = QTLogicalToDevice(this,height);
  vp = QRect(0, 0, w, h);
  reset_pan_zoom();

  updateGL();

}

void GLPhoto::paintGL()
{
  // init viewport
  glEnable(GL_SCISSOR_TEST);
  glViewport(vp.x(), vp.y(), vp.width(), vp.height());
  glScissor(vp.x(), vp.y(), vp.width(), vp.height());

  if(ds->isLoaded())
    glClearColor(0.2, 0.0, 0.0, 1.0);
  else
    glClearColor(0.0, 0.0, 0.0, 1.0);

  glClear(GL_COLOR_BUFFER_BIT);

  // init splat texture
  if(splat_tex_id==0){
    QImage splat_im(":/resources/probability_distribution.png");
    splat_tex_size = splat_im.width();
    splat_tex_id = create_texture(this,splat_im);
  }

  //
  if(!ds->isLoaded())
    return;

  foreach(rasterID id,ds->rasters.keys()){
    if(ds->rasters[id]->tex_id == 0){
      ds->rasters[id]->tex_id = create_texture(this, ds->rasters[id]->image);
    }
  }

  // setup gl state
  //  glDisable(GL_BLEND);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_NORMALIZE);
  glDisable(GL_COLOR_MATERIAL);
  glDisable(GL_DEPTH_TEST);

  // setup projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, vp.width(), 0, vp.height(), -1.0f, 1.0f);

  // init modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  pz->push_GL_transform();
  {
    draw2D();
  }
  pz->pop_GL_transform();
}

void GLPhoto::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);
  resizeGL(width(), height());

}

void GLPhoto::keyPressEvent(QKeyEvent *event)
{
  event->ignore();
  updateGL();
}

void GLPhoto::keyReleaseEvent(QKeyEvent *event)
{
  event->ignore();
  updateGL();
}

void GLPhoto::mousePressEvent(QMouseEvent *event)
{
  if(!ds->isLoaded())
    return;

  int t = gTime->elapsed();

  event->accept();
  setFocus();

  // todo: panzoom stuff
  if( (event->modifiers() & Qt::ShiftModifier)||
      (event->buttons() & Qt::RightButton ) ) {
    pz->start_panning(invert_y(event->pos()));
    return;
  }

  if ( engine->sketching_permitted &&
       (event->button() == Qt::LeftButton )) {
    engine->sketching = true;
    engine->add_sketch_input(UserInput(t, cursor_2_image_coords(event->pos()), UserInput::BEGIN, ""));
  }

  updateGL();
}


void GLPhoto::mouseMoveEvent(QMouseEvent *event)
{
  if(!ds->isLoaded())
    return;

  int t = gTime->elapsed();

  event->accept();

  // todo: panzoom stuff
  if( (event->modifiers() & Qt::ShiftModifier)||
      (event->buttons() & Qt::RightButton ) ){
    pz->pan(invert_y(event->pos()));
    return;
  }

  if( engine->sketching &&
      (event->buttons() & Qt::LeftButton) ) {

    engine->add_sketch_input(UserInput(t, cursor_2_image_coords(event->pos()), UserInput::CONTINUE, ""));
  };
  updateGL();

}

void GLPhoto::mouseReleaseEvent(QMouseEvent *event)
{
  if(!ds->isLoaded())
    return;

  int t = gTime->elapsed();

  if( (event->modifiers() & Qt::ShiftModifier)||
      (event->buttons() & Qt::RightButton ) ){
    pz->end_panning();
    return;
  }

  if( engine->sketching  ) {

    engine->sketching=false;
    engine->add_sketch_input(UserInput(t, cursor_2_image_coords(event->pos()), UserInput::END, ""));
  }

  event->accept();


  updateGL();
}

void GLPhoto::wheelEvent(QWheelEvent *event)
{
  event->accept();
  setFocus();

  float numDegrees = event->delta() / 8.0f;
  float numSteps = numDegrees / 15.0f;

  if (event->orientation() == Qt::Vertical) {
    pz->zoom(numSteps);
    emit enable_zoom_in(pz->can_zoom_in());
    emit enable_zoom_out(pz->can_zoom_out());
  }

  updateGL();
}

void GLPhoto::mouseDoubleClickEvent(QMouseEvent *event)
{
  event->ignore();
  LOG_INFO()<<"double click";
  updateGL();
}



void GLPhoto::draw2D()
{
  Raster* curr_rast = ds->rasters[ds->current_raster];

  draw_tex_quad(curr_rast->tex_id, curr_rast->image.size());

  foreach(sketchID id, curr_rast->sketches){

    QColor c=ds->sketches.value(id)->color;

    foreach(SketchPoint p, ds->sketches.value(id)->points){

      qreal scale = p.sizePX()/splat_tex_size;

      draw_tex_quad(splat_tex_id, QSize(splat_tex_size,splat_tex_size), // tex info
                    c.redF(),c.greenF(),c.blueF(),1.0, // RGBA
                    QVector2D(p.x*curr_rast->w, (1.0-p.y)*curr_rast->h), // position in pixels
                    scale,
                    0.0, // angle
                    QPointF(splat_tex_size / 2.0f, splat_tex_size / 2.0f)); // hot spot
    }
  }

  if(engine->sketching_permitted){
    QPoint p = cursor_2_image_coords(mapFromGlobal(cursor().pos()));
    QColor c = engine->sketch_color;
    qreal scale = SketchPoint::variance2sizePX(engine->tool_size)/splat_tex_size;

    draw_tex_quad(splat_tex_id, QSize(splat_tex_size,splat_tex_size), // tex info
                  c.redF(),c.greenF(),c.blueF(),0.8, // RGBA
                  QVector2D(p.x(), (curr_rast->h-p.y())), // position in pixels
                  scale,
                  0.0, // angle
                  QPointF(splat_tex_size / 2.0f, splat_tex_size / 2.0f)); // hot spot
  }


  if(gTime->elapsed() - engine->tool_size_updated < 1000)  {
    QPoint p = cursor_2_image_coords(QPoint(width()/2.0,height()/2.0));
    QColor c = engine->sketch_color;
    qreal scale = SketchPoint::variance2sizePX(engine->tool_size)/splat_tex_size;

    draw_tex_quad(splat_tex_id, QSize(splat_tex_size,splat_tex_size), // tex info
                  c.redF(),c.greenF(),c.blueF(),0.6, // RGBA
                  QVector2D(p.x(), (curr_rast->h-p.y())), // position in pixels
                  scale,
                  0.0, // angle
                  QPointF(splat_tex_size / 2.0f, splat_tex_size / 2.0f)); // hot spot
  }

}

void GLPhoto::refresh()
{
  // redraw only when the window is active
  if(QApplication::activeWindow()) {
    updateGL();
  } else {
    Sleeper::msleep(30);
  }
}

QPoint GLPhoto::invert_y(QPoint pos)
{
  return QPoint(pos.x(),vp.height()-pos.y());
}

void GLPhoto::rasterChanged()
{
  reset_pan_zoom();
  emit enable_zoom(true);
  emit enable_zoom_in(pz->can_zoom_in());
  emit enable_zoom_out(pz->can_zoom_out());
}

void GLPhoto::reset_pan_zoom()
{
  if(!ds->isLoaded())
    return;

  Raster* curr_rast = ds->rasters[ds->current_raster];
  pz->reset( QRect(0, 0, curr_rast->w, curr_rast->h) );

  emit enable_zoom_in(pz->can_zoom_in());
  emit enable_zoom_out(pz->can_zoom_out());
}
void GLPhoto::zoom_in()
{
  pz->zoom(1.0);
  emit enable_zoom_in(pz->can_zoom_in());
  emit enable_zoom_out(pz->can_zoom_out());
}

void GLPhoto::zoom_out()
{
  pz->zoom(-1.0);
  emit enable_zoom_in(pz->can_zoom_in());
  emit enable_zoom_out(pz->can_zoom_out());
}

void GLPhoto::zoom_2_original()
{
  pz->zoom_2_original();
  emit enable_zoom_in(pz->can_zoom_in());
  emit enable_zoom_out(pz->can_zoom_out());
}

QPoint GLPhoto::cursor_2_image_coords(QPoint p)
{
  QVector2D v = pz->screen_2_world_space(invert_y(p*devicePixelRatio()));
  Raster* curr_rast = ds->rasters[ds->current_raster];
  return QPoint(v.x(),curr_rast->h-v.y());
}
