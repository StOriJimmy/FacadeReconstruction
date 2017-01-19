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


class PhotoWidget : public QWidget
{
  Q_OBJECT
public:
  PhotoWidget(QWidget *parent=0);
  QPushButton *color_swatch;
  void reset_zoom();
public slots:
  void prev_raster();
  void next_raster();
  void set_color();
  void update();
protected:
  DataStore *ds;
  Engine *engine;

  void create_actions();
  void create_toolbars();

  QToolBar* toolBar;
  GLPhoto* glarea;

  QAction *prevRasterAction;
  QAction *nextRasterAction;

//  QActionGroup *toolActionGroup;
//  QAction *sketchAction;
//  QAction *selectColorAction;
//  QActionselectColorActiontion;
//  QAction *mergeSelectedSketchesAction;
  QAction *deleteSketchesAction;

  QAction *reset_pz_action;
  QAction *zoom_original_action;
  QAction *zoom_out_action;
  QAction *zoom_in_action;

  QSlider *toolSizeSlider;

};


#endif /* PHOTO_WIDGET_HPP_ */
