// Point.h - FINAL CORRECTED VERSION
#ifndef PP_1_POINT_H
#define PP_1_POINT_H

#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

class Point {
private:
  std::array<double, 2> coordinates;
  int cluster{};

public:
  // Default constructor: creates a point at (0, 0). PERFECT for vector
  // initialization.
  Point() : coordinates{0.0, 0.0}, cluster(-1) {}

  // Constructor for specific coordinates.
  Point(double x, double y) : coordinates{x, y}, cluster(-1) {}

  // Constructor from a vector (for loading from CSV), with a runtime check.
  explicit Point(const std::vector<double> &coords) : cluster(-1) {
    if (coords.size() != 2) {
      throw std::invalid_argument(
          "Vector must contain 2 elements for a 2D Point.");
    }
    coordinates[0] = coords[0];
    coordinates[1] = coords[1];
  }

  // --- Performance-critical functions are inline ---

  inline double distance(const Point &other) const {
    const double dx = coordinates[0] - other.coordinates[0];
    const double dy = coordinates[1] - other.coordinates[1];
    return std::sqrt(dx * dx + dy * dy);
  }

  // ADDED BACK: l2normdiff is just an alias for distance.
  inline double l2normdiff(const Point &other) const { return distance(other); }

  inline const std::array<double, 2> &getCoordinates() const {
    return coordinates;
  }

  inline void setCluster(int c) { cluster = c; }
  inline int getCluster() const { return cluster; }

  // --- Operators ---
  inline Point &operator+=(const Point &rhs) {
    coordinates[0] += rhs.coordinates[0];
    coordinates[1] += rhs.coordinates[1];
    return *this;
  }

  inline void reset() { coordinates.fill(0.0); }
  inline Point operator/(int div) const {
    if (div == 0)
      return Point(0.0, 0.0);
    return Point(coordinates[0] / div, coordinates[1] / div);
  }

  // --- Logging functions ---

  void logPointCoordinates() const {
    std::cout << coordinates[0] << " " << coordinates[1];
  }

  void log_results() const {
    logPointCoordinates();
    std::cout << " Cluster: " << cluster << std::endl;
  }
};

#endif // PP_1_POINT_H
