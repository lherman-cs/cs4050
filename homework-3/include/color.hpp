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
  Color operator+(const Color &other) const;
  Color operator+(const double scalar) const;
  Color operator-(const Color &other) const;
  Color operator-() const;
  Color operator-(const double scalar) const;
  Color operator*(const Color &other) const;
  Color operator*(const double scalar) const;
  Color operator/(const Color &other) const;
  Color operator/(const double scalar) const;

  double dot(const Color &other) const;
  Color cross(const Color &other) const;
  Color normalize() const;
};

#endif