#include "color.hpp"
#include <math.h>
#include <utility>

Color::Color(double red, double green, double blue)
    : red(red), green(green), blue(blue) {}

Color::Color(double scalar) : red(scalar), green(scalar), blue(scalar) {}

Color::Color(const Color &other) {
  red = other.red;
  green = other.red;
  blue = other.blue;
}

Color::Color(Color &&other) {
  red = std::move(other.red);
  green = std::move(other.green);
  blue = std::move(other.blue);
}

Color &Color::operator=(const Color &other) {
  red = other.red;
  green = other.green;
  blue = other.blue;
  return *this;
}

Color &Color::operator=(Color &&other) {
  red = std::move(other.red);
  green = std::move(other.green);
  blue = std::move(other.blue);
  return *this;
}

Color Color::operator+(const Color &other) {
  return {red + other.red, green + other.green, blue + other.blue};
}

Color Color::operator+(const double scalar) {
  return {red + scalar, green + scalar, blue + scalar};
}

Color Color::operator-(const Color &other) {
  return {red - other.red, green - other.green, blue - other.blue};
}

Color Color::operator-() { return {-red, -green, -blue}; }

Color Color::operator-(const double scalar) {
  return {red - scalar, green - scalar, blue - scalar};
}

Color Color::operator*(const Color &other) {
  return {red * other.red, green * other.green, blue * other.blue};
}

Color Color::operator*(const double scalar) {
  return {red * scalar, green * scalar, blue * scalar};
}

Color Color::operator/(const Color &other) {
  return {red / other.red, green / other.green, blue / other.blue};
}

Color Color::operator/(const double scalar) {
  return {red / scalar, green / scalar, blue / scalar};
}

double Color::dot(const Color &other) {
  return red * other.red + green * other.green + blue * other.blue;
}

Color Color::cross(const Color &other) {
  return Color(green * other.blue - blue * other.green,
               blue * other.red - red * other.blue,
               red * other.green - green * other.red);
}

Color Color::normalize() {
  double distance = std::pow(red, 2) + std::pow(green, 2) + std::pow(blue, 2);
  distance = std::sqrt(distance);

  return *this / distance;
}