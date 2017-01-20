#ifndef TRACKBALL_WIDGET_HPP_
#define TRACKBALL_WIDGET_HPP_

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

#include <QtGui>
#include "common.hpp"
#include "datastore.hpp"
#include "engine.hpp"
#include "gl_trackball.hpp"

/**
 * @brief The TrackballWidget class.
 * The trackball widget displays the vcg trackball, the point cloud and any computed surfaces.
 */
class TrackballWidget : public QWidget
{
  Q_OBJECT
public:
    /**
     * @brief Construct a new TrackBall widget.
     * @param parent A pointer to the parent widget
     */
    TrackballWidget(QWidget *parent=0);

public slots:
    void add_raster();              ///< Add a raster to the trackball  (not used)
    void update();                  ///< Update the object
    void delete_all_surfaces();     ///< Delete all of the surfaces from the trackball
    void delete_last_surface();     ///< Delete the last surface from the trackball

protected:
    DataStore *ds;                  ///< A pointer to the datastor object
    Engine *engine;                 ///< A pointer to the Engine objects

    void create_actions();          ///< \brief Create all of the actions
    void create_toolbars();         ///< \brief Create all of the toolbars

    QToolBar* toolBar;
    GLTrackball* glarea;
    QAction *addRasterAction;

    QAction *draw_pointcloud;
    QAction *draw_edges;
    QAction *draw_surfaces;
    QAction *draw_trackball;
    QAction *draw_cameras;

    QAction *deleteSurfacesAction;
    QAction *deleteLastSurfaceAction;
};


#endif /* TRACKBALL_WIDGET_HPP_ */
