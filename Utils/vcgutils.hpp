#ifndef VCGUTILS_HPP_
#define VCGUTILS_HPP_

#include <QtOpenGL>
#include "common.hpp" //note: hack

QPoint as_qpoint(const vcg::Point2f &p);

vcg::Plane3f GetViewPlane(const vcg::View < float > &camera,
                          const vcg::Point3f  &center);
vcg::Point3f HitViewPlane(vcg::Trackball *tb, const vcg::Point3f &p);

bool hit_photo_plane(const vcg::Point2f &p, const vcg::Plane3f &photo_plane, const vcg::Shotf &camera, vcg::Point3f &plane_point);


#endif // VCGUTILS_HPP_
