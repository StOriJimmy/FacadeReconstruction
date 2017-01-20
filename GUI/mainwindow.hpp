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

/**
 * @brief The MainWindow class.
 * The main window for the GUI.
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
    /**
     * @brief Construct the main window
     * @param parent
     */
    MainWindow(QMainWindow *parent = 0);
    ~MainWindow();

    QAction *openAction;   ///< The open file action

    void raise_splash();   ///< \brief Display the 'loading' splash
    void close_splash();   ///< \brief Close the 'loading' splash

public slots:
    void open();             ///< \brief Open a file from disk
    void save();             ///< \brief Save to disk

protected:
    void closeEvent(QCloseEvent *event);

protected slots:
    void onCylinderAction();          ///< \brief Called when the cylinder tool is requested
    void onQuadricAction();           ///< \brief Called when the quadric tool is requested
    void onGTMIAction();              ///< \brief Called when the the 'parallel GTM' tool is requested
    void onGTMOAction();              ///< \brief Called when the 'orthogonal GTM' tool is requested
    void onAdvancedSurfaceAction();   ///< \brief Called when the advanced surface creation tool is requested
    void onAdvancedEdgeAction();      ///< \brief Called when the advanced edge tool is requested
    void onNextAction();              ///< \brief Called when the 'next' button is pushed in wizard mode
    void onCancelAction();            ///< \brief Called when 'cancel' is pushed in wizard mode
    void onGotoAdvancedAction();      ///< \brief Called when the 'goto advanced mode' is initialised
    void onGotoSimpleAction();        ///< \brief Called when the 'goto simple (wizard) mode' is initialised

private:
    void create_actions();            ///< \brief Create all of the actions
    void create_menus();              ///< \brief Create all of the menus
    void create_toolbars();           ///< \brief Create all of the toolbars
    void next_cyl_action();           ///< \brief Called when 'next' is called in the cylinder tool in wizard mode
    void next_quad_action();          ///< \brief Called when the 'next' button is called in the quadric tool in wizard mode
    void next_gtmi_action();          ///< \brief Called when the 'next' button is called in the parallel GTM tool in wizard mode
    void next_gtmo_action();          ///< \brief Called when the 'next' button is called in the orthogonal GTM tool in wizard mode

private:
  DataStore *ds;                        ///< A pointer to the data store object
  Engine *engine;                       ///< A pointer to the Engine object

  PhotoWidget *photo_widget;            ///< A pointer to the photo widget
  TrackballWidget *trackball_widget;    ///< A pointer to the trackball widget
  QMdiArea *mdi_area;                   ///< A pointer to the MDI area

  QMdiSubWindow *sub_3d;                ///< A pointer to the 3D window
  QMdiSubWindow *sub_2d;                ///< A pointer to the 2D window

  QMenu *fileMenu;                      ///< A pointer to the file menu
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

  /**
   * @brief An enumeration specifying which step of the wizard is being operated in.
   */
  enum WizStep
  {
    NO_STEP,
    STEP_1,
    STEP_2
  };

  bool wizard_mode;         ///< True if using wizard mode
  SurfaceKind wizard_kind;  ///< The kind of surface
  WizStep wizard_step;      ///< The step in the wizard currently used

//  QPixmap *splash_pix;
  QSplashScreen *splash;    ///< A pointer to the 'loading' splash window

};

#endif /* MAINWINDOW_HPP_ */
