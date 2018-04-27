#ifndef MODEL_HPP
#define MODEL_HPP

#include <list>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "coordinate.hpp"
#include "texture.hpp"
#include "triangle.hpp"

enum class Axis { x, y };

class Model {
 public:
  std::vector<Coordinate> vertices;
  std::vector<Texture> textures;
  std::unordered_map<Coordinate *, Coordinate> normals;
  std::vector<Triangle> triangles;

  Model(const char *model_file);
  Model(const Model &other);

  bool find_closest_intersection(const Coordinate &eye,
                                 const Coordinate &direction, Triangle *closest,
                                 Coordinate *intersected_point,
                                 Coordinate *normal);
  bool is_shadowed(const Coordinate &eye, const Coordinate &direction,
                   const Triangle &surface);

 private:
  void read_triangle(std::istringstream &sin);
  void read_triangle_(std::list<Coordinate *> &vertices);
  void read_texture(std::istringstream &sin);
  void read_vertex(std::istringstream &sin);

  Coordinate get_avg();
  Coordinate get_max();
  Coordinate get_min();
  void normalize();
  void move(const Coordinate &to);
  void scale(double factor);
  void rotate(Axis axis, double degree);

  // Helpers
  bool is_ear(const std::list<Coordinate *> &vertices,
              std::list<Coordinate *>::iterator) const;
  void compute_normals();
};

#endif