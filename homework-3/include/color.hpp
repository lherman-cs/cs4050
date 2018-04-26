#ifndef COLOR_HPP
#define COLOR_HPP

#include <ostream>

class Color {
 public:
  double red, green, blue;
  Color(double red, double green, double blue);
  Color(double scalar);
  Color(const Color &other);
  Color(Color &&other);
  Color();
  Color &operator=(const Color &other);
  Color &operator=(Color &&other);
  Color operator+(const Color &other) const;
  Color operator+(const double scalar) const;
  Color operator-(const Color &other) const;
  Color operator-() const;
  Color operator-(const double scalar) const;
  Color operator*(const Color &other) const;
  Color operator*(const double scalar) const;
  Color operator/(const Color &other) const;
  Color operator/(const double scalar) const;
  friend std::ostream &operator<<(std::ostream &stream, const Color &c);

  double dot(const Color &other) const;
  Color cross(const Color &other) const;
  Color normalize() const;
};

#endif