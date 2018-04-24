#include "model.hpp"
#include <float.h>
#include <limits.h>
#include <string.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "utils.hpp"

// Helper to read vertex
void Model::read_vertex(std::istringstream &sin) {
  Coordinate vertex;
  sin >> vertex.x >> vertex.y >> vertex.z;
  this->vertices.push_back(vertex);
}

// Helper to read texture
void Model::read_texture(std::istringstream &sin) {
  Texture texture;
  sin >> texture.u >> texture.v;
  this->textures.push_back(texture);
}

// Helper to read triangle
void Model::read_triangle(std::istringstream &sin) {
  Triangle tri;
  std::string group;
  std::string el;

  std::list<Coordinate *> vertices;

  while (sin >> group) {
    std::istringstream sin_el(group);

    std::getline(sin_el, el, '/');
    vertices.push_back(&this->vertices[std::stoi(el) - 1]);
  }

  read_triangle_(vertices);
}

// read_triangle_ is a helper for read_triangle which helps
// in doing triangle remesh if the number of polygon's sides
// is more than 3. This helper will also store all the triangles
// into this.vertices.
//
// Parameters:
//  vertices are polygon's vertices
void Model::read_triangle_(std::list<Coordinate *> &vertices) {
  auto it = vertices.begin();
  while (vertices.size() > 3) {
    if (is_ear(vertices, it)) {
      auto b_it = it, c_it = it;
      b_it--;
      c_it++;
      if (b_it == vertices.end()) b_it--;
      if (c_it == vertices.end()) c_it++;

      this->triangles.push_back(Triangle(*b_it, *it, *c_it));
      it = vertices.erase(it);
      if (it == vertices.end()) it++;
    } else
      it++;
  }

  it = vertices.begin();
  auto b_it = it, c_it = it;
  b_it++;
  c_it++;
  c_it++;
  this->triangles.push_back(Triangle(*it, *b_it, *c_it));
}

// Read model_file and parse it
Model::Model(const char *model_file) {
  std::ifstream fin(model_file);
  std::string line;
  std::string type;

  while (getline(fin, line)) {
    std::istringstream sin(line);
    sin >> type;

    if (type == "v")
      this->read_vertex(sin);
    else if (type == "vt")
      this->read_texture(sin);
    else if (type == "f")
      this->read_triangle(sin);
    else
      continue;
  }

  fin.close();

  normalize();
  compute_normals();
}

Model::Model(const Model &other) {
  this->triangles = other.triangles;
  this->normals = other.normals;
  this->textures = other.textures;
  this->vertices = other.vertices;
}

// Get the average of vertices and return a vertex
// containing the average for every axis
Coordinate Model::get_avg() {
  Coordinate v;
  double n = 0;
  for (const auto &vertex : vertices) {
    n++;
    v = v + (vertex - v) / n;
  }
  return v;
}

// Get the maximum of vertices and return a vertex
// containing the maximum for every axis
Coordinate Model::get_max() {
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
Coordinate Model::get_min() {
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
void Model::normalize() {
  move(Coordinate(0.0));
  Coordinate max = get_max(), min = get_min();
  double scl_max = std::max(std::max(max.x, max.y), max.z);
  double scl_min = std::min(std::min(min.x, min.y), min.z);
  double scl = (scl_max - scl_min) / 2.0;

  for (auto &v : vertices) v = ((v - scl_min) / scl - 1.0) * 0.95;
}

// Move to the given vertex as a center
void Model::move(const Coordinate &to) {
  Coordinate avg = get_avg();
  for (auto &v : vertices) v = v - avg + to;
}

// Scale all vertices by given factor
void Model::scale(double factor) {
  Coordinate avg = get_avg();
  for (auto &v : vertices) v = (v - avg) * factor + avg;
}

void Model::rotate(Axis axis, double degree) {
  Coordinate avg = get_avg();

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
  move({0, 0, 0});

  switch (axis) {
    case Axis::x:
      std::for_each(vertices.begin(), vertices.end(), rotate_x_fn);
      break;
    case Axis::y:
      std::for_each(vertices.begin(), vertices.end(), rotate_y_fn);
      break;
  }

  // Move all vertices back to where they were
  move(avg);
}

// is_ear is to check if a triangle that is made of it - 1, it, it + 1
// is an ear or not.
//
// Parameters:
//  vertices are a list of vertices in the polygon. NOT MODEL'S VERTICES.
//  it is the current vertex iterator in the polygon
//
// Returns:
//  true if it's an ear, else false
bool Model::is_ear(const std::list<Coordinate *> &vertices,
                   std::list<Coordinate *>::iterator it) const {
  Coordinate &a = **it;
  auto b_it = it;
  auto c_it = it;

  b_it--;
  if (b_it == vertices.end()) b_it--;
  Coordinate &b = **b_it;

  c_it++;
  if (c_it == vertices.end()) c_it++;
  Coordinate &c = **c_it;

  Triangle tri = Triangle(&a, &b, &c);
  auto tmp = it;

  // Check to the left
  for (--it; it != vertices.end(); it--)
    if (tri.is_in(**it)) return false;

  it = tmp;
  // Check to the right
  for (++it; it != vertices.end(); it++)
    if (tri.is_in(**it)) return false;

  return true;
}

void Model::compute_normals() {
  std::unordered_map<Coordinate *, std::unordered_set<Triangle *>> m;

  for (Triangle &tri : triangles) {
    m[tri.vertices[0]].insert(&tri);
    m[tri.vertices[1]].insert(&tri);
    m[tri.vertices[2]].insert(&tri);
  }

  for (Coordinate &v : vertices) {
    Coordinate dividend = Coordinate(0.0);
    double divisor = 0.0;
    for (auto it = m[&v].begin(); it != m[&v].end(); it++) {
      Coordinate normal = (*it)->normal();
      dividend = dividend + normal;
      divisor += normal.dist();
    }
    normals[&v] = (dividend / divisor).normalize();
  }
}