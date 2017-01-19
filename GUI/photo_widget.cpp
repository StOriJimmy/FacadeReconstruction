#include "photo_widget.hpp"




PhotoWidget::PhotoWidget(QWidget *parent)
  :QWidget(parent)
  ,ds(DataStore::data())
  ,engine(Engine::engine())
{

  QGLFormat glFormat;
  glFormat.setVersion(4, 1);
  glFormat.setSampleBuffers(true);
  glarea = new GLPhoto(glFormat,this);
  glarea->setBackgroundRole(QPalette::Base);
  glarea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  glarea->setCursor(Qt::CrossCursor);

  toolSizeSlider = new QSlider(Qt::Horizontal);
  toolSizeSlider->setMinimum(0);
  toolSizeSlider->setMaximum(100);
  toolSizeSlider->setMinimumWidth(100);
  connect(toolSizeSlider,SIGNAL(valueChanged(int)),engine,SLOT(set_tool_size(int)));
  toolSizeSlider->setValue(10);

  create_actions();
  create_toolbars();

  QVBoxLayout* vbox = new QVBoxLayout(this);
  vbox->addWidget(toolBar);
  vbox->addWidget(glarea);
  vbox->setContentsMargins(0,0,0,0);
  vbox->setSpacing(0);
  setLayout(vbox);
}

void PhotoWidget::reset_zoom()
{
  glarea->rasterChanged();
}

void PhotoWidget::prev_raster()
{
  QList<rasterID> ids=ds->rasters.keys();

  ds->current_raster = ids.at((ids.indexOf(ds->current_raster)-1+ids.size())%ids.size());
  glarea->rasterChanged();
  update();
}

void PhotoWidget::next_raster()
{
  QList<rasterID> ids=ds->rasters.keys();

  ds->current_raster = ids.at((ids.indexOf(ds->current_raster)+1)%ids.size());
  glarea->rasterChanged();

  update();
}

void PhotoWidget::set_color()
{
  QColor c =  QColorDialog::getColor(engine->sketch_color,this,"Select Sketch color");
  if(!c.isValid())
    return;

  engine->sketch_color = c;
  color_swatch->setStyleSheet(COLOR_STYLE.arg(engine->sketch_color.name())
                              .arg(getIdealTextColor(engine->sketch_color).name()));

}

void PhotoWidget::update()
{
  QWidget::update();
  glarea->updateGL();
}

void PhotoWidget::create_actions()
{
  prevRasterAction = new QAction(QIcon(":/icons/1leftarrow.ico"), tr("Prev raster"), this);
  prevRasterAction->setShortcut(Qt::Key_Left);
  connect(prevRasterAction, SIGNAL(triggered()), this, SLOT(prev_raster()));

  nextRasterAction = new QAction(QIcon(":/icons/1rightarrow.ico"), tr("Next raster"), this);
  nextRasterAction->setShortcut(Qt::Key_Right);
  connect(nextRasterAction, SIGNAL(triggered()), this, SLOT(next_raster()));

//  toolActionGroup = new QActionGroup(this);

//  sketchAction = new QAction(QIcon(":/icons/pencil.ico"),tr("Sketch"), this);
//  sketchAction->setCheckable(true);
//  sketchAction->setChecked(true);
//  toolActionGroup->addAction(sketchAction);

//  eraseAction =  new QAction(QIcon(":/icons/eraser.ico"),tr("Erase"), this);
//  eraseAction->setCheckable(true);
//  toolActionGroup->addAction(eraseAction);
//  eraseAction->setEnabled(false); // todo implement

//  selectColorAction =  new QAction(tr("Color"), this);

  color_swatch = new QPushButton(this);
  color_swatch->setStyleSheet(COLOR_STYLE.arg(engine->sketch_color.name())
                              .arg(getIdealTextColor(engine->sketch_color).name()));
  connect(color_swatch, SIGNAL(clicked(bool)), this, SLOT(set_color()));


//  toolActionGroup->addAction(selectColorAction);
//  selectColorAction->setEnabled(false); // todo implement

//  deselectSketchAction =  new QAction(QIcon(":/icons/edit_remove.ico"),tr("Deselect"), this);
//  deselectSketchAction->setCheckable(true);
//  toolActionGroup->addAction(deselectSketchAction);
//  deselectSketchAction->setEnabled(false); // todo implement


//  mergeSelectedSketchesAction =  new QAction(QIcon(":/icons/plus.ico"),tr("Merge selected"), this);
//  mergeSelectedSketchesAction->setEnabled(false); // todo implement

  deleteSketchesAction =  new QAction(QIcon(":/icons/editdelete.ico"),tr("Delete sketches"), this);
  connect(deleteSketchesAction, SIGNAL(triggered()), engine, SLOT(delete_sketches_in_current_raster()));

  reset_pz_action = new QAction(QIcon(":/icons/zoom_fit_best.ico"),tr("Fit to window"), this);
  reset_pz_action->setShortcut(tr("Ctrl+0"));
  connect(reset_pz_action,SIGNAL(triggered()),glarea,SLOT(reset_pan_zoom()));
  connect(glarea,SIGNAL(enable_zoom(bool)),reset_pz_action,SLOT(setEnabled(bool)));
//  reset_pz_action->setEnabled(false);

  zoom_original_action = new QAction(QIcon(":/icons/zoom_original.ico"),tr("Original &Zoom"), this);
  zoom_original_action->setShortcut(tr("Ctrl+="));
  connect(zoom_original_action,SIGNAL(triggered()),glarea,SLOT(zoom_2_original()));
  connect(glarea,SIGNAL(enable_zoom(bool)),zoom_original_action,SLOT(setEnabled(bool)));
//  zoom_original_action->setEnabled(false);

  zoom_out_action = new QAction(QIcon(":/icons/zoom_out.ico"),tr("Zoom out"), this);
  zoom_out_action->setShortcuts(QList<QKeySequence>() << QKeySequence::ZoomOut << QKeySequence(tr("Ctrl+Shift+d")));
  connect(zoom_out_action,SIGNAL(triggered()),glarea,SLOT(zoom_out()));
  connect(glarea,SIGNAL(enable_zoom_out(bool)),zoom_out_action,SLOT(setEnabled(bool)));
//  zoom_out_action->setEnabled(false);

  zoom_in_action = new QAction(QIcon(":/icons/zoom_in.ico"),tr("Zoom in"), this);
  zoom_in_action->setShortcuts(QList<QKeySequence>() << QKeySequence::ZoomIn << QKeySequence(tr("Ctrl+Shift+i")));
  connect(zoom_in_action,SIGNAL(triggered()),glarea,SLOT(zoom_in()));
  connect(glarea,SIGNAL(enable_zoom_in(bool)),zoom_in_action,SLOT(setEnabled(bool)));
//  zoom_in_action->setEnabled(false);

}

void PhotoWidget::create_toolbars()
{
  toolBar = new QToolBar(this);
  toolBar->addAction(prevRasterAction);
  toolBar->addAction(nextRasterAction);
  toolBar->addSeparator();
//  toolBar->addAction(sketchAction);
//  toolBar->addAction(eraseAction);


  toolBar->addWidget(color_swatch);
//  toolBar->addAction(deselectSketchAction);
  toolBar->addSeparator();
//  toolBar->addAction(mergeSelectedSketchesAction);
  toolBar->addAction(deleteSketchesAction);
  toolBar->addSeparator();
  toolBar->addWidget(toolSizeSlider);
  toolBar->addSeparator();

  toolBar->addAction( zoom_in_action);
  toolBar->addAction( zoom_out_action);
  toolBar->addAction( reset_pz_action);
  toolBar->addAction( zoom_original_action);


}
