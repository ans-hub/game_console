// *************************************************************
// File:    polygon.h
// Descr:   represents 2d polygon helpers (works with vertexes)
// URL:     https://github.com/ans-hub/geomath_lib
// *************************************************************

#ifndef GM_POLYGON_2D_H
#define GM_POLYGON_2D_H

#include <vector>

#include "vector.h"
#include "line.h"
#include "trig.h"
#include "helpers.h"

namespace anshub {

using Point = Vector;
using Vertices = std::vector<Point>;  // see note #1 after code

namespace polygon2d {

  void    CheckInvariant(const Vertices&);
  float   Square(const Vertices&);
  Point   Barycenter(const Vertices&, float);
  bool    PointInside(const Vertices&, const Point&);
  bool    PointInside(float, float, float, float, float, float);
  bool    PointInside(const Point&, const Point&, const Point&);
  bool    PointsInside(float, float, float, float, Vertices&);
  bool    PointsInside(float, float, float, float, Vertices&&);
  bool    CutConvex(Vertices&, Vertices&, const Line&);
  void    RotatePoint(float&, float&, float, const TrigTable&);

} // namespace polygon2d

} // namespace anshub

#endif  // GM_POLYGON_2D_H

// Note #1 : this helper functions works with set of vertexes, but not with 
// Polygon object, since Polygon object has much another meanings and data fields
// and not necessary here (Polygon class placed in Draw library)