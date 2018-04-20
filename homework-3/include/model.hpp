#ifndef MODEL_HPP
#define MODEL_HPP

#include <list>
#include <sstream>
#include <vector>
#include "coordinate.hpp"
#include "texture.hpp"
#include "triangle.hpp"

class Model {
 public:
  std::vector<Coordinate> vertices;
  std::vector<Texture> textures;
  std::vector<Coordinate> normals;
  std::vector<Triangle> triangles;

  Model(const char *model_file);
  Model(const Model &other);

 private:
  void read_triangle(std::istringstream &sin);
  void read_normal(std::istringstream &sin);
  void read_texture(std::istringstream &sin);
  void read_vertex(std::istringstream &sin);

  // Helpers
  bool is_ear(const std::list<Coordinate *> &vertices,
              std::list<Coordinate *>::iterator) const;
};

#endif