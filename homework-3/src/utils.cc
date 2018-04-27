#include "utils.hpp"
#include <float.h>
#include <cmath>
#include <iostream>
#include <limits>

// Calculate diffuse illumination using phong shading
// Parameters:
//  to_source: a normalized vector from a point on a surface to the light
//  normal: a normalized normal of the surface
//
// Returns:
//  diffuse contribution to the illumination in rgb
Color get_diffuse(const Coordinate &to_source, const Coordinate &normal) {
  Color k(0.61424, 0.04136, 0.04136);
  Color l(0.8, 0.8, 0.8);

  return k * l * to_source.dot(normal);
}

Color get_specular(const Coordinate &to_source, const Coordinate &normal,
                   const Coordinate &to_viewer) {
  Color k(0.727811, 0.626959, 0.626959);
  Color l(0.7, 0.7, 0.7);
  double alpha = 0.6;

  Coordinate r = -to_source - normal * 2.0 * ((-to_source).dot(normal));
  double dot = to_viewer.dot(r);
  if (dot <= 0) return 0;
  return k * l * std::pow(to_viewer.dot(r), alpha);
}

Color get_ambient() {
  Color k(0.1745, 0.01175, 0.01175);
  Color l(0.2, 0.2, 0.2);

  return k * l;
}