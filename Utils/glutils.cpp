#include <Logger.h>
#include "glutils.hpp"
#include "vcgutils.hpp"
#include "spline.hpp"

#ifdef _MSC_VER
#include <gl/GLU.h>
#else
#include <glu.h>
#endif

void log_gl_params()
{
  GLenum params[] = {
    GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
    GL_MAX_CUBE_MAP_TEXTURE_SIZE,
    GL_MAX_DRAW_BUFFERS,
    GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
    GL_MAX_TEXTURE_IMAGE_UNITS,
    GL_MAX_TEXTURE_SIZE,
    GL_MAX_VARYING_FLOATS,
    GL_MAX_VERTEX_ATTRIBS,
    GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
    GL_MAX_VERTEX_UNIFORM_COMPONENTS,
    GL_MAX_VIEWPORT_DIMS,
    GL_STEREO,
  };
  const char *names[] = {
    "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
    "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
    "GL_MAX_DRAW_BUFFERS",
    "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
    "GL_MAX_TEXTURE_IMAGE_UNITS",
    "GL_MAX_TEXTURE_SIZE",
    "GL_MAX_VARYING_FLOATS",
    "GL_MAX_VERTEX_ATTRIBS",
    "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
    "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
    "GL_MAX_VIEWPORT_DIMS",
    "GL_STEREO",
  };
  LOG_INFO("GL Context Params:");

  // integers - only works if the order is 0-10 integer return types
  for(int i = 0; i < 10; i++) {
    int v = 0;
    glGetIntegerv(params[i], &v);
    LOG_INFO("%s %i", names[i], v);
  }

  // others
  int v[2];
  v[0] = v[1] = 0;
  glGetIntegerv(params[10], v);
  LOG_INFO("%s %i %i", names[10], v[0], v[1]);
  unsigned char s = 0;
  glGetBooleanv(params[11], &s);
  LOG_INFO("%s %u", names[11], (unsigned int)s);
}


GLuint create_brush_texture(QGLWidget *context,QImage img)
{
  context->makeCurrent();
  QImage tex = QGLWidget::convertToGLFormat(
        img);  // converts to RGBA format... but we want only RGB
  GLuint texture;
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, tex.width(), tex.height(), GL_RGBA,
                    GL_UNSIGNED_BYTE, tex.bits());

  //	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
  //	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
  //	//glGenerateMipmaps(GL_TEXTURE_2D);

  return texture;
}


GLuint create_texture(QGLWidget *context, QImage img, bool whiten)
{
  context->makeCurrent();
  QImage tex = QGLWidget::convertToGLFormat(img);

  if(whiten) {
    // make sure image has white background
    for(int y = 0; y < tex.height(); ++y) {
      uchar *data = reinterpret_cast<uchar * >(tex.scanLine(y));

      for(int x = 0; x < tex.width(); ++x) {
        data[0] = 255;
        data[1] = 255;
        data[2] = 255;
        data += 4;
      } // inner for
    } // outer for
  }

  GLuint texture;
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tex.width(), tex.height(), GL_RGBA,
                    GL_UNSIGNED_BYTE, tex.bits());
  return texture;
}


void push_all()
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
}

void pop_all()
{
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();
}


void init_fbo(int w, int h, QGLFramebufferObject *fbo, QImage *im)
{
  bind_fbo(w, h, fbo);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  if(im != NULL) {
    draw_tex_quad(*im);
  }

  unbind_fbo(fbo);
}

void bind_fbo(int w, int h, QGLFramebufferObject *fbo, bool blending)
{
  push_all();
  fbo->bind();

  if(blending) {
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE,
                        GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
  } else {
    glDisable(GL_BLEND);
  }

  glDisable(GL_SCISSOR_TEST);
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void unbind_fbo(QGLFramebufferObject *fbo)
{
  fbo->release();
  pop_all();
}

void draw_fbo_quad(QGLFramebufferObject *fbo, float alpha)
{
  glPushMatrix();
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fbo->texture());
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glScalef(fbo->width(), fbo->height(), 0);

    glBegin(GL_QUADS);
    {
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex2f(1.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex2f(1.0f, 1.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex2f(0.0f, 1.0f);
    }
    glEnd();
  }
  glPopMatrix();
}

void draw_tex_quad(QImage img, qreal alpha, QVector2D position, qreal scale, qreal angle, QPointF hotspot)
{
  glPushMatrix();
  {
    glTranslatef(position.x(), position.y(), 0.0);
    glScalef(scale, scale, 1.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glTranslatef(-hotspot.x(), -hotspot.y(), 0.0);
    glScalef(img.width(), img.height(), 1.0);

    QImage tex = QGLWidget::convertToGLFormat(img);
    GLuint texture;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, tex.bits());
    glColor4f(1.0, 1.0, 1.0, alpha);
    glBegin(GL_QUADS);
    {
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex2f(1.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex2f(1.0f, 1.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex2f(0.0f, 1.0f);
    }
    glEnd();
    glDeleteTextures(1, &texture);
  }
  glPopMatrix();
}

void draw_tex_quad(GLuint texture, QSize tex_size, qreal r,qreal g, qreal b, qreal alpha, QVector2D position,  qreal scale, qreal angle, QPointF hotspot)
{
  glPushMatrix();
  {
    glTranslatef(position.x(), position.y(), 0.0);
    glScalef(scale, scale, 1.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glTranslatef(-hotspot.x(), -hotspot.y(), 0.0);
    glScalef(tex_size.width(), tex_size.height(), 1.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor4f(r, g, b, alpha);

    glBegin(GL_QUADS);
    {
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex2f(1.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex2f(1.0f, 1.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex2f(0.0f, 1.0f);
    }
    glEnd();
  }
  glPopMatrix();
}



void draw_camera(vcg::Shotf *shot, float drawscale, const vcg::Color4b &camcolor)
{
  //  vcg::Color4b camcolor = vcg::Color4b(100, 200, 255, 255);
  //  float drawscale = 0.05;

  vcg::Matrix44f currtr;
  currtr.SetIdentity();

  if(!shot->IsValid()) {
    // no drawing if camera not valid
    LOG_WARNING("Invalid camera");
    return;
  }

  if(shot->Intrinsics.cameraType != vcg::Camera<float>::PERSPECTIVE) {
    // no drawing if camera not perspective
    LOG_WARNING("Non-perspective camera");
    return;
  }

  vcg::Point3f vp = shot->GetViewPoint();
  vcg::Point3f ax0 = shot->Axis(0);
  vcg::Point3f ax1 = shot->Axis(1);
  vcg::Point3f ax2 = shot->Axis(2);

  glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
  {
    glDepthFunc(GL_ALWAYS);
    glDisable(GL_LIGHTING);


    // arbitrary size to draw axis
    float len = shot->Intrinsics.FocalMm * drawscale;

    glPushMatrix();
    glMultMatrix(Inverse(currtr));  //remove current mesh transform

    // grey axis, aligned with scene axis
    glColor3f(.7f, .7f, .7f);
    glBegin(GL_LINES);
    {
      glVertex3f(vp[0] - (len / 2.0), vp[1], vp[2]);
      glVertex3f(vp[0] + (len / 2.0), vp[1], vp[2]);
      glVertex3f(vp[0], vp[1] - (len / 2.0), vp[2]);
      glVertex3f(vp[0], vp[1] + (len / 2.0), vp[2]);
      glVertex3f(vp[0], vp[1], vp[2] - (len / 2.0));
      glVertex3f(vp[0], vp[1], vp[2] + (len / 2.0));
    }
    glEnd();

    // RGB axis, aligned with camera axis
    glBegin(GL_LINES);
    {
      glColor3f(1.0, 0, 0);
      glVertex(vp);
      glVertex(vp + ax0 * len);
      glColor3f(0, 1.0, 0);
      glVertex(vp);
      glVertex(vp + ax1 * len);
      glColor3f(0, 0, 1.0);
      glVertex(vp);
      glVertex(vp + ax2 * len);
    }
    glEnd();

    // Now draw the frustum
    vcg::Point3f viewportCenter = vp - (ax2 * shot->Intrinsics.FocalMm * drawscale);
    vcg::Point3f viewportHorizontal = ax0 * float(shot->Intrinsics.ViewportPx[0] *
        shot->Intrinsics.PixelSizeMm[0] / 2.0f * drawscale);
    vcg::Point3f viewportVertical   = ax1 * float(shot->Intrinsics.ViewportPx[1] *
        shot->Intrinsics.PixelSizeMm[1] / 2.0f * drawscale);


    glBegin(GL_LINES);
    {
      glColor(camcolor);
      glVertex3f(vp[0], vp[1], vp[2]);
      glVertex(viewportCenter);
      glColor(camcolor);
      glVertex(vp);
      glVertex(viewportCenter + viewportHorizontal + viewportVertical);
      glVertex(vp);
      glVertex(viewportCenter + viewportHorizontal - viewportVertical);
      glVertex(vp);
      glVertex(viewportCenter - viewportHorizontal + viewportVertical);
      glVertex(vp);
      glVertex(viewportCenter - viewportHorizontal - viewportVertical);
    }
    glEnd();
    glBegin(GL_LINE_LOOP);
    {
      glVertex(viewportCenter + viewportHorizontal + viewportVertical);
      glVertex(viewportCenter + viewportHorizontal - viewportVertical);
      glVertex(viewportCenter - viewportHorizontal - viewportVertical);
      glVertex(viewportCenter - viewportHorizontal + viewportVertical);
    }
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(.8f, .8f, .8f, .2f);
    glBegin(GL_TRIANGLE_FAN);
    {
      glVertex(vp);
      glVertex(viewportCenter + viewportHorizontal + viewportVertical);
      glVertex(viewportCenter + viewportHorizontal - viewportVertical);
      glVertex(viewportCenter - viewportHorizontal - viewportVertical);
      glVertex(viewportCenter - viewportHorizontal + viewportVertical);
      glVertex(viewportCenter + viewportHorizontal + viewportVertical);
    }
    glEnd();
    glDisable(GL_BLEND);


    // remove mesh transform
    glPopMatrix();


  }
  glPopAttrib();
}

void draw_segment(const vcg::Segment3f &segment, const vcg::Color4b &color, float width)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_LINE_SMOOTH);

  glLineWidth(width);
  glColor(color);
  glBegin(GL_LINES);
  {
    glVertex(segment.P0());
    glVertex(segment.P1());
  }
  glEnd();
  glPopAttrib();
}

void draw_segment_list(const QList<vcg::Segment3f> &segments, const vcg::Color4b &color, float width)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_LINE_SMOOTH);

  glLineWidth(width);
  glColor(color);
  glBegin(GL_LINES);
  {
    for(int i=0; i<segments.size();++i) {
      glVertex(segments.at(i).P0());
      glVertex(segments.at(i).P1());
    }
  }
  glEnd();
  glPopAttrib();
}


void draw_point(const vcg::Point3f &point, const vcg::Color4b &color, float size)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_POINT_SMOOTH);

  glPointSize(size);
  glColor(color);

  glBegin(GL_POINTS);
  {
    glVertex(point);
  }
  glEnd();

  glPopAttrib();
}

void draw_point_list(const QList<vcg::Point3f> &points, const vcg::Color4b &color, float size)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_POINT_SMOOTH);

  glPointSize(size);
  glColor(color);

  glBegin(GL_POINTS);
  {
    for(int i=0; i<points.size();++i) {
      glVertex(points.at(i));
    }
  }
  glEnd();

  glPopAttrib();
}

void draw_curve(const QList<vcg::Point3f> &pts, const vcg::Color4b &color, float width)
{
  if(pts.size() < 2) {
    return;
  }

  QList< sp::Vec3 > raw_nodes;

  if(pts.size() == 2) {
    sp::Vec3 a((double)pts.at(0).X(), (double)pts.at(0).Y(), (double)pts.at(0).Z());
    sp::Vec3 b((double)pts.at(1).X(), (double)pts.at(1).Y(), (double)pts.at(1).Z());
    sp::Vec3 impostor = (a + b) / 2.0;
    raw_nodes.push_back(a);
    raw_nodes.push_back(impostor);
    raw_nodes.push_back(b);
  } else {
    foreach(vcg::Point3f p, pts)
      raw_nodes.push_back(sp::Vec3((double)p.X(), (double)p.Y(), (double)p.Z()));
  }

  // at least 3 nodes
  std::vector< qreal > times;
  std::vector< sp::Vec3 > points;
  qreal tot_len = 0.0;

  qreal min_dist = raw_nodes.at(0).distance_from(raw_nodes.at(1));
  for(int p = 0; p < raw_nodes.size(); ++p) {
    if(p > 0) {
      qreal dist = raw_nodes.at(p).distance_from(raw_nodes.at(p - 1));
      tot_len += dist;
      min_dist = qMin(min_dist,dist);
    }

    times.push_back(tot_len);
    points.push_back(raw_nodes.at(p));
  }

  QList< sp::Vec3 > splined_nodes;

  sp::Spline<qreal, sp::Vec3 > spline(times, points);
  qreal t = 0.0;
  qreal delta = qMax(0.00001,min_dist/2.0);

  while(t < tot_len) {
    sp::Vec3 n = spline[t];
    splined_nodes.append(n);
    t += delta;
  }

  splined_nodes.append(spline[tot_len]);
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_LINE_SMOOTH);

  glLineWidth(width);
  glColor(color);

  glBegin(GL_LINE_STRIP);
  {
    foreach(sp::Vec3 p, splined_nodes) {
      glVertex3f(p.x, p.y, p.z);
    }
  }
  glEnd();
  glPopAttrib();
}


void draw_framelet(int w, int h, const vcg::Plane3f &photo_plane, const vcg::Shotf &camera)
{
  vcg::Point3f p1,p2,p3,p4;
  hit_photo_plane(vcg::Point2f(0,0),photo_plane,camera,p1);
  hit_photo_plane(vcg::Point2f(w,0),photo_plane,camera,p2);
  hit_photo_plane(vcg::Point2f(w,h),photo_plane,camera,p3);
  hit_photo_plane(vcg::Point2f(0,h),photo_plane,camera,p4);

  glEnable(GL_BLEND);
  glColor4f(1.0,1.0,1.0,0.5);
  glBegin(GL_QUADS);
  {
    glTexCoord2f(0.0f, 0.0f);
    glVertex(p1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex(p2);
    glTexCoord2f(1.0f, 1.0f);
    glVertex(p3);
    glTexCoord2f(0.0f, 1.0f);
    glVertex(p4);
  }
  glEnd();

  glColor4f(1.0,1.0,1.0,1.0);

  glBegin(GL_LINE_LOOP);
  {
    glVertex(p1);
    glVertex(p2);
    glVertex(p3);
    glVertex(p4);
  }
  glEnd();

}
