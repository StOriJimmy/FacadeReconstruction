#include "trackball_widget.hpp"

TrackballWidget::TrackballWidget(QWidget *parent)
  :QWidget(parent)
  ,ds(DataStore::data())
  ,engine(Engine::engine())
{

  QGLFormat glFormat;
  glFormat.setVersion(4, 1);
  glFormat.setSampleBuffers(true);
  glarea = new GLTrackball(glFormat,this);
  glarea->setBackgroundRole(QPalette::Base);
  glarea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  glarea->setCursor(Qt::CrossCursor);

  create_actions();
  create_toolbars();

  QVBoxLayout* vbox = new QVBoxLayout(this);
  vbox->addWidget(toolBar);
  vbox->addWidget(glarea);
  vbox->setContentsMargins(0,0,0,0);
  vbox->setSpacing(0);
  setLayout(vbox);
}

void TrackballWidget::add_raster()
{
  glarea->save_buffer=true;
  update();

}

void TrackballWidget::update()
{
  QWidget::update();
  glarea->updateGL();
}

void TrackballWidget::delete_all_surfaces()
{
  qDebug()<<"here";
  bool has_surfaces = false;
  foreach (meshID id, ds->meshes.keys()) {
    if(ds->meshes[id]->mesh_kind == SURFACE){
      has_surfaces=true;
      break;
    }
  }
  if(!has_surfaces)
    return;

  QMessageBox msgBox;
  msgBox.setText("Do you want to delete ALL generated surfaces?");
  msgBox.setInformativeText("You cannot recover this action");
  msgBox.setStandardButtons(QMessageBox::Ok |QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  int ret = msgBox.exec();

  if(ret==QMessageBox::Ok)
    engine->delete_all_surfaces();
}

void TrackballWidget::delete_last_surface()
{
  if(engine->surface_undo_stack.isEmpty())
    return;

  QMessageBox msgBox;
  msgBox.setText("Do you want to delete the last generated surface?");
  msgBox.setInformativeText("You cannot recover this action");
  msgBox.setStandardButtons(QMessageBox::Ok |QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  int ret = msgBox.exec();

  if(ret==QMessageBox::Ok){
    meshID id = engine->surface_undo_stack.takeLast();
    Mesh *s = ds->meshes[id];
    ds->meshes.remove(id);
    delete s;
  }



}

void TrackballWidget::create_actions()
{
  addRasterAction = new QAction(QIcon(":/icons/snapshot.png"), tr("Save view as new raster"), this);
  addRasterAction->setShortcut(Qt::Key_Plus);
  connect(addRasterAction, SIGNAL(triggered()), this, SLOT(add_raster()));

  draw_pointcloud = new QAction(QIcon(":/icons/points.png"), "Pointcloud",this);
  draw_pointcloud->setCheckable(true);
  connect(draw_pointcloud,SIGNAL(toggled(bool)),glarea,SLOT(set_draw_pointcloud(bool)));
  draw_pointcloud->setChecked(true);

  draw_edges = new QAction(QIcon(":/icons/selected_vert.png"), "Edges",this);
  draw_edges->setCheckable(true);
  connect(draw_edges,SIGNAL(toggled(bool)),glarea,SLOT(set_draw_edges(bool)));
  draw_edges->setChecked(true);
  draw_edges->setChecked(false);

  draw_surfaces = new QAction(QIcon(":/icons/flatlines.png"), "Surfaces",this);
  draw_surfaces->setCheckable(true);
  connect(draw_surfaces,SIGNAL(toggled(bool)),glarea,SLOT(set_draw_surfaces(bool)));
  draw_surfaces->setChecked(true);

  draw_trackball = new QAction(QIcon(":/icons/no_edit.png"), "Trackball",this);
  draw_trackball->setCheckable(true);
  connect(draw_trackball,SIGNAL(toggled(bool)),glarea,SLOT(set_draw_trackball(bool)));
  draw_trackball->setChecked(true);

  draw_cameras = new QAction(QIcon(":/icons/Hardware-Video-Camera-icon.png"), "Cameras",this);
  draw_cameras->setCheckable(true);
  connect(draw_cameras,SIGNAL(toggled(bool)),glarea,SLOT(set_draw_cameras(bool)));
  draw_cameras->setChecked(true);
  draw_cameras->setChecked(false);

  deleteSurfacesAction =  new QAction(QIcon(":/icons/editdelete.ico"),tr("Delete all surfaces"), this);
  connect(deleteSurfacesAction, SIGNAL(triggered()), this, SLOT(delete_all_surfaces()));

  deleteLastSurfaceAction =  new QAction(QIcon(":/icons/undo.ico"),tr("Delete last surface"), this);
  connect(deleteLastSurfaceAction, SIGNAL(triggered()), this, SLOT(delete_last_surface()));

}
void TrackballWidget::create_toolbars()
{
  toolBar = new QToolBar(this);
  //  toolBar->addAction(addRasterAction);
  //  toolBar->addSeparator();
  //  toolBar->addWidget(new QLabel("Render:"));
  toolBar->addAction(draw_pointcloud);
  toolBar->addAction(draw_edges);
  toolBar->addAction(draw_surfaces);
  toolBar->addAction(draw_trackball);
  toolBar->addAction(draw_cameras);
  toolBar->addSeparator();
  toolBar->addAction(deleteSurfacesAction);
  toolBar->addAction(deleteLastSurfaceAction);

}
