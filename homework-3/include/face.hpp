#ifndef FACE_HPP
#define FACE_HPP

#include <vector>
#include "coordinate.hpp"
#include "texture.hpp"

struct FaceElement {
  Coordinate *vertex;
  Texture *texture;
  Coordinate *normal;
};
typedef std::vector<FaceElement> Face;

#endif