#include <Logger.h>
#include "vcgutils.hpp"

QPoint as_qpoint(const vcg::Point2f &p)
{
  return QPoint(p.X(),p.Y());
}


vcg::Plane3f GetViewPlane(const vcg::View < float > &camera,
                          const vcg::Point3f  &center)
{
  vcg::Point3f vp = camera.ViewPoint();
  vcg::Plane3f plane;
  vcg::Point3f plane_normal = vp - center;
  plane_normal.Normalize();
  plane.Set(plane_normal, plane_normal.dot(center));
  return plane;
}

vcg::Point3f HitViewPlane(vcg::Trackball *tb, const vcg::Point3f &p)
{
//  LOG_INFO() << "  p" << p.X() <<  p.Y() <<p.Z() ;
  vcg::Plane3f vp = GetViewPlane(tb->camera, tb->center);
//  LOG_INFO() << "  view plane"<< vp.Direction().X() <<  vp.Direction().Y() << vp.Direction().Z() << vp.Offset();
  vcg::Line3fN ln = tb->camera.ViewLineFromWindow(vcg::Point3f(p[0], p[1], 0));
//  LOG_INFO() << "  line" << ln.Origin().X() <<  ln.Origin().Y() <<ln.Origin().Z() << "---"<< ln.Direction().X() <<  ln.Direction().Y() <<ln.Direction().Z();
  vcg::Point3f PonVP;
  vcg::IntersectionPlaneLine < float >(vp, ln, PonVP);
//  LOG_INFO() << "  p on vp" << PonVP.X() <<  PonVP.Y() <<PonVP.Z() ;
  return PonVP;
}


bool hit_photo_plane(const vcg::Point2f &im_p, const vcg::Plane3f &photo_plane, const vcg::Shotf &camera, vcg::Point3f &plane_point)
{
  vcg::Point3f lv0 = camera.GetViewPoint();
  vcg::Point3f lv1 = camera.UnProject(im_p, 100.0f);
  vcg::Line3f li;
  li.SetOrigin(lv0);
  li.SetDirection(lv1);
  li.Normalize();

  bool intersect = vcg::IntersectionPlaneLine(photo_plane, li, plane_point);
  return intersect;
}
