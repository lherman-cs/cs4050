#ifndef UTILS_H
#define UTILS_H

#include <unordered_map>
#include <vector>

struct Vertex {
  double x, y, z;
};

struct Texture {
  double u, v;
};

struct FaceElement {
  // 0: Vertex Indices
  // 1: Vertex Texture Coordinate Indices
  // 2: Vertex Normal Indices
  Vertex *vertex;
  Texture *texture;
  Vertex *normal;
};
typedef std::vector<FaceElement> Face;

struct Model {
  std::vector<Vertex> vertices;
  std::vector<Texture> textures;
  std::vector<Vertex> normals;
  std::vector<Face> faces;
};

void read_model(const char *model_file, Model *model);
Vertex get_avg(std::vector<Vertex> &vertices);
Vertex get_max(std::vector<Vertex> &vertices);
Vertex get_min(std::vector<Vertex> &vertices);
void normalize(std::vector<Vertex> &vertices, double height, double width,
               double scale);
void move(std::vector<Vertex> &vertices, const Vertex &to);

// Filling polygons
struct EdgeInfo {
  int y_upper;
  double x_cur;
  double x_inc;
  double z_cur;
  double z_inc;
};

void build_edge_table(
    const std::vector<FaceElement> &face,
    std::unordered_map<int, std::vector<EdgeInfo>> &edge_table);
#endif