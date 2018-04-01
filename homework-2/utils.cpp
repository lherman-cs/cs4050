#include "utils.hpp"
#include <math.h>
#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

// Helper to read vertex
void read_vertex(std::istringstream &sin, Model *model) {
  Vertex vertex;
  sin >> vertex.x >> vertex.y >> vertex.z;
  model->vertices.push_back(vertex);
}

// Helper to read texture
void read_texture(std::istringstream &sin, Model *model) {
  Texture texture;
  sin >> texture.u >> texture.v;
  model->textures.push_back(texture);
}

// Helper to read normal
void read_normal(std::istringstream &sin, Model *model) {
  Vertex normal;
  sin >> normal.x >> normal.y >> normal.z;
  model->normals.push_back(normal);
}

// Helper to read face
void read_face(std::istringstream &sin, Model *model) {
  Face face;
  std::string group;
  std::string el;

  while (sin >> group) {
    std::istringstream sin_el(group);
    int i = 0;
    FaceElement face_el;
    memset(&face_el, 0, sizeof(face_el));

    while (std::getline(sin_el, el, '/')) {
      switch (i) {
        case 0:  // Vertex
          face_el.vertex = &model->vertices[std::stoi(el) - 1];
          break;
        case 1:  // Texture
          face_el.texture = &model->textures[std::stoi(el) - 1];
          break;
        case 2:  // Normal
          face_el.normal = &model->normals[std::stoi(el) - 1];
          break;
      }
      i++;
    }
    face.push_back(face_el);
  }
  model->faces.push_back(face);
}

// Read model_file, parse it, and store it in model
void read_model(const char *model_file, Model *model) {
  std::ifstream fin(model_file);
  std::string line;
  std::string type;

  while (getline(fin, line)) {
    std::istringstream sin(line);
    sin >> type;

    if (type == "v")
      read_vertex(sin, model);
    else if (type == "vt")
      read_texture(sin, model);
    else if (type == "vn")
      read_texture(sin, model);
    else if (type == "f")
      read_face(sin, model);
    else
      continue;
  }

  fin.close();
}

// Get the average of vertices and return a vertex
// containing the average for every axis
Vertex get_avg(std::vector<Vertex> &vertices) {
  Vertex v;
  for (const auto &vertex : vertices) {
    v.x += vertex.x;
    v.y += vertex.y;
    v.z += vertex.z;
  }

  v.x /= vertices.size();
  v.y /= vertices.size();
  v.z /= vertices.size();

  return v;
}

// Get the maximum of vertices and return a vertex
// containing the maximum for every axis
Vertex get_max(std::vector<Vertex> &vertices) {
  double min = std::numeric_limits<double>::min();
  Vertex v = {min, min, min};
  for (const auto &vertex : vertices) {
    v.x = vertex.x > v.x ? vertex.x : v.x;
    v.y = vertex.y > v.y ? vertex.y : v.y;
    v.z = vertex.z > v.z ? vertex.z : v.z;
  }
  return v;
}

// Get the minimum of vertices and return a vertex
// containing the minimum for every axis
Vertex get_min(std::vector<Vertex> &vertices) {
  double max = std::numeric_limits<double>::max();
  Vertex v = {max, max, max};
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
void normalize(std::vector<Vertex> &vertices, double height, double width,
               double scale) {
  int center_x = ((int)width) >> 1;
  int center_y = ((int)height) >> 1;

  // Move to origin
  move(vertices, {0, 0, 0});

  // Normalize
  Vertex max = get_max(vertices), min = get_min(vertices);
  scale *= std::min(width / (max.x - min.x), height / (max.y - min.y));
  auto normalize_fn = [&max, &min, scale](Vertex &v) {
    v.x *= scale;
    v.y *= scale;
    // normalize to be between [0, 1]
    v.z = (v.z - min.z) / (max.z - min.z);
  };
  std::for_each(vertices.begin(), vertices.end(), normalize_fn);

  // Move to center
  Vertex avg = get_avg(vertices);
  move(vertices, {(double)center_x, (double)center_y, avg.z});
}

// Move to the given vertex as a center
void move(std::vector<Vertex> &vertices, const Vertex &to) {
  Vertex avg = get_avg(vertices);

  auto move_fn = [&avg, &to](Vertex &v) {
    v.x = v.x - avg.x + to.x;
    v.y = v.y - avg.y + to.y;
    v.z = v.z - avg.z + to.z;
  };
  std::for_each(vertices.begin(), vertices.end(), move_fn);
}

/**
 * Clear edge_table and construct a new one using the given face
 */
void build_edge_table(
    const std::vector<FaceElement> &face,
    std::unordered_map<int, std::vector<EdgeInfo>> &edge_table) {
  edge_table.clear();

  for (unsigned i = 0; i < face.size(); i++) {
    // Get neighbors
    const Vertex &left =
        i == 0 ? *face[face.size() - 1].vertex : *face[i - 1].vertex;
    const Vertex &middle = *face[i].vertex;
    const Vertex &right =
        i == face.size() - 1 ? *face[0].vertex : *face[i + 1].vertex;

    EdgeInfo info;
    int cur_y = (int)round(middle.y);
    info.x_cur = middle.x;
    info.z_cur = middle.z;

    // If the neighbor points have bigger y, push the points to the edge_table
    if (left.y > middle.y) {
      info.y_upper = (int)round(left.y);
      info.x_inc = (left.x - middle.x) / (left.y - middle.y);
      info.z_inc = (left.z - middle.z) / (left.y - middle.y);
      edge_table[cur_y].push_back(info);
    }
    if (right.y > middle.y) {
      info.y_upper = (int)round(right.y);
      info.x_inc = (right.x - middle.x) / (right.y - middle.y);
      info.z_inc = (right.z - middle.z) / (right.y - middle.y);
      edge_table[cur_y].push_back(info);
    }
  }
}