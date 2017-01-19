#ifndef GLVP_PHOTO_HPP_
#define GLVP_PHOTO_HPP_

/* ----------------------------------------------------------------------
 * Copyright (C) 2016 Daniel Beale and Luca Benedetti. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * ---------------------------------------------------------------------- */

#include "common.hpp"
#include <QtGui>
#include "engine.hpp"
#include "datastore.hpp"
#include "panzoomer.hpp"

class GLArea;

class GLPhoto : public QGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT
public:
  GLPhoto(const QGLFormat &format,  QWidget *parent = 0);
  ~GLPhoto();
  QRect vp; // the viewport (in device coordinates)

  QPoint invert_y(QPoint pos);
public slots:
  void rasterChanged();
  void reset_pan_zoom();
  void zoom_in();
  void zoom_out();
  void zoom_2_original();
signals:
  void enable_zoom(bool value);
  void enable_zoom_in(bool value);
  void enable_zoom_out(bool value);

protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

  void resizeEvent(QResizeEvent *event);
  void keyPressEvent(QKeyEvent *e);
  void keyReleaseEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *e);

protected:
  DataStore *ds;
  Engine *engine;

  QPoint cursor_2_image_coords(QPoint pos);
  void draw2D();


  QTimer timer; // timer to auto refresh the gl window when app is active

  GLuint splat_tex_id;
  float splat_tex_size;
  PanZoomer *pz;

protected slots:
  void refresh();
};

#endif /* GLVP_PHOTO_HPP_ */
