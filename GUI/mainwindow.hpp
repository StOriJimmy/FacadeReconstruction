#ifndef MAINWINDOW_HPP_
#define MAINWINDOW_HPP_

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

#include <QMainWindow>
#include <datastore.hpp>

QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolBar;
class QMenu;
class QActionGroup;
class QMdiArea;
//class ;
QT_END_NAMESPACE

class PhotoWidget;
class TrackballWidget;
class Engine;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QMainWindow *parent = 0);
  ~MainWindow();

  QAction *openAction;

  void raise_splash();
  void close_splash();

public slots:
  void open();
  void save();

protected:
  void closeEvent(QCloseEvent *event);

  //  void open(QString filename);
protected slots:
  void onCylinderAction();
  void onQuadricAction();
  void onGTMIAction();
  void onGTMOAction();
  void onAdvancedSurfaceAction();
  void onAdvancedEdgeAction();
  void onNextAction();
  void onCancelAction();
  void onGotoAdvancedAction();
  void onGotoSimpleAction();
private:
  void create_actions();
  void create_menus();
  void create_toolbars();
  void next_cyl_action();
  void next_quad_action();
  void next_gtmi_action();
  void next_gtmo_action();

private:
  DataStore *ds;
  Engine *engine;

  PhotoWidget *photo_widget;
  TrackballWidget *trackball_widget;
  QMdiArea *mdi_area;

  QMdiSubWindow *sub_3d;
  QMdiSubWindow *sub_2d;

  QMenu *fileMenu;
  QMenu *editMenu;

  QToolBar *fileToolBar;
  QToolBar *editToolBar;
  QToolBar *advancedEditToolBar;
  QToolBar *wizardToolBar;

  QLabel *wizardLabel;
  QAction *nextAction;
  QAction *cancelAction;

  QAction *exitAction;
  QAction *saveAction;


  QAction *cylinderAction;
  QAction *quadricAction;
  QAction *gtmiAction;
  QAction *gtmoAction;

  QAction *advancedEdgeAction;
  QAction *advancedSurfaceAction;

  QAction *gotoAdvancedAction;
  QAction *gotoSimpleAction;

  QLabel *current_status;
  QFont regular_font;
  QFont bold_font;
  QFont italic_font;

  enum WizStep{
    NO_STEP,
    STEP_1,
    STEP_2
  };

  bool wizard_mode;
  SurfaceKind wizard_kind;
  WizStep wizard_step;

//  QPixmap *splash_pix;
  QSplashScreen *splash;

};

#endif /* MAINWINDOW_HPP_ */
