#include "utils.hpp"
#include <math.h>
#include <algorithm>
#include <iostream>
#include <limits>

// Get the average of vertices and return a vertex
// containing the average for every axis
Coordinate get_avg(std::vector<Coordinate> &vertices) {
  Coordinate v;
  double n = 0;
  for (const auto &vertex : vertices) {
    n++;
    v.x += (vertex.x - v.x) / n;
    v.y += (vertex.y - v.y) / n;
    v.z += (vertex.z - v.z) / n;
  }
  return v;
}

// Get the maximum of vertices and return a vertex
// containing the maximum for every axis
Coordinate get_max(std::vector<Coordinate> &vertices) {
  double min = std::numeric_limits<double>::min();
  Coordinate v = {min, min, min};
  for (const auto &vertex : vertices) {
    v.x = vertex.x > v.x ? vertex.x : v.x;
    v.y = vertex.y > v.y ? vertex.y : v.y;
    v.z = vertex.z > v.z ? vertex.z : v.z;
  }
  return v;
}

// Get the minimum of vertices and return a vertex
// containing the minimum for every axis
Coordinate get_min(std::vector<Coordinate> &vertices) {
  double max = std::numeric_limits<double>::max();
  Coordinate v = {max, max, max};
  for (const auto &vertex : vertices) {
    v.x = vertex.x < v.x ? vertex.x : v.x;
    v.y = vertex.y < v.y ? vertex.y : v.y;
    v.z = vertex.z < v.z ? vertex.z : v.z;
  }
  return v;
}

// Move all polygons to the origin and scale them so that
// they fit into the screen. Then, move the polygons
// to the middle of the screen
void normalize(std::vector<Coordinate> &vertices, double height, double width,
               double scale) {
  int center_x = ((int)width) >> 1;
  int center_y = ((int)height) >> 1;

  // Move to origin
  move(vertices, {0, 0, 0});

  // Normalize
  Coordinate max = get_max(vertices), min = get_min(vertices);
  scale *= std::min(width, height);
  auto normalize_fn = [&max, &min, scale](Coordinate &v) {
    v = (v - min) / (max - min) * scale;
  };
  std::for_each(vertices.begin(), vertices.end(), normalize_fn);

  // Move to center
  Coordinate avg = get_avg(vertices);
  move(vertices, {(double)center_x, (double)center_y, avg.z});
}

// Move to the given vertex as a center
void move(std::vector<Coordinate> &vertices, const Coordinate &to) {
  Coordinate avg = get_avg(vertices);

  auto move_fn = [&avg, &to](Coordinate &v) {
    v.x = v.x - avg.x + to.x;
    v.y = v.y - avg.y + to.y;
    v.z = v.z - avg.z + to.z;
  };
  std::for_each(vertices.begin(), vertices.end(), move_fn);
}

// Scale all vertices by given factor
void scale(std::vector<Coordinate> &vertices, double factor) {
  Coordinate avg = get_avg(vertices);

  auto scale_fn = [&avg, factor](Coordinate &v) {
    v.x = (v.x - avg.x) * factor + avg.x;
    v.y = (v.y - avg.y) * factor + avg.y;
    v.z = (v.z - avg.z) * factor + avg.z;
  };
  std::for_each(vertices.begin(), vertices.end(), scale_fn);
}

void rotate(std::vector<Coordinate> &vertices, Axis axis, double degree) {
  Coordinate avg = get_avg(vertices);

  auto rotate_x_fn = [degree](Coordinate &v) {
    const double y = v.y, z = v.z;
    v.y = y * std::cos(degree) - z * std::sin(degree);
    v.z = y * std::sin(degree) + z * std::cos(degree);
  };
  auto rotate_y_fn = [degree](Coordinate &v) {
    const double x = v.x, z = v.z;
    v.x = x * std::cos(degree) + z * std::sin(degree);
    v.z = -x * std::sin(degree) + z * std::cos(degree);
  };

  // Move all vertices to the origin first
  move(vertices, {0, 0, 0});

  switch (axis) {
    case Axis::x:
      std::for_each(vertices.begin(), vertices.end(), rotate_x_fn);
      break;
    case Axis::y:
      std::for_each(vertices.begin(), vertices.end(), rotate_y_fn);
      break;
  }

  // Move all vertices back to where they were
  move(vertices, avg);
}

// Find the closest intersection based on the z axis.
// The more positive, the closer the triangle is.
// Parameters:
//  faces: a list of triangles
//  eye: a coordinate where the eye is
//  direction: a unit vector from the eye to a point
//
// Returns:
//  closests: in place return the closest triangle
//  intersected_point: in place return the closest intersected point
//  a bool if there is an intersection or not
bool find_closest_intersection(const std::vector<Face> &faces,
                               const Coordinate &eye,
                               const Coordinate &direction, Triangle *closest,
                               Coordinate *intersected_point) {
  double closest_z = Triangle::NOT_INTERSECTED;
  bool intersected = false;
  for (Face face : faces) {
    Triangle tri(face);
    double t = tri.intersects(eye, direction);

    if (t != Triangle::NOT_INTERSECTED) {
      Coordinate p = Coordinate(eye + direction * t);
      if (p.z > closest_z) {
        closest_z = p.z;
        *closest = tri;
        *intersected_point = p;
        intersected = true;
      }
    }
  }
  return intersected;
}

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
  Color l(0.6, 0.6, 0.6);
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