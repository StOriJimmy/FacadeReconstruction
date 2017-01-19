#ifndef GLUTILS_HPP_
#define GLUTILS_HPP_

#include <QtOpenGL>
#include "common.hpp" //note: hack

void log_gl_params();

GLuint create_brush_texture(QGLWidget *context, QImage img);
GLuint create_texture(QGLWidget *context, QImage img, bool whiten=false);

void push_all();
void pop_all();

void init_fbo(int w, int h, QGLFramebufferObject *fbo, QImage *im=NULL);
void bind_fbo(int w, int h, QGLFramebufferObject *fbo, bool blending=true);
void unbind_fbo(QGLFramebufferObject *fbo);

void draw_fbo_quad(QGLFramebufferObject *fbo, float alpha = 1.0);
void draw_tex_quad(QImage img, qreal alpha = 1.0, QVector2D position = QVector2D(0.0, 0.0),  qreal scale = 1.0, qreal angle = 0.0, QPointF hotspot = QPointF(0.0, 0.0));
void draw_tex_quad(GLuint texture, QSize tex_size, qreal r = 1.0, qreal g = 1.0, qreal b = 1.0, qreal alpha = 1.0, QVector2D position = QVector2D(0.0, 0.0),  qreal scale = 1.0, qreal angle = 0.0, QPointF hotspot = QPointF(0.0, 0.0));

void draw_camera(vcg::Shotf *shot, float drawscale = 0.05, const vcg::Color4b &camcolor = vcg::Color4b(100, 200, 255, 255));
void draw_segment(const vcg::Segment3f &segment, const vcg::Color4b &color = vcg::Color4b(255, 255, 0, 255), float width = 2.0f);
void draw_segment_list(const QList<vcg::Segment3f> &segments, const vcg::Color4b &color = vcg::Color4b(255, 255, 0, 255), float width = 2.0f);
void draw_point(const vcg::Point3f &point, const vcg::Color4b &color = vcg::Color4b(0, 255, 255, 255), float size = 4.0f);
void draw_point_list(const QList<vcg::Point3f> &points, const vcg::Color4b &color = vcg::Color4b(0, 255, 255, 255), float size = 4.0f);

void draw_curve(const QList<vcg::Point3f> &pts, const vcg::Color4b &color = vcg::Color4b(128, 128, 128, 255), float width = 1.5f);

void draw_framelet(int w, int h, const vcg::Plane3f &photo_plane, const vcg::Shotf &camera);


#endif // GLUTILS_HPP_
