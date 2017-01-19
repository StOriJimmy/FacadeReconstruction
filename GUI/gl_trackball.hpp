#ifndef GL_TRACKBALL_HPP_
#define GL_TRACKBALL_HPP_

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

class GLTrackball: public QGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT
public:
  GLTrackball(const QGLFormat &format, QWidget *parent = 0);
  ~GLTrackball();

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

  QPointF map_from_QT_to_vp(QPoint pos);
  void draw3D();

  vcg::Trackball trackball;
  QRect vp; // the viewport (in device coordinates)


  QTimer timer; // timer to auto refresh the gl window when app is active
protected slots:
  void refresh();

public:
  bool draw_pointcloud;
  bool draw_edges;
  bool draw_surfaces;
  bool draw_trackball;
  bool draw_cameras;
  bool save_buffer;

public slots:
  void set_draw_pointcloud(bool value);
  void set_draw_edges(bool value);
  void set_draw_surfaces(bool value);
  void set_draw_trackball(bool value);
  void set_draw_cameras(bool value);

private:
  float cameraDist;
  float nearPlane;
  float farPlane;
  float fov;


  void initializeShot(vcg::Shotf &shot);
  QPair<vcg::Shotf, float> shotFromTrackball();
};




/*
    Given a shot "refCamera" and a trackball "track", computes a new shot which is equivalent
    to apply "refCamera" o "track" (via CPU).
    */
    template <class T>
    vcg::Shot<T> track2ShotCPU(vcg::Shot<T> &refCamera, vcg::Trackball *track){
        vcg::Shot<T> view;

        double _near, _far;
        _near = 0.1;
        _far = 100;

        //get shot extrinsics matrix
    vcg::Matrix44<T> shotExtr;
        refCamera.GetWorldToExtrinsicsMatrix().ToMatrix(shotExtr);

    vcg::Matrix44<T> model2;
        model2 = (shotExtr)* vcg::Matrix44<T>::Construct(track->Matrix());
    vcg::Matrix44<T> model;
        model2.ToMatrix(model);

        //get translation out of modelview
    vcg::Point3<T> tra;
        tra[0] = model[0][3]; tra[1] = model[1][3]; tra[2] = model[2][3];
        model[0][3] = model[1][3] = model[2][3] = 0;

        //get pure rotation out of modelview
        double det = model.Determinant();
        double idet = 1/pow(det, 1/3.0); //inverse of the determinant
        model *= idet;
        model[3][3] = 1;
        view.Extrinsics.SetRot(model);

        //get pure translation out of modelview
    vcg::Matrix44<T> imodel = model;
        vcg::Transpose(imodel);
        tra = -(imodel*tra);
        tra *= idet;
        view.Extrinsics.SetTra(vcg::Point3<T>::Construct(tra));

        //use same current intrinsics
        view.Intrinsics = refCamera.Intrinsics;

        return view;
    }

    /*
    Given a shot "from" and a trackball "track", updates "track" with "from" extrinsics.
    A traslation involving cameraDistance is included. This is necessary to compensate a trasformation that OpenGL performs
    at the end of the graphic pipeline.
    */
    template <class T>
        void shot2Track(const vcg::Shot<T> &from, const float cameraDist, vcg::Trackball &tb)
    {
      vcg::Quaternion<T> qfrom; qfrom.FromMatrix(from.Extrinsics.Rot());
      tb.track.rot = vcg::Quaternionf::Construct(qfrom);
      tb.track.tra =  (vcg::Point3f::Construct(-from.Extrinsics.Tra()));
      tb.track.tra += vcg::Point3f::Construct(tb.track.rot.Inverse().Rotate(vcg::Point3f(0,0,cameraDist)))*(1/tb.track.sca);
    }

#endif /* GL_TRACKBALL_HPP_ */
