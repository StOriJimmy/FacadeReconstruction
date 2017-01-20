#ifndef PHOTO_WIDGET_HPP_
#define PHOTO_WIDGET_HPP_

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
#include "gl_photo.hpp"

/**
 * @brief The PhotoWidget class.
 * The widget used to display the photographs.
 */
class PhotoWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct a PhotoWidget
     * @param parent A pointer to the parent
     */
    PhotoWidget(QWidget *parent=0);

    QPushButton *color_swatch;  ///< The colour swatch
    void reset_zoom();          ///< \brief Reset the zoom level

public slots:
    void prev_raster();         ///< \brief Select the previous raster to view
    void next_raster();         ///< \brief Select the next raster to view
    void set_color();           ///< \brief Set the colour
    void update();              ///< \brief Update the object

protected:
    DataStore *ds;                  ///< A pointer to the datastore object
    Engine *engine;                 ///< A pointer to the Engine object

    void create_actions();          ///< \brief Create the actions
    void create_toolbars();         ///< \brief Create the toolbars

    QToolBar* toolBar;
    GLPhoto* glarea;

    QAction *prevRasterAction;
    QAction *nextRasterAction;
    QAction *deleteSketchesAction;

    QAction *reset_pz_action;
    QAction *zoom_original_action;
    QAction *zoom_out_action;
    QAction *zoom_in_action;

    QSlider *toolSizeSlider;

};


#endif /* PHOTO_WIDGET_HPP_ */
