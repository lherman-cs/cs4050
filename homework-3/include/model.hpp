#ifndef MODEL_HPP
#define MODEL_HPP

#include <sstream>
#include <vector>
#include "coordinate.hpp"
#include "face.hpp"
#include "texture.hpp"

class Model {
 public:
  std::vector<Coordinate> vertices;
  std::vector<Texture> textures;
  std::vector<Coordinate> normals;
  std::vector<Face> faces;

  Model(const char *model_file);
  Model(const Model &other);

 private:
  void read_face(std::istringstream &sin);
  void read_normal(std::istringstream &sin);
  void read_texture(std::istringstream &sin);
  void read_vertex(std::istringstream &sin);
};

#endif