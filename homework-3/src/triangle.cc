#include "triangle.hpp"
#include <limits>

double det(const Coordinate coords[3]) {
  return coords[0].x * (coords[1].y * coords[2].z - coords[1].z * coords[2].y) -
         coords[1].x * (coords[0].y * coords[2].z - coords[0].z * coords[2].y) +
         coords[2].x * (coords[0].y * coords[1].z - coords[0].z * coords[1].y);
}

Triangle::Triangle(Coordinate *a, Coordinate *b, Coordinate *c)
    : vertices{a, b, c} {}

Triangle::Triangle() : vertices{nullptr, nullptr, nullptr} {}

bool Triangle::operator==(const Triangle &other) const {
  return vertices[0] == other.vertices[0] && vertices[1] == other.vertices[1] &&
         vertices[2] == other.vertices[2];
}

bool Triangle::operator!=(const Triangle &other) const {
  return !(*this == other);
}

std::ostream &operator<<(std::ostream &stream, const Triangle &tri) {
  stream << *tri.vertices[0] << '\n'
         << *tri.vertices[1] << '\n'
         << *tri.vertices[2] << '\n'
         << "=================================================";
  return stream;
}

// Return t parameter if it intersects
// Else return infinity
bool Triangle::intersects(const Coordinate &eye, const Coordinate &direction,
                          double *t) const {
  Coordinate &a = *this->vertices[0];
  Coordinate &b = *this->vertices[1];
  Coordinate &c = *this->vertices[2];

  Coordinate eb = b - a;
  Coordinate ec = c - a;

  Coordinate bary_t[3] = {-eb, -ec, a - eye};
  Coordinate bary_a[3] = {-eb, -ec, direction};
  Coordinate bary_b[3] = {a - eye, -ec, direction};
  Coordinate bary_c[3] = {-eb, a - eye, direction};

  *t = det(bary_t) / det(bary_a);
  double beta = det(bary_b) / det(bary_a);
  double gamma = det(bary_c) / det(bary_a);

  if (*t > 0 && beta >= 0 && gamma >= 0 && beta + gamma <= 1) return true;
  return false;
}

// is_in determines if p is in the triangle
// Parameters:
//  p is a coordinate to be tested
// Returns:
//  true if p is in the triangle, else false
//
// Equation (see slide ch. 15 on page 19):
//  p = a + beta(b - a) + gamma(c - a)
bool Triangle::is_in(const Coordinate &p) const {
  Coordinate &a = *this->vertices[0];
  Coordinate &b = *this->vertices[1];
  Coordinate &c = *this->vertices[2];

  Coordinate eb = b - a;
  Coordinate ec = c - a;

  double gamma = eb.x * (p.y - a.y) - eb.y * (p.x - a.x);
  gamma /= (ec.y * eb.x - eb.y * ec.x);

  double beta = (p.x - a.x - gamma * ec.x) / eb.x;

  return beta > 0 && gamma > 0 && beta + gamma < 1;
}

bool Triangle::is_empty() const {
  return vertices[0] == nullptr && vertices[1] == nullptr &&
         vertices[2] == nullptr;
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