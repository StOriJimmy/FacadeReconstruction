#include "datastore.hpp"
#include <wrap/qt/shot_qt.h>
#include "utils.hpp"

// Mesh

Mesh::Mesh()
  :mesh_kind(POINT_CLOUD)
  ,surface_kind(NONE)
  ,mesh_color(vcg::Color4f::White)
  ,loaded(false)
{
  static meshID id=0;
  this->id = id++;
  //mesh.vert.EnableTexCoord();
}

Mesh::~Mesh()
{
  //TODO: proper cleanup
}

bool Mesh::load(QString filename,QString label)
{
  if(loaded){
    qFatal("Cannot load the same mesh twice!");
  }

  int failure = vcg::tri::io::ImporterPLY<CMesh>::Open(mesh ,filename.toStdString().c_str());

  if(!failure)
  {
    // update bounding box
    vcg::tri::UpdateBounding<CMesh>::Box(mesh);
    // update Normals
    vcg::tri::UpdateNormal<CMesh>::PerVertexNormalizedPerFace(mesh);
    vcg::tri::UpdateNormal<CMesh>::PerFaceNormalized(mesh);
    // Initialize the opengl wrapper
    glWrap.m = &mesh;
    glWrap.Update();

    if(mesh.FN()==0){
      mesh_kind=POINT_CLOUD;
    } else {
      mesh_kind=POISSON_RECONSTRUCTION;
    }
    surface_kind=NONE;
    this->label=label;

    loaded = true;
  }
  else
  {
    qFatal("cannot open mesh");
    loaded = false;
  }
  return loaded;
}

bool Mesh::save(QString filename)
{
  int failure = vcg::tri::io::ExporterPLY<CMesh>::Save(mesh,filename.toStdString().c_str(),vcg::tri::io::Mask::IOM_VERTCOORD | vcg::tri::io::Mask::IOM_VERTTEXCOORD, false);

  if(!failure)
    return true;

  qFatal("cannot save mesh"); //TODO: graceful fallback
  return false;
}

bool Mesh::isLoaded()
{
  return loaded;
}

Mesh::Mesh(const Mesh &)
{
  qFatal("");
}

Mesh & Mesh::operator=(const Mesh &)
{
  qFatal("");
  return *this;
}

// Raster

Raster::Raster()
  :w(-1)
  ,h(-1)
  ,tex_id(-1)
  ,loaded(false)
{
  static rasterID id=0;
  this->id = id++;
}

Raster::~Raster()
{
  //TODO
}

bool Raster::load(QString filename, vcg::Shotf *shot, QString label)
{
  return load(QImage(filename), shot,label);
}

bool Raster::load(QImage im, vcg::Shotf *shot, QString label)
{
  if(loaded){
    qFatal("Cannot load the same raster twice!");
    return false;
  }

  if(shot==NULL){
    qFatal("NULL shot!");
    return false;
  }


  image = im;
  if(image.isNull()){
    qFatal("null image");
    return false;
  }
  w=image.width();
  h=image.height();
  tex_id=0;
  this->label=label;
  thumbnail = QPixmap::fromImage(image.scaled(100,100,Qt::KeepAspectRatio));

  camera=shot;

  if(w != shot->Intrinsics.ViewportPx.X()){
    qFatal("image width does not match shot");
    return false;
  }
  if(h != shot->Intrinsics.ViewportPx.Y()){
    qFatal("image height does not match shot");
    return false;
  }

  loaded=true;

  return loaded;
}

bool Raster::isLoaded()
{
  return loaded;
}

Raster::Raster(const Raster &)
{
  qFatal("");
}

Raster &Raster::operator =(const Raster&)
{
  qFatal("");
}

//SketchPoint

float SketchPoint::MIN_POINT_SIZE = 3;
float SketchPoint::MAX_POINT_SIZE = 500;

SketchPoint::SketchPoint()
  :x(0)
  ,y(0)
  ,variance(0)
{

}

SketchPoint::SketchPoint(float v)
  :x(v)
  ,y(v)
  ,variance(v)
{

}

SketchPoint::SketchPoint(float x, float y, float variance)
  :x(x)
  ,y(y)
  ,variance(variance)
{

}

SketchPoint::~SketchPoint()
{

}

SketchPoint::SketchPoint(const SketchPoint &o)
  :x(o.x)
  ,y(o.y)
  ,variance(o.variance)
{
}

SketchPoint &SketchPoint::operator=(const SketchPoint &o)
{
  x=o.x;
  y=o.y;
  variance=o.variance;
  return *this;
}


SketchPoint & SketchPoint::operator+=(const SketchPoint &o)
{
  x+=o.x;
  y+=o.y;
  variance+=o.variance;
  return *this;
}

SketchPoint & SketchPoint::operator*=(const SketchPoint &o)
{
  x*=o.x;
  y*=o.y;
  variance*=o.variance;
  return *this;
}

SketchPoint & SketchPoint::operator-=(const SketchPoint &o)
{
  x-=o.x;
  y-=o.y;
  variance-=o.variance;
  return *this;
}

SketchPoint & SketchPoint::operator/=(const SketchPoint &o)
{
  x/=o.x;
  y/=o.y;
  variance/=o.variance;
  return *this;
}

SketchPoint & SketchPoint::operator+=(float f)
{
  x+=f;
  y+=f;
  variance+=f;
  return *this;
}

SketchPoint & SketchPoint::operator*=(float f)
{
  x*=f;
  y*=f;
  variance*=f;
  return *this;
}

SketchPoint & SketchPoint::operator-=(float f)
{
  x-=f;
  y-=f;
  variance-=f;
  return *this;
}

SketchPoint & SketchPoint::operator/=(float f)
{
  x/=f;
  y/=f;
  variance/=f;
  return *this;
}

const SketchPoint SketchPoint::operator+(const SketchPoint &o) const
{
  return SketchPoint(*this) += o;
}

const SketchPoint SketchPoint::operator*(const SketchPoint &o) const
{
  return SketchPoint(*this) *= o;
}

const SketchPoint SketchPoint::operator-(const SketchPoint &o) const
{
  return SketchPoint(*this) -= o;
}

const SketchPoint SketchPoint::operator/(const SketchPoint &o) const
{
  return SketchPoint(*this) /= o;
}

const SketchPoint SketchPoint::operator+(float f) const
{
  return SketchPoint(*this) += f;
}

const SketchPoint SketchPoint::operator*(float f) const
{
  return SketchPoint(*this) *= f;
}

const SketchPoint SketchPoint::operator-(float f) const
{
  return SketchPoint(*this) -= f;
}

const SketchPoint SketchPoint::operator/(float f) const
{
  return SketchPoint(*this) /= f;
}

bool SketchPoint::operator==(const SketchPoint &o) const
{
  return (x==o.x)
      && (y==o.y)
      && (variance==o.variance)
      ;
}
bool SketchPoint::operator!=(const SketchPoint &o) const
{
  return !(*this == o);
}

float SketchPoint::distance_from2(const SketchPoint &o) const
{
  return (*this-o).length2();
}

float SketchPoint::distance_from(const SketchPoint &o) const
{
  return  (*this-o).length();
}

float SketchPoint::length2() const
{
  return x*x + y*y;
}

float SketchPoint::length() const
{
  return sqrtf(length2());
}

SketchPoint& SketchPoint::normalize()
{
  float len2 = length2();
  if(len2>0)
    (*this) *= 1.0f/sqrtf(len2);
  else
    throw std::runtime_error("division by zero!");
  return *this;
}

SketchPoint SketchPoint::normalized() const
{
  SketchPoint n (*this); // make copy
  return n.normalize(); // return normalized copy
}

// size in pixels at original resolution
float SketchPoint::sizePX() const
{
  return MIN_POINT_SIZE + (variance * (MAX_POINT_SIZE - MIN_POINT_SIZE));
}

float SketchPoint::variance2sizePX(float variance)
{
  return MIN_POINT_SIZE + (variance * (MAX_POINT_SIZE - MIN_POINT_SIZE));
}

//Sketch

Sketch::Sketch(rasterID raster)
  :raster(raster)
  ,points()
  ,color()
  ,already_used(false)
{
  static sketchID id=0;
  this->id = id++;
}

Sketch::Sketch(const Sketch &o)
  :id(o.id)
  ,raster(o.raster)
  ,points(o.points)
  ,color(o.color)
  ,already_used(o.already_used)
{

}

Sketch::~Sketch()
{

}

Sketch &Sketch::operator=(const Sketch &o)
{
  id=o.id;
  points=o.points;
  points=o.points;
  color=o.color;
  already_used=o.already_used;
  return *this;

}

Sketch::Sketch()
{
  qFatal("");
}

//Edge

Edge::Edge()

  :points()
  ,edge_color()
  ,mesh()
  ,glWrap()
  ,already_used(false)
{
  static edgeID id=0;
  this->id = id++;
}

Edge::Edge(const Edge &o)
  :id(o.id)
  ,points(o.points)
  ,edge_color(o.edge_color)
  //  ,mesh(o.mesh)  //TODO fix
  ,glWrap(o.glWrap)
  ,already_used(o.already_used)
{

}

Edge &Edge::operator=(const Edge &o)
{
  id=o.id;
  points=o.points;
  edge_color=o.edge_color;
  //  mesh=o.mesh; //TODO fix
  glWrap=o.glWrap;
  already_used=o.already_used;
  return *this;
}


// Datastore

DataStore *DataStore::instance = NULL; // singleton instance

DataStore* DataStore::data()
{
  if(instance==NULL)
    instance = new DataStore;

  return instance;
}

bool DataStore::load_MLP(QString filename)
{
  if(loaded){
    QMessageBox msgBox;
    msgBox.setText("loading a new scene when one is currently loaded is not supported yet, please restart the app to do so.");
    msgBox.exec();
    return false;
  }



  LOG_INFO()<<"loading MLP"<<filename;

  QFile qf(filename);
  QFileInfo qfInfo(filename);
  mlp_filename = qfInfo.absoluteFilePath();

  QDir tmpDir = QDir::current();
  QDir::setCurrent(qfInfo.absoluteDir().absolutePath());
  if( !qf.open(QIODevice::ReadOnly ) ){
    qFatal("cannot open file");
    return false;
  }


  QDomDocument doc("MeshLabDocument");    //It represents the XML document
  if(!doc.setContent( &qf )){
    qFatal("cannot open populate dom");
    return false;
  }

  QDomElement root = doc.documentElement();

  QDomNode node;

  node = root.firstChild();

  while(!node.isNull()){

    if(QString::compare(node.nodeName(),"MeshGroup")==0)
    {
      QDomNode mesh_node = node.firstChild();
      while(!mesh_node.isNull()){
        QString mesh_filename = qfInfo.absoluteDir().absolutePath() + "/"
            +mesh_node.attributes().namedItem("filename").nodeValue();
        QString mesh_label = mesh_node.attributes().namedItem("label").nodeValue();

        QDomNode tr_node=mesh_node.firstChild();

        if(!tr_node.isNull() && QString::compare(tr_node.nodeName(),"MLMatrix44")==0)
        {
          if (tr_node.childNodes().size() == 1)
          {
            QString v = tr_node.firstChild().nodeValue().split("\n", QString::SkipEmptyParts).join(" ");
            QStringList values =  v.split(" ",QString::SkipEmptyParts);
            vcg::Matrix44f trm;
            for(int y = 0; y < 4; y++)
              for(int x = 0; x < 4; x++)
                trm[y][x] = values[x + 4*y].toFloat();

            if(trm != vcg::Matrix44f::Identity()){
              qFatal("expected identity MLMatrix");
              return false;
            }
          }
        }

        Mesh *mesh = new Mesh;
        if(!mesh->load(mesh_filename, mesh_label)){
          qFatal("cannot load mesh");
          return false;
        }
        meshes[mesh->id] = mesh;
        LOG_INFO()<<"Loaded mesh"<<mesh->label;

        if(mesh->mesh_kind==POINT_CLOUD){
          mesh->mesh_color = Qt::white;
          point_cloud = mesh->id;
        } else if (mesh->mesh_kind==POISSON_RECONSTRUCTION){
          mesh->mesh_color = Qt::blue;

          poisson_input = mesh->id;

        } else {
          qFatal("");
          return false;
        }

        mesh_node = mesh_node.nextSibling();
      }
    } else if(QString::compare(node.nodeName(),"RasterGroup")==0) {
      QDomNode raster_node = node.firstChild();

      while(!raster_node.isNull())
      {
        QString raster_label=raster_node.attributes().namedItem("label").nodeValue();

        QDomNode camera_node=raster_node.firstChildElement("VCGCamera");

        vcg::Shotf* shot = new vcg::Shotf;
        if(!ReadShotFromQDomNode(*shot,camera_node)){
          qFatal("cannot load shot");
          return false;
        }

        QString image_filename;
        QDomElement plane_node = raster_node.firstChildElement("Plane");
        if(!plane_node.isNull())
        {
          image_filename = qfInfo.absoluteDir().absolutePath() + "/"
              +  plane_node.attribute("fileName").split("/",QString::SkipEmptyParts).join("/");

          plane_node = node.nextSiblingElement("Plane");
        }else{
          qFatal("missing Plane node");
          return false;
        }

        Raster *raster=new Raster;
        if(!raster->load(image_filename,shot,raster_label)){
          qFatal("cannot load raster");
          return false;
        }
        rasters[raster->id] = raster;
        LOG_INFO()<<"Loaded raster"<<raster->label;


        if(rasters.size()==1){
          current_raster=raster->id;
        }

        raster_node=raster_node.nextSibling();
      }
    }
    node = node.nextSibling();
  }

  QDir::setCurrent(tmpDir.absolutePath());
  qf.close();

  loaded=true;

  return loaded;
}

bool DataStore::isLoaded()
{
  return loaded;
}

bool DataStore::save_MLP(QString filename)
{
  QFileInfo fi(filename);
  QDir tmpDir = QDir::current();
  QDir::setCurrent(fi.absoluteDir().absolutePath());

  QString mlp("<!DOCTYPE MeshLabDocument>\n"
              " <MeshLabProject>\n"
              "  <MeshGroup>\n");

  QString fpre=filename.left(filename.length()-4);

  QList< meshID > surfaces = meshes.keys();
  foreach (meshID id, surfaces) {
    if(meshes[id]->mesh_kind == SURFACE){
      Mesh *s = meshes[id];
      QString mlabel;
      switch (s->surface_kind) {
      case CYLINDER:
        mlabel=QString("%1_surface_%2_cylinder").arg(fpre).arg(id);
        break;
      case QUADRIC:
        mlabel=QString("%1_surface_%2_quadric").arg(fpre).arg(id);
        break;
      case GTM_INTERPOLATED:
        mlabel=QString("%1_surface_%2_gtm_interpolated").arg(fpre).arg(id);
        break;
      case GTM_ORTHOGONAL:
        mlabel=QString("%1_surface_%2_gtm_orthogonal").arg(fpre).arg(id);
        break;
      default:
        qFatal("");
        break;
      }
      QString mfilename(mlabel+".ply");

      mlp.append(QString("  <MLMesh label=\"%1\" filename=\"%2\">\n"
                         "   <MLMatrix44>\n"
                         "1 0 0 0 \n"
                         "0 1 0 0 \n"
                         "0 0 1 0 \n"
                         "0 0 0 1 \n"
                         "</MLMatrix44>\n"
                         "  </MLMesh>\n").arg(mlabel).arg(mfilename));

      if(!s->save(mfilename))
        return false;

    }
  }

  mlp.append(" </MeshGroup>\n"
             " <RasterGroup/>\n"
             "</MeshLabProject>\n");

  writefile(filename,mlp);

  QDir::setCurrent(tmpDir.absolutePath());


  return true;
}


DataStore::DataStore()
  :point_cloud(0)
  ,poisson_input(0)
  ,current_raster(0)
  ,loaded(false)
{
  //  vcg::tri::Cylinder(6, 3, mesh);

  //  // update bounding box
  //  vcg::tri::UpdateBounding<CMesh>::Box(mesh);
  //  // update Normals
  //  vcg::tri::UpdateNormal<CMesh>::PerVertexNormalizedPerFace(mesh);
  //  vcg::tri::UpdateNormal<CMesh>::PerFaceNormalized(mesh);
  //  // Initialize the opengl wrapper
  //  glWrap.m = &mesh;
  //  glWrap.Update();
}

DataStore::DataStore(const DataStore &)
{
  qFatal("");
}

DataStore &DataStore::operator=(const DataStore &)
{
  qFatal("");
  return *this;
}

