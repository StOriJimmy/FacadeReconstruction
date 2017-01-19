#include <QtWidgets>

#include "common.hpp"
#include "mainwindow.hpp"
#include "engine.hpp"
#include "photo_widget.hpp"
#include "trackball_widget.hpp"
#include "sketch_selection_dialog.hpp"
#include "edge_selection_dialog.hpp"

#include "utils.hpp"

MainWindow::MainWindow(QMainWindow *parent)
  : QMainWindow(parent)
  , ds(DataStore::data())
  , engine(Engine::engine())

{
  engine->setParent(this);

  regular_font=QFont("Stencil", 20);
  bold_font=QFont("Stencil", 20, QFont::Bold);
  italic_font=QFont("Stencil", 20, QFont::Normal, true);

  QPixmap splash_pix(":/resources/plswt.png");
  //  QPainter painter(&splash_pix);
  //  painter.drawText(20,20,APP_DISPLAY_NAME);
  splash = new QSplashScreen(splash_pix);

  photo_widget = new PhotoWidget(this);
  trackball_widget = new TrackballWidget(this);

  mdi_area=new QMdiArea(this);
  setCentralWidget(mdi_area);
  sub_3d = mdi_area->addSubWindow(trackball_widget);
  sub_3d->setWindowTitle("3D view");
  sub_2d = mdi_area->addSubWindow(photo_widget);
  sub_2d->setWindowTitle("2D view");
  mdi_area->tileSubWindows();

  current_status = new QLabel("Ready.");
  statusBar()->addPermanentWidget(current_status, 1);

  wizard_mode = false;
  wizard_kind = NONE;
  wizard_step = NO_STEP;
  create_actions();
  create_menus();
  create_toolbars();
  setFont(regular_font);

  setWindowTitle(APP_DISPLAY_NAME);
  resize(1400, 890);
}

MainWindow::~MainWindow()
{
  delete splash;
  //  delete splash_pix;
}

void MainWindow::raise_splash()
{
  splash->show();
  if (  QApplication::desktop()->screenCount() > 1 ) {
    QRect rect =  QApplication::desktop()->screenGeometry(1);
    QRect r2 = splash->rect();
    splash->move(rect.center()-QPoint(r2.width()/2,r2.height()/2));
  }
  splash->raise();
  QApplication::instance()->processEvents();
  Sleeper::usleep(50000);
  splash->raise();
  QApplication::instance()->processEvents();
}

void MainWindow::close_splash()
{
  splash->hide();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  event->accept();
}

void MainWindow::create_actions()
{
  openAction = new QAction(QIcon(":/icons/fileopen.ico"), tr("Open"), this);
  openAction->setShortcuts(QKeySequence::Open);
  connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
  //  openAction->setEnabled(false);

  exitAction = new QAction(QIcon(":/icons/exit.ico"), tr("Quit"), this);
  exitAction->setShortcuts(QKeySequence::Quit);
  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

  saveAction = new QAction(QIcon(":/icons/filesaveas.ico"), tr("Save Surfaces"), this);
  saveAction->setShortcuts(QKeySequence::Save);
  connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

  cylinderAction = new QAction("MAKE CYLINDER",this);
  connect(cylinderAction, SIGNAL(triggered()), this, SLOT(onCylinderAction()));

  quadricAction = new QAction("MAKE QUADRIC",this);
  connect(quadricAction, SIGNAL(triggered()), this, SLOT(onQuadricAction()));

  gtmiAction = new QAction("MAKE INTERPOLATED GTM",this);
  connect(gtmiAction, SIGNAL(triggered()), this, SLOT(onGTMIAction()));

  gtmoAction = new QAction("MAKE ORTHOGONAL GTM",this);
  connect(gtmoAction, SIGNAL(triggered()), this, SLOT(onGTMOAction()));

  advancedEdgeAction = new QAction("MAKE EDGES",this);
  connect(advancedEdgeAction, SIGNAL(triggered()), this, SLOT(onAdvancedEdgeAction()));

  advancedSurfaceAction = new QAction("MAKE SURFACE",this);
  connect(advancedSurfaceAction, SIGNAL(triggered()), this, SLOT(onAdvancedSurfaceAction()));

  gotoAdvancedAction = new QAction("Advanced mode.",this);
  connect(gotoAdvancedAction, SIGNAL(triggered()), this, SLOT(onGotoAdvancedAction()));

  gotoSimpleAction = new QAction("Simple mode.",this);
  connect(gotoSimpleAction, SIGNAL(triggered()), this, SLOT(onGotoSimpleAction()));

  wizardLabel = new QLabel("label");
  wizardLabel->setFont(italic_font);

  nextAction = new QAction("NEXT",this);
  nextAction->setFont(bold_font);
  connect(nextAction, SIGNAL(triggered()), this, SLOT(onNextAction()));
  cancelAction = new QAction("Cancel",this);
  connect(cancelAction, SIGNAL(triggered()), this, SLOT(onCancelAction()));


}

void MainWindow::create_menus()
{
  fileMenu = new QMenu(tr("&File"), this);
  fileMenu->addAction(openAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);

  menuBar()->addMenu(fileMenu);

  editMenu = new QMenu(tr("&Edit"), this);
  editMenu->addSection("Basic:");
  editMenu->addAction(cylinderAction);
  editMenu->addAction(quadricAction);
  editMenu->addAction(gtmiAction);
  editMenu->addAction(gtmoAction);
  editMenu->addSection("Advanced:");
  editMenu->addAction(advancedEdgeAction);
  editMenu->addAction(advancedSurfaceAction);

  menuBar()->addMenu(editMenu);

}

void MainWindow::create_toolbars()
{
  fileToolBar = addToolBar(tr("File"));
  fileToolBar->addAction(exitAction);
  fileToolBar->addSeparator();
  fileToolBar->addAction(openAction);
  fileToolBar->addSeparator();
  fileToolBar->addAction(saveAction);

  editToolBar = addToolBar(tr("Edit"));
  editToolBar->addAction(cylinderAction);
  editToolBar->addAction(quadricAction);
  editToolBar->addAction(gtmiAction);
  editToolBar->addAction(gtmoAction);
  editToolBar->addSeparator();
  QWidget* empty1 = new QWidget();
  empty1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  editToolBar->addWidget(empty1);
  editToolBar->addSeparator();
  editToolBar->addAction(gotoAdvancedAction);


  advancedEditToolBar = addToolBar(tr("Advanced Edit"));
  advancedEditToolBar->addAction(advancedEdgeAction);
  advancedEditToolBar->addAction(advancedSurfaceAction);
  QWidget* empty2 = new QWidget();
  empty2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  advancedEditToolBar->addWidget(empty2);
  advancedEditToolBar->addSeparator();
  advancedEditToolBar->addAction(gotoSimpleAction);

  advancedEditToolBar->hide();

  //  addToolBarBreak();

  wizardToolBar = addToolBar(tr("Wizard"));
  wizardToolBar->addAction(nextAction);
  wizardToolBar->addSeparator();
  QWidget* empty3 = new QWidget();
  empty3->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  wizardToolBar->addWidget(empty3);
  wizardToolBar->addWidget(wizardLabel);
  QWidget* empty4 = new QWidget();
  empty4->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  wizardToolBar->addWidget(empty4);
  wizardToolBar->addSeparator();
  wizardToolBar->addAction(cancelAction);
  wizardToolBar->addSeparator();

  wizardToolBar->hide();

}

void MainWindow::next_cyl_action()
{
  Q_ASSERT(wizard_mode);
  Q_ASSERT(wizard_kind == CYLINDER);
  Q_ASSERT(wizard_step == STEP_1);

  // check if we have at least 2 sketches in different rasters
  QSet< rasterID > r;
  foreach(Sketch *s,ds->sketches.values()){
    r.insert(s->raster);
  }
  if(r.size()<2){
    QMessageBox msgBox;
    msgBox.setText("Please sketch the Cylinder from at least 2 view");
    msgBox.exec();
    return;
  }
  raise_splash();
  // generate the edge map
  if(!engine->make_edge(ds->sketches.values(),ds->sketches.values().at(0)->color.name())){
    close_splash();
    QMessageBox msgBox;
    msgBox.setText("Cannot create a cylinder with the sketches provided, please try again.");
    msgBox.exec();
    onCancelAction();
    return;
  }

  // generate the column
  CylinderInputParams params;
  params.SurfaceType = CYLINDER;
  if(!engine->make_surface(ds->edges.values(),QColor(Qt::lightGray).name(),&params)){
    close_splash();
    QMessageBox msgBox;
    msgBox.setText("Cannot create a cylinder with the sketches provided, please try again.");
    msgBox.exec();
    onCancelAction();
    return;
  }
  close_splash();

  //go back to normal view:
  wizard_mode = false;
  wizard_kind = NONE;
  wizard_step = NO_STEP;
  wizardLabel->setText("");
  editToolBar->show();
  advancedEditToolBar->hide();

  engine->delete_all_edges();
  engine->delete_all_sketches();
  engine->sketch_color = QColor(Qt::red);
  sub_2d->show();
  sub_3d->show();
  mdi_area->tileSubWindows();
  wizardToolBar->hide();

}

void MainWindow::next_quad_action()
{
  Q_ASSERT(wizard_mode);
  Q_ASSERT(wizard_kind == QUADRIC);
  Q_ASSERT(wizard_step == STEP_1);


  // check if we have at least 2 sketches in different rasters
  QSet< rasterID > r;
  foreach(Sketch *s,ds->sketches.values()){
    r.insert(s->raster);
  }
  if(r.size()<2){
    QMessageBox msgBox;
    msgBox.setText("Please sketch the Quadric from at least 2 view");
    msgBox.exec();
    return;
  }

  raise_splash();
  // generate the edge map
  if(!engine->make_edge(ds->sketches.values(),ds->sketches.values().at(0)->color.name())){
    close_splash();
    QMessageBox msgBox;
    msgBox.setText("Cannot create a quadric with the sketches provided, please try again.");
    msgBox.exec();
    onCancelAction();
    return;
  }

  // generate the quadric
  QuadricInputParams params;
  params.SurfaceType = QUADRIC;
  if(!engine->make_surface(ds->edges.values(),QColor(Qt::lightGray).name(),&params)){
    close_splash();
    QMessageBox msgBox;
    msgBox.setText("Cannot create a quadric with the sketches provided, please try again.");
    msgBox.exec();
    onCancelAction();
    return;
  }
  close_splash();

  //go back to normal view:
  wizard_mode = false;
  wizard_kind = NONE;
  wizard_step = NO_STEP;
  wizardLabel->setText("");
  editToolBar->show();
  advancedEditToolBar->hide();

  engine->delete_all_edges();
  engine->delete_all_sketches();
  engine->sketch_color = QColor(Qt::red);
  sub_2d->show();
  sub_3d->show();
  mdi_area->tileSubWindows();
  wizardToolBar->hide();

}

void MainWindow::next_gtmi_action()
{
  Q_ASSERT(wizard_mode);
  Q_ASSERT(wizard_kind == GTM_INTERPOLATED);
  Q_ASSERT(wizard_step != NO_STEP);

  if(wizard_step == STEP_1) {

    // check if we have at least 2 sketches of the same color in different rasters
    QSet< rasterID > r;
    QSet< QString > cn;
    foreach(Sketch *s,ds->sketches.values()){
      r.insert(s->raster);
      cn.insert(s->color.name());
    }
    if(r.size()<2){
      QMessageBox msgBox;
      msgBox.setText("Please sketch the edge from at least 2 view");
      msgBox.exec();
      return;
    }
    if(cn.size()!=1){
      QMessageBox msgBox;
      msgBox.setText("Please use the same color for all the sketches in the same edge ");
      msgBox.exec();
      return;
    }

    wizardLabel->setText("Now sketch the SECOND edge of the interpolated surface in at least 2 view and press \"NEXT\"");
    wizard_step = STEP_2;
    engine->sketch_color = QColor(Qt::cyan);
    photo_widget->color_swatch->setStyleSheet(COLOR_STYLE.arg(engine->sketch_color.name())
                                              .arg(getIdealTextColor(engine->sketch_color).name()));

    engine->sketching_permitted = true;


  } else if(wizard_step == STEP_2) {

    // regroups all sketches by color name
    QMap< QString, QList<sketchID> > sketch_groups;
    foreach (sketchID id, ds->sketches.keys()) {
      sketch_groups[ds->sketches[id]->color.name()].append(id);
    }

    // check that there are exactly 2 colors:
    if(sketch_groups.keys().size()!=2){
      QMessageBox msgBox;
      msgBox.setText("Please use the same color for all the sketches in the same edge ");
      msgBox.exec();
      return;
    }

    //check again that each color is sketched in at least 2 rasters
    foreach (QString color_name, sketch_groups.keys()) {
      QSet< rasterID > r;
      foreach (sketchID id, sketch_groups[color_name]) {
        r.insert(ds->sketches[id]->raster);
      }
      if(r.size()<2){
        QMessageBox msgBox;
        msgBox.setText("Please sketch each edge from at least 2 view");
        msgBox.exec();
        return;
      }
    }

    raise_splash();
    // foreach color group make an edge
    foreach (QString color_name, sketch_groups.keys()) {
      //select relevant skectches
      QList< Sketch* > sketches;
      foreach (sketchID id, sketch_groups[color_name]) {
        sketches.append(ds->sketches[id]);
      }
      if(!engine->make_edge(sketches,color_name)){
        close_splash();
        QMessageBox msgBox;
        msgBox.setText("Cannot create a surface with the sketches provided, please try again.");
        msgBox.exec();
        onCancelAction();
        return;
      }
    }

    // generate the quadric
    GTMInputParams params;
    params.SurfaceType = GTM_INTERPOLATED;
    if(!engine->make_surface(ds->edges.values(),QColor(Qt::lightGray).name(),&params)){
      close_splash();
      QMessageBox msgBox;
      msgBox.setText("Cannot create a surface with the sketches provided, please try again.");
      msgBox.exec();
      onCancelAction();
      return;
    }

    close_splash();

    //go back to normal view:
    wizard_mode = false;
    wizard_kind = NONE;
    wizard_step = NO_STEP;
    wizardLabel->setText("");
    editToolBar->show();
    advancedEditToolBar->hide();

    engine->delete_all_edges();
    engine->delete_all_sketches();
    engine->sketch_color = QColor(Qt::red);
    sub_2d->show();
    sub_3d->show();
    mdi_area->tileSubWindows();
    wizardToolBar->hide();

  } else qFatal("");
}

void MainWindow::next_gtmo_action()
{
  Q_ASSERT(wizard_mode);
  Q_ASSERT(wizard_kind == GTM_ORTHOGONAL);
  Q_ASSERT(wizard_step != NO_STEP);

  if(wizard_step == STEP_1) {

    // check if we have at least 2 sketches of the same color in different rasters
    QSet< rasterID > r;
    QSet< QString > cn;
    foreach(Sketch *s,ds->sketches.values()){
      r.insert(s->raster);
      cn.insert(s->color.name());
    }
    if(r.size()<2){
      QMessageBox msgBox;
      msgBox.setText("Please sketch the edge from at least 2 view");
      msgBox.exec();
      return;
    }
    if(cn.size()!=1){
      QMessageBox msgBox;
      msgBox.setText("Please use the same color for all the sketches in the same edge ");
      msgBox.exec();
      return;
    }

    wizardLabel->setText("Now sketch the edge for the EXTRUSION of surface in at least 2 view and press \"NEXT\"");
    wizard_step = STEP_2;
    engine->sketch_color = QColor(Qt::yellow);
    photo_widget->color_swatch->setStyleSheet(COLOR_STYLE.arg(engine->sketch_color.name())
                                              .arg(getIdealTextColor(engine->sketch_color).name()));

    engine->sketching_permitted = true;

  } else if(wizard_step == STEP_2) {

    // regroups all sketches by color name
    QMap< QString, QList<sketchID> > sketch_groups;
    foreach (sketchID id, ds->sketches.keys()) {
      sketch_groups[ds->sketches[id]->color.name()].append(id);
    }

    // check that there are exactly 2 colors:
    if(sketch_groups.keys().size()!=2){
      QMessageBox msgBox;
      msgBox.setText("Please use the same color for all the sketches in the same edge ");
      msgBox.exec();
      return;
    }

    //check again that each color is sketched in at least 2 rasters
    foreach (QString color_name, sketch_groups.keys()) {
      QSet< rasterID > r;
      foreach (sketchID id, sketch_groups[color_name]) {
        r.insert(ds->sketches[id]->raster);
      }
      if(r.size()<2){
        QMessageBox msgBox;
        msgBox.setText("Please sketch each edge from at least 2 view");
        msgBox.exec();
        return;
      }
    }

    raise_splash();
    // foreach color group make an edge
    foreach (QString color_name, sketch_groups.keys()) {
      //select relevant skectches
      QList< Sketch* > sketches;
      foreach (sketchID id, sketch_groups[color_name]) {
        sketches.append(ds->sketches[id]);
      }
      if(!engine->make_edge(sketches,color_name)){
        close_splash();
        QMessageBox msgBox;
        msgBox.setText("Cannot create a surface with the sketches provided, please try again.");
        msgBox.exec();
        onCancelAction();
        return;
      }
    }

    // generate the quadric
    GTMInputParams params;
    params.SurfaceType = GTM_ORTHOGONAL;
    if(!engine->make_surface(ds->edges.values(),QColor(Qt::lightGray).name(),&params)){
      close_splash();
      QMessageBox msgBox;
      msgBox.setText("Cannot create a surface with the sketches provided, please try again.");
      msgBox.exec();
      onCancelAction();
      return;
    }

    close_splash();

    //go back to normal view:
    wizard_mode = false;
    wizard_kind = NONE;
    wizard_step = NO_STEP;
    wizardLabel->setText("");
    editToolBar->show();
    advancedEditToolBar->hide();

    engine->delete_all_edges();
    engine->delete_all_sketches();
    engine->sketch_color = QColor(Qt::red);
    sub_2d->show();
    sub_3d->show();
    mdi_area->tileSubWindows();
    wizardToolBar->hide();

  } else qFatal("");
}

void MainWindow::onGotoAdvancedAction()
{
  engine->sketching_permitted = true;
  advancedEditToolBar->show();
  wizardToolBar->hide();
  editToolBar->hide();
}

void MainWindow::onGotoSimpleAction()
{
  engine->sketching_permitted = false;
  editToolBar->show();
  wizardToolBar->hide();
  advancedEditToolBar->hide();
}

void MainWindow::open()
{
  QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("Open MLP"),
                                                  QDir::currentPath(),
                                                  tr("Meshlab File (*.mlp)"));

  if(fileName.isEmpty()) {
    return;
  }

  raise_splash();

  ds->load_MLP(fileName);
  photo_widget->reset_zoom();
  photo_widget->update();
  trackball_widget->update();

  close_splash();

  sub_3d->hide();
  sub_3d->show();
}

void MainWindow::save()
{
  if(engine->count_surfaces()==0)
    return;

  QDialog *dialog = new QDialog;
  QPushButton *pb = new QPushButton("Close gaps between surfaces (makes single .ply)");
  pb->setCheckable(true);

  QPushButton *cb = new QPushButton("Colorize surfaces (makes single .ply)");
  cb->setCheckable(true);
  cb->setChecked(true);

  QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

  connect(bb, SIGNAL(accepted()), dialog, SLOT(accept()));
  connect(bb, SIGNAL(rejected()), dialog, SLOT(reject()));

  QVBoxLayout *l = new QVBoxLayout();
  l->addWidget(new QLabel("Save options:"));
  l->addWidget(pb);
  l->addWidget(cb);
  l->addWidget(bb);

  dialog->setLayout(l);

  int ret = dialog->exec();

  if(!ret)
    return;


  if(!pb->isChecked() && !cb->isChecked()){
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save MLP"),
                                                    QDir::currentPath(),
                                                    tr("Meshlab File (*.mlp)"));

    if(!fileName.endsWith(".mlp",Qt::CaseInsensitive))
      fileName.append(".mlp");

    raise_splash();
    bool r=ds->save_MLP(fileName);
    close_splash();

    if(!r){
      QMessageBox msgBox;
      msgBox.setText("Cannot save surfaces");
      msgBox.exec();
    }
  } else {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save PLY"),
                                                    QDir::currentPath(),
                                                    tr("PLY File (*.ply)"));

    if(!fileName.endsWith(".ply",Qt::CaseInsensitive))
      fileName.append(".ply");

    raise_splash();
    bool r=engine->save_total(fileName,pb->isChecked(), cb->isChecked());
    close_splash();

    if(!r){
      QMessageBox msgBox;
      msgBox.setText("Cannot save surfaces");
      msgBox.exec();
    }

  }
}

void MainWindow::onCylinderAction()
{
  engine->sketching_permitted = true;

  wizard_mode = true;
  wizard_kind = CYLINDER;
  wizard_step = STEP_1;
  wizardLabel->setText("Sketch the Cylinder Area in at least 2 view and press \"NEXT\"");
  engine->delete_all_edges();
  engine->delete_all_sketches();
  engine->sketch_color = QColor(Qt::red);
  photo_widget->color_swatch->setStyleSheet(COLOR_STYLE.arg(engine->sketch_color.name())
                                            .arg(getIdealTextColor(engine->sketch_color).name()));

  editToolBar->hide();
  advancedEditToolBar->hide();
  wizardToolBar->show();
  sub_2d->show();
  sub_3d->hide();
  mdi_area->tileSubWindows();
}

void MainWindow::onQuadricAction()
{
  engine->sketching_permitted = true;

  wizard_mode = true;
  wizard_kind = QUADRIC;
  wizard_step = STEP_1;
  wizardLabel->setText("Sketch the Quadric Area in at least 2 view and press \"NEXT\"");
  engine->delete_all_edges();
  engine->delete_all_sketches();
  engine->sketch_color = QColor(Qt::green);
  photo_widget->color_swatch->setStyleSheet(COLOR_STYLE.arg(engine->sketch_color.name())
                                            .arg(getIdealTextColor(engine->sketch_color).name()));
  editToolBar->hide();
  advancedEditToolBar->hide();
  wizardToolBar->show();
  sub_2d->show();
  sub_3d->hide();
  mdi_area->tileSubWindows();
}

void MainWindow::onGTMIAction()
{
  engine->sketching_permitted = true;

  wizard_mode = true;
  wizard_kind = GTM_INTERPOLATED;
  wizard_step = STEP_1;
  wizardLabel->setText("Sketch the first edge of the interpolated surface in at least 2 view and press \"NEXT\"");
  engine->delete_all_edges();
  engine->delete_all_sketches();
  engine->sketch_color = QColor(Qt::blue);
  photo_widget->color_swatch->setStyleSheet(COLOR_STYLE.arg(engine->sketch_color.name())
                                            .arg(getIdealTextColor(engine->sketch_color).name()));
  editToolBar->hide();
  advancedEditToolBar->hide();
  wizardToolBar->show();
  sub_2d->show();
  sub_3d->hide();
  mdi_area->tileSubWindows();
}

void MainWindow::onGTMOAction()
{
  engine->sketching_permitted = true;

  wizard_mode = true;
  wizard_kind = GTM_ORTHOGONAL;
  wizard_step = STEP_1;
  wizardLabel->setText("Sketch the main edge of the extruded surface in at least 2 view and press \"NEXT\"");
  engine->delete_all_edges();
  engine->delete_all_sketches();
  engine->sketch_color = QColor(Qt::magenta);
  photo_widget->color_swatch->setStyleSheet(COLOR_STYLE.arg(engine->sketch_color.name())
                                            .arg(getIdealTextColor(engine->sketch_color).name()));
  editToolBar->hide();
  advancedEditToolBar->hide();
  wizardToolBar->show();
  sub_2d->show();
  sub_3d->hide();
  mdi_area->tileSubWindows();
}



void MainWindow::onAdvancedEdgeAction()
{
  SketchSelectionDialog dialog(this);
  int ret=dialog.exec();
  //  qDebug()<<ret<<dialog.selection<<dialog.NumberOfNewSamples<<dialog.aspectratio;

  if(ret==0){
    LOG_INFO()<<"operation canceled";
    return;
  }

  // regroups selected sketches by color name
  QMap< QString, QList<sketchID> > sketch_groups;
  foreach (sketchID id, ds->sketches.keys()) {
    if(dialog.selection.contains(id))
      sketch_groups[ds->sketches[id]->color.name()].append(id);
    ds->sketches[id]->already_used=true;
  }

  // foreach color group make an edge
  foreach (QString color_name, sketch_groups.keys()) {

    //select relevant skectches
    QList< Sketch* > sketches;

    foreach (sketchID id, sketch_groups[color_name]) {
      sketches.append(ds->sketches[id]);
    }

    raise_splash();
    engine->make_edge(sketches,color_name,dialog.NumberOfNewSamples,dialog.aspectratio);
    close_splash();
  }
}

void MainWindow::onNextAction()
{
  Q_ASSERT(wizard_mode);
  Q_ASSERT(wizard_kind != NONE);
  Q_ASSERT(wizard_step != NO_STEP);

  engine->sketching_permitted = false;


  switch (wizard_kind) {
  case CYLINDER:
    next_cyl_action();
    break;
  case QUADRIC:
    next_quad_action();
    break;
  case GTM_INTERPOLATED:
    next_gtmi_action();
    break;
  case GTM_ORTHOGONAL:
    next_gtmo_action();
    break;
  default:
    qFatal("");
    break;
  }
}

void MainWindow::onCancelAction()
{
  engine->sketching_permitted = false;

  wizard_mode = false;
  wizard_kind = NONE;
  wizard_step = NO_STEP;
  wizardLabel->setText("");
  editToolBar->show();
  advancedEditToolBar->hide();
  engine->delete_all_edges();
  engine->delete_all_sketches();
  engine->sketch_color = QColor(Qt::red);
  photo_widget->color_swatch->setStyleSheet(COLOR_STYLE.arg(engine->sketch_color.name())
                                            .arg(getIdealTextColor(engine->sketch_color).name()));
  sub_2d->show();
  sub_3d->show();
  mdi_area->tileSubWindows();
  wizardToolBar->hide();
}



void MainWindow::onAdvancedSurfaceAction()
{
  EdgeSelectionDialog dialog(this);
  int ret=dialog.exec();

  if(ret==0){
    LOG_INFO()<<"operation canceled";
    return;
  }

  raise_splash();
  engine->make_surface(dialog.edges,  dialog.edges.at(0)->edge_color.name(), dialog.params);
  close_splash();
}
