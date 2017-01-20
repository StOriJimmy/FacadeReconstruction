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

/**
 * @brief The GLTrackball class.
 * The panel used for displaying the vcg trackball.
 */
class GLTrackball: public QGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT
public:
    /**
     * @brief Construct a GLTrackball
     * @param format The format
     * @param parent A pointer to the parent
     */
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
    DataStore *ds;                            ///< A pointer to the datastore object
    Engine *engine;                           ///< A pointer to the Engine object

    QPointF map_from_QT_to_vp(QPoint pos);    ///< \brief Map from a Qt point to a vcg viewport point
    void draw3D();                            ///< \brief Draw the trackball and point cloud on to the panel

    vcg::Trackball trackball;                 ///< The vcg trackball object
    QRect vp;                                 ///< The viewport (in device coordinates)

    QTimer timer;                             ///< Timer to auto refresh the gl window when app is active

protected slots:
    void refresh();                           ///< \brief Update the window when it is active

public:
    bool draw_pointcloud;                     ///< If true, draw the point cloud
    bool draw_edges;                          ///< If true, draw the edges
    bool draw_surfaces;                       ///< If true, draw the surfaces
    bool draw_trackball;                      ///< If true, draw the trackball
    bool draw_cameras;                        ///< If true, draw the cameras
    bool save_buffer;                         ///< If true, save the buffer

public slots:
    void set_draw_pointcloud(bool value);     ///< \brief Set the 'draw point cloud' boolean
    void set_draw_edges(bool value);          ///< \brief Set the 'draw edges' boolean
    void set_draw_surfaces(bool value);       ///< \brief Set the 'draw surfaces' boolean
    void set_draw_trackball(bool value);      ///< \brief Set the 'draw trackball' boolean
    void set_draw_cameras(bool value);        ///< \brief Set the 'draw cameras' boolean

private:
    float cameraDist;                         ///< The camera distance
    float nearPlane;                          ///< The near threshold plane
    float farPlane;                           ///< The far threshold plane
    float fov;                                ///< The field of view

    /**
     * @brief Initialise the view with teh camera parameters
     * @param shot The camera parameters
     */
    void initializeShot(vcg::Shotf &shot);

    /**
     * @brief Get the camera parameters from the trackball
     * @return
     */
    QPair<vcg::Shotf, float> shotFromTrackball();
};




/**
 * \brief Given a shot "refCamera" and a trackball "track", computes a new shot which is equivalent
 * to apply "refCamera" o "track" (via CPU).
 */
template <class T>
vcg::Shot<T> track2ShotCPU(vcg::Shot<T> &refCamera, vcg::Trackball *track)
{
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

/**
 * \brief Given a shot "from" and a trackball "track", updates "track" with "from" extrinsics.
 * A traslation involving cameraDistance is included. This is necessary to compensate a trasformation that OpenGL performs
 * at the end of the graphic pipeline.
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
