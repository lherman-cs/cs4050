#include "triangle.hpp"
#include <limits>

double det(const Coordinate coords[3]) {
  return coords[0].x * (coords[1].y * coords[2].z - coords[1].z * coords[2].y) -
         coords[1].x * (coords[0].y * coords[2].z - coords[0].z * coords[2].y) +
         coords[2].x * (coords[0].y * coords[1].z - coords[0].z * coords[1].y);
}

Triangle::Triangle(Coordinate *a, Coordinate *b, Coordinate *c)
    : vertices{a, b, c} {}

Triangle::Triangle(Face &face)
    : vertices{face[0].vertex, face[1].vertex, face[2].vertex} {}

Triangle::Triangle() : vertices{NULL, NULL, NULL} {}

bool Triangle::operator==(const Triangle &other) const {
  return vertices[0] == other.vertices[0] && vertices[1] == other.vertices[1] &&
         vertices[2] == other.vertices[2];
}

bool Triangle::operator!=(const Triangle &other) const {
  return !(*this == other);
}

// Return t parameter if it intersects
// Else return infinity
double Triangle::intersects(const Coordinate &eye,
                            const Coordinate &direction) const {
  Coordinate &a = *this->vertices[0];
  Coordinate &b = *this->vertices[1];
  Coordinate &c = *this->vertices[2];

  Coordinate eb = b - a;
  Coordinate ec = c - a;

  Coordinate bary_t[3] = {-eb, -ec, a - eye};
  Coordinate bary_a[3] = {-eb, -ec, direction};
  Coordinate bary_b[3] = {a - eye, -ec, direction};
  Coordinate bary_c[3] = {-eb, a - eye, direction};

  double t = det(bary_t) / det(bary_a);
  double beta = det(bary_b) / det(bary_a);
  double gamma = det(bary_c) / det(bary_a);

  if (t > 0 && beta > 0 && gamma > 0 && beta + gamma < 1) return t;
  return NOT_INTERSECTED;
}

bool Triangle::is_empty() const {
  return vertices[0] == NULL && vertices[1] == NULL && vertices[2] == NULL;
}

// Get a normalized normal from a triangle
Coordinate Triangle::normal() const {
  Coordinate &p1 = *this->vertices[0];
  Coordinate &p2 = *this->vertices[1];
  Coordinate &p3 = *this->vertices[2];

  Coordinate u = p2 - p1;
  Coordinate v = p3 - p1;

  Coordinate n = u.cross(v);
  return n.normalize();
}