#include "engine.hpp"
#include "back_end.hpp"
#include "spline.hpp"

Engine *Engine::instance = NULL; // singleton instance

Engine* Engine::engine()
{
  if(instance==NULL)
    instance = new Engine;

  return instance;
}

void Engine::add_sketch_input(const UserInput &input)
{
  Raster* curr_rast = ds->rasters[ds->current_raster];
  if(input.input_type == UserInput::BEGIN){
    Sketch *sketch = new Sketch(curr_rast->id);
    sketch->color = sketch_color;
    ds->sketches[sketch->id] = sketch;
    curr_rast->sketches.push_back(sketch->id);
    sketch->points.append(SketchPoint(input.point.x()/curr_rast->w,
                                      input.point.y()/curr_rast->h,
                                      tool_size*(0.5f+0.5f*input.tablet_pressure)));
  }else if(input.input_type == UserInput::CONTINUE){
    Sketch *sketch = ds->sketches[curr_rast->sketches.last()];
    SketchPoint sp(input.point.x()/curr_rast->w,
                   input.point.y()/curr_rast->h,
                   tool_size*(0.5f+0.5f*input.tablet_pressure));
    //add point (if different)
    if(sketch->points.size()==0 ||
       sp.distance_from(sketch->points.last()) > 4.0/curr_rast->w){
      sketch->points.append(sp);
    }

  }else if(input.input_type == UserInput::END){

    Sketch *sketch = ds->sketches[curr_rast->sketches.last()];
    SketchPoint sp(input.point.x()/curr_rast->w,
                   input.point.y()/curr_rast->h,
                   tool_size*(0.5f+0.5f*input.tablet_pressure));

    //add last point (if different)
    if(sketch->points.size()==0 ||
       sp.distance_from(sketch->points.last()) > 4.0/curr_rast->w){
      sketch->points.append(sp);
    }

    //resample along cubic spline
    if(sketch->points.size()>1){
      QList< sp::Vec3 > raw_nodes;

      if(sketch->points.size()==2){
        sp::Vec3 a((double)sketch->points.at(0).x*curr_rast->w,(double)sketch->points.at(0).y*curr_rast->h,(double)sketch->points.at(0).variance);
        sp::Vec3 b((double)sketch->points.at(1).x*curr_rast->w,(double)sketch->points.at(1).y*curr_rast->h,(double)sketch->points.at(1).variance);
        sp::Vec3 impostor = (a+b) / 2.0;
        raw_nodes.push_back(a);
        raw_nodes.push_back(impostor);
        raw_nodes.push_back(b);
      }else{
        foreach(SketchPoint p,sketch->points)
          raw_nodes.push_back(sp::Vec3((double)p.x*curr_rast->w,(double)p.y*curr_rast->h,(double)p.variance));
      }

      // at least 3 nodes
      std::vector< qreal > times;
      std::vector< sp::Vec3 > points;
      qreal tot_len=0.0;
      for(int p=0;p<raw_nodes.size();++p){
        if(p>0){
          tot_len += raw_nodes.at(p).distance_from(raw_nodes.at(p-1));
        }
        times.push_back(tot_len);
        points.push_back(raw_nodes.at(p));
      }

      QList< sp::Vec3 > splined_nodes;

      sp::Spline<qreal, sp::Vec3 > spline(times, points);
      qreal t=0.0;
      double delta;
      while(t<tot_len){
        sp::Vec3 n = spline[t];
        splined_nodes.append( n );
        delta = SketchPoint::variance2sizePX(n.z)/2;

        t+=qMax(delta,1.0);
      }
      splined_nodes.append( spline[tot_len] );

      sketch->points.clear();
      foreach(sp::Vec3 p,splined_nodes){
        sketch->points.append(SketchPoint(p.x/curr_rast->w,p.y/curr_rast->h,p.z));
      }


    }

    //find possible other sketch of the same color in the same raster
    bool found=false;
    sketchID found_id;
    foreach(sketchID id, ds->sketches.keys()){
      if(( id != sketch->id ) &&
         ( ds->sketches[id]->raster == sketch->raster ) &&
         ( ds->sketches[id]->color == sketch->color )){
        found = true;
        found_id=id;
        break;
      }
    }
    if(found){
      // merge last with found one and delete last
      ds->sketches[found_id]->points.append(sketch->points);
      ds->sketches.remove(sketch->id);
      curr_rast->sketches.removeLast();
      delete sketch;
    }

  }else{
    qFatal("");
  }

}

void Engine::set_tool_size(int toolbar_value)
{
  tool_size =  toolbar_value / 100.0;
  tool_size_updated = gTime->elapsed();
}

void Engine::delete_sketches_in_current_raster()
{
  Raster* curr_rast = ds->rasters[ds->current_raster];
  QList< sketchID > sketches= curr_rast->sketches;
  foreach (sketchID id, sketches) {
    Sketch *s = ds->sketches[id];

    ds->sketches.remove(id);
    delete s;
  }
  curr_rast->sketches.clear();
}

void Engine::delete_all_sketches()
{
  foreach (rasterID id, ds->rasters.keys()) {
    ds->rasters[id]->sketches.clear();
  }

  QList< sketchID > sketches= ds->sketches.keys();
  foreach (sketchID id, sketches) {
    Sketch *s = ds->sketches[id];

    ds->sketches.remove(id);
    delete s;
  }
}

void Engine::delete_all_edges()
{
  QList< edgeID > edges = ds->edges.keys();
  foreach (edgeID id, edges) {
    Edge *e = ds->edges[id];

    ds->edges.remove(id);
    delete e;
  }
}

void Engine::delete_all_surfaces()
{
  QList< meshID > surfaces = ds->meshes.keys();
  foreach (meshID id, surfaces) {
    if(ds->meshes[id]->mesh_kind == SURFACE){
      Mesh *s = ds->meshes[id];

      ds->meshes.remove(id);
      delete s;
    }
  }
  surface_undo_stack.clear();
}

int Engine::count_surfaces()
{
  int n=0;
  QList< meshID > surfaces = ds->meshes.keys();
  foreach (meshID id, surfaces) {
    if(ds->meshes[id]->mesh_kind == SURFACE){
      n++;
    }
  }
  return n;
}

bool Engine::make_edge(QList< Sketch* > sketches, QString color_name, uint32_t NumberOfNewSamples, double aspectratio)
{
  // output points
  QList< vcg::Point3f > edge_points;

  LOG_INFO()<<"SketchesToEdge";
  SketchesToEdge(sketches, ds->rasters, ds->meshes[ds->point_cloud], edge_points,
      NumberOfNewSamples,aspectratio);
  LOG_INFO()<<edge_points.size();

  if(!edge_points.isEmpty()){
    Edge *e=new Edge;
    e->edge_color = QColor(color_name);

    CMesh::VertexIterator vi = vcg::tri::Allocator<CMesh>::AddVertices(e->mesh,edge_points.size());
    foreach(vcg::Point3f p, edge_points){
      vi->P()=CMesh::CoordType ( p.X(), p.Y(), p.Z());
      QVector<float> fp;

      fp.append(p.X());
      fp.append(p.Y());
      fp.append(p.Z());
      e->points.append(fp);

      ++vi;
    }

    vcg::tri::UpdateBounding<CMesh>::Box(e->mesh);
    // update Normals
    vcg::tri::UpdateNormal<CMesh>::PerVertexNormalizedPerFace(e->mesh);
    vcg::tri::UpdateNormal<CMesh>::PerFaceNormalized(e->mesh);
    // Initialize the opengl wrapper
    e->glWrap.m = &(e->mesh);
    e->glWrap.Update();

    ds->edges[e->id] = e;
    return true;
  }else{
    return false;
  }


}

bool Engine::make_surface(QList<Edge *> edges, QString color_name, SurfaceInputParams *params)
{
  Mesh *m=new Mesh;

  LOG_INFO()<<"EdgesToSurface";
  EdgesToSurface(edges,params,m->mesh);
  LOG_INFO()<<m->mesh.VN()<<m->mesh.FN();

  if(m->mesh.VN()>0){
    m->mesh_kind=SURFACE;
    m->surface_kind=params->SurfaceType;
    m->mesh_color = QColor(color_name);

    vcg::tri::UpdateBounding<CMesh>::Box(m->mesh);
    // update Normals
    vcg::tri::UpdateNormal<CMesh>::PerVertexNormalizedPerFace(m->mesh);
    vcg::tri::UpdateNormal<CMesh>::PerFaceNormalized(m->mesh);
    // Initialize the opengl wrapper
    m->glWrap.m = &(m->mesh);
    m->glWrap.Update();

    ds->meshes[m->id] = m;
    surface_undo_stack.append(m->id);
    return true;
  } else {
    return false;
  }


}

bool Engine::save_total(QString filename, bool with_poisson, bool with_color)
{
  QList< Mesh* > surfaces;
  foreach (meshID id, ds->meshes.keys()) {
    if(ds->meshes[id]->mesh_kind == SURFACE){
      surfaces.append(ds->meshes[id]);
    }
  }
  return SaveTotal(filename,surfaces, with_poisson, with_color,ds->mlp_filename);
}

Engine::Engine(const Engine &)
{
  qFatal("");
}

Engine &Engine::operator=(const Engine &)
{
  qFatal("");
  return *this;
}

Engine::Engine()
  :ds(DataStore::data())
  ,sketching_permitted(false)
  ,sketching(false)
  ,sketch_color(Qt::red)
{

}

Engine::~Engine()
{

}
