#include "model.hpp"
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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

// Helper to read normal
void Model::read_normal(std::istringstream &sin) {
  Coordinate normal;
  sin >> normal.x >> normal.y >> normal.z;
  this->normals.push_back(normal);
}

// Helper to read triangle
void Model::read_triangle(std::istringstream &sin) {
  Triangle tri;
  std::string group;
  std::string el;

  int i = 0;
  while (sin >> group) {
    std::istringstream sin_el(group);

    std::getline(sin_el, el, '/');
    tri.vertices[i] = &this->vertices[std::stoi(el) - 1];
    i++;
  }
  this->triangles.push_back(tri);
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
    else if (type == "vn")
      this->read_texture(sin);
    else if (type == "f")
      this->read_triangle(sin);
    else
      continue;
  }

  fin.close();
}

Model::Model(const Model &other) {
  this->triangles = other.triangles;
  this->normals = other.normals;
  this->textures = other.textures;
  this->vertices = other.vertices;
}