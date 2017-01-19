#include "gl_trackball.hpp"

#include "utils.hpp"
#include "glutils.hpp"
#include "vcgutils.hpp"
#include "vcg/math/shot.h"

GLTrackball::GLTrackball(const QGLFormat &format, QWidget *parent)
  :QGLWidget(format, parent)
  ,ds(DataStore::data())
  ,engine(Engine::engine())
  ,draw_pointcloud(false)
  ,draw_edges(false)
  ,draw_surfaces(false)
  ,draw_trackball(false)
  ,draw_cameras(false)
  ,save_buffer(false)
{
  // disable stuff that may lead to flickering
  //  setAutoFillBackground(false);
  //  setAttribute(Qt::WA_NoSystemBackground, true);
  //  setAttribute(Qt::WA_PaintOnScreen, true);
  //  setAttribute(Qt::WA_Hover, true);
  //  setAttribute(Qt::WA_OpaquePaintEvent, true);
  //  setMouseTracking(true);


  trackball.center = vcg::Point3f(0, 0, 0);
  trackball.radius = 1.0;
  trackball.track.rot.FromAxis(M_PI, vcg::Point3f(1, 0, 0));
  trackball.track.rot *= vcg::Quaternionf(-M_PI_4/2, vcg::Point3f(0, 1, 0));
  //  trackball.track.sca *= 0.90;

  connect(&timer, SIGNAL(timeout()), this, SLOT(refresh()));
  timer.setSingleShot(false);
  timer.setInterval(100);
  timer.start();
}

GLTrackball::~GLTrackball()
{

}

void GLTrackball::initializeGL()
{
  initializeOpenGLFunctions();
  QOpenGLFunctions funcs(QOpenGLContext::currentContext());

  if(! funcs.hasOpenGLFeature(QOpenGLFunctions::NPOTTextures)) {
    qWarning("Cannot enable NPOT textures!");
    exit(EXIT_FAILURE);
  }

  LOG_INFO(QString("Using OpenGL v%1.%2")
           .arg(format().majorVersion())
           .arg(format().minorVersion()));
  log_gl_params();
  LOG_INFO() << "devicePixelRatio" << devicePixelRatio();

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
}

void GLTrackball::resizeGL(int width, int height)
{
  const int w = QTLogicalToDevice(this,width);
  const int h = QTLogicalToDevice(this,height);
  vp = QRect(0, 0, w, h);
  updateGL();

}

void GLTrackball::paintGL()
{
  glEnable(GL_SCISSOR_TEST);
  glViewport(vp.x(), vp.y(), vp.width(), vp.height());
  glScissor(vp.x(), vp.y(), vp.width(), vp.height());
  if(ds->isLoaded())
    glClearColor(0.0, 0.1, 0.1, 1.0);
  else
    glClearColor(0.0, 0.0, 0.0, 1.0);

  if(save_buffer){
    glClearColor(0.1, 0.1, 0.1, 1.0);
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(!ds->isLoaded())
    return;

  // init vars
  cameraDist = 3;//scene->cam_photo_distance;
  nearPlane = cameraDist * 0.1f;
  farPlane = cameraDist + fmax(1.75f, 10.0f);
  fov = 60;
  //  const float viewRatio = 1.15f;
  const GLfloat fAspect = (GLfloat)vp.width() / vp.height();

  // setup gl state
  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
//  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  // setup projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //  glOrtho(-viewRatio * fAspect, viewRatio * fAspect,
  //          -viewRatio, viewRatio,
  //          nearPlane, farPlane);
  gluPerspective(fov, fAspect, nearPlane, farPlane);

  // init modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, cameraDist, 0, 0, 0, 0, 1, 0);

  // apply trackball
  trackball.GetView();
  trackball.Apply();

  // draw 3D objects
  glPushMatrix();
  {

  // retrieve the base point cloud
  Mesh *pc_mesh = ds->meshes[ds->point_cloud];

  // center and scale relative to the point cloud
  float d = 1.5f / pc_mesh->mesh.bbox.Diag();
  vcg::glScale(d);
  glTranslate(-pc_mesh->glWrap.m->bbox.Center());


  // draw edges
  if(draw_edges){
    foreach (edgeID id, ds->edges.keys()) {
      Edge *edge = ds->edges[id];
      glColor4f(edge->edge_color.redF(),
                edge->edge_color.greenF(),
                edge->edge_color.blueF(),
                1.0);
      edge->glWrap.Draw<vcg::GLW::DMPoints, vcg::GLW::CMNone, vcg::GLW::TMNone> ();
    }
  }

  // draw surfaces
  if(draw_surfaces){
    foreach(meshID id,ds->meshes.keys()){
      Mesh * mesh=ds->meshes[id];
      if(mesh->mesh_kind==SURFACE){

        glColor4f(mesh->mesh_color.redF(),
                  mesh->mesh_color.greenF(),
                  mesh->mesh_color.blueF(),
                  1.0);

        mesh->mesh.C() = vcg::Color4b(mesh->mesh_color.red(),
                                      mesh->mesh_color.green(),
                                      mesh->mesh_color.blue(),
                                      255);
        mesh->glWrap.Draw<vcg::GLW::DMFlat, vcg::GLW::CMPerMesh, vcg::GLW::TMNone> ();
      }
    }
  }

  // draw the point cloud
  if(draw_pointcloud){
    glColor4f(pc_mesh->mesh_color.redF(),
              pc_mesh->mesh_color.greenF(),
              pc_mesh->mesh_color.blueF(),
              1.0);
    pc_mesh->glWrap.Draw<vcg::GLW::DMPoints, vcg::GLW::CMNone, vcg::GLW::TMNone> ();

    glColor4f(1.0,1.0,1.0,1.0);
  }




  if(save_buffer){
    save_buffer=false;

    QImage im = grabFrameBuffer(true);

    QPair<vcg::Shotf, float> view=shotFromTrackball();

    vcg::Shotf *shot=new vcg::Shotf;

    *shot=vcg::Shotf::Construct(view.first);
    float track_scale = view.second; //TODO: fix scale
//    qDebug()<<track_scale;

    Raster *r=new Raster;

    r->load(im,shot,QString("Virtual Shot %1").arg(r->id));

    ds->rasters[r->id]=r;
    ds->current_raster=r->id;

    //    im.save("im.png");
  }

  // draw cameras
  if(draw_cameras){
    vcg::Color4b camcolor;
    foreach(rasterID id, ds->rasters.keys()){
      vcg::Shotf *shot = ds->rasters[id]->camera;
      if(ds->current_raster==id)
        camcolor = vcg::Color4b(255, 255, 00, 255);
      else
        camcolor = vcg::Color4b(100, 200, 255, 255);
      glPushMatrix();

      draw_camera(shot,0.0025*cameraDist,camcolor);
      glPopMatrix();

    }
  }

  }

  glPopMatrix();

  // draw trackball
  if(draw_trackball)
    trackball.DrawPostApply();
}

void GLTrackball::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);
  resizeGL(width(), height());

}

void GLTrackball::keyPressEvent(QKeyEvent *event)
{
  event->ignore();

  if(event->key() == Qt::Key_Control) {
    trackball.ButtonDown(QT2VCG(Qt::NoButton, Qt::ControlModifier));
  }

  if(event->key() == Qt::Key_Shift) {
    trackball.ButtonDown(QT2VCG(Qt::NoButton, Qt::ShiftModifier));
  }

  if(event->key() == Qt::Key_Alt) {
    trackball.ButtonDown(QT2VCG(Qt::NoButton, Qt::AltModifier));
  }

  updateGL();
}

void GLTrackball::keyReleaseEvent(QKeyEvent *event)
{
  event->ignore();

  if(event->key() == Qt::Key_Control) {
    trackball.ButtonUp(QT2VCG(Qt::NoButton, Qt::ControlModifier));
  }

  if(event->key() == Qt::Key_Shift) {
    trackball.ButtonUp(QT2VCG(Qt::NoButton, Qt::ShiftModifier));
  }

  if(event->key() == Qt::Key_Alt) {
    trackball.ButtonUp(QT2VCG(Qt::NoButton, Qt::AltModifier));
  }

  updateGL();
}

void GLTrackball::mousePressEvent(QMouseEvent *event)
{
  event->accept();

  setFocus();
  trackball.MouseDown(QT2VCG_X(this, event), QT2VCG_Y(this, event),
                      QT2VCG(event->button(), event->modifiers()));
  updateGL();
}

void GLTrackball::mouseMoveEvent(QMouseEvent *event)
{
  event->accept();

  if(event->buttons()) {
    trackball.MouseMove(QT2VCG_X(this, event), QT2VCG_Y(this, event));
    updateGL();
  }
}

void GLTrackball::mouseReleaseEvent(QMouseEvent *event)
{
  event->accept();

  trackball.MouseUp(QT2VCG_X(this, event), QT2VCG_Y(this, event),
                    QT2VCG(event->button(), event->modifiers()));
  updateGL();
}

void GLTrackball::wheelEvent(QWheelEvent *event)
{
  event->accept();

  const int WHEEL_STEP = 120;
  trackball.MouseWheel(event->delta() / float(WHEEL_STEP),
                       QTWheel2VCG(event->modifiers()));
  updateGL();
}

void GLTrackball::mouseDoubleClickEvent(QMouseEvent *event)
{
  event->ignore();

}

QPointF GLTrackball::map_from_QT_to_vp(QPoint pos)
{
  QPoint gl_p = as_qpoint(QTLogicalToOpenGL(this,pos));
  return QPoint(gl_p.x() - vp.x(), gl_p.y() - vp.y());
}



void GLTrackball::refresh()
{
  // redraw only when the window is active
  if(QApplication::activeWindow()) {
    updateGL();
  } else {
    Sleeper::msleep(30);
  }
}

void GLTrackball::set_draw_trackball(bool value)
{
  draw_trackball = value;
}

void GLTrackball::set_draw_cameras(bool value)
{
  draw_cameras = value;
}

void GLTrackball::set_draw_surfaces(bool value)
{
  draw_surfaces = value;
}

void GLTrackball::set_draw_edges(bool value)
{
  draw_edges = value;
}

void GLTrackball::set_draw_pointcloud(bool value)
{
  draw_pointcloud = value;
}



void GLTrackball::initializeShot(vcg::Shotf &shot)
{
  shot.Intrinsics.PixelSizeMm[0]=0.036916077;
  shot.Intrinsics.PixelSizeMm[1]=0.036916077;

  shot.Intrinsics.DistorCenterPx[0]=width()/2;
  shot.Intrinsics.DistorCenterPx[1]=height()/2;
  shot.Intrinsics.CenterPx[0]=width()/2;
  shot.Intrinsics.CenterPx[1]=height()/2;
  shot.Intrinsics.ViewportPx[0]=width();
  shot.Intrinsics.ViewportPx[1]=height();

  double viewportYMm = shot.Intrinsics.PixelSizeMm[1]*shot.Intrinsics.ViewportPx[1];
  double default_fov = 60.0;
  shot.Intrinsics.FocalMm = viewportYMm/(2*tanf(vcg::math::ToRad(default_fov/2.0f))); //27.846098mm

  shot.Extrinsics.SetIdentity();
}

QPair<vcg::Shotf,float> GLTrackball::shotFromTrackball()
{
  vcg::Shotf shot;
  initializeShot(shot);

  double viewportYMm=shot.Intrinsics.PixelSizeMm[1]*shot.Intrinsics.ViewportPx[1];
  shot.Intrinsics.FocalMm = viewportYMm/(2*tanf(vcg::math::ToRad(fov/2)));

  //    float cameraDist = getCameraDistance();

  //add the translation introduced by gluLookAt() (0,0,cameraDist), in order to have te same view---------------
  //T(gl)*S*R*T(t) => SR(gl+t) => S R (S^(-1)R^(-1)gl + t)
  //Add translation S^(-1) R^(-1)(gl)
  //Shotd doesn't introduce scaling
  //---------------------------------------------------------------------
  shot.Extrinsics.SetTra( shot.Extrinsics.Tra() + (Inverse(shot.Extrinsics.Rot())*vcg::Point3f(0, 0, cameraDist)));

  vcg::Shotf newShot = track2ShotCPU<vcg::Shotf::ScalarType>(shot, &trackball);

  return QPair<vcg::Shotf, float> (newShot,trackball.track.sca);
}
