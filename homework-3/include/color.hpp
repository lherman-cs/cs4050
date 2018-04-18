#ifndef COLOR_HPP
#define COLOR_HPP

class Color {
 public:
  double red, green, blue;
  Color(double red, double green, double blue);
  Color(double scalar);
  Color(const Color &other);
  Color(Color &&other);
  Color &operator=(const Color &other);
  Color &operator=(Color &&other);
  Color operator+(const Color &other);
  Color operator+(const double scalar);
  Color operator-(const Color &other);
  Color operator-();
  Color operator-(const double scalar);
  Color operator*(const Color &other);
  Color operator*(const double scalar);
  Color operator/(const Color &other);
  Color operator/(const double scalar);

  double dot(const Color &other);
  Color cross(const Color &other);
  Color normalize();
};

#endif