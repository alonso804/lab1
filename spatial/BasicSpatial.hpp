#pragma once

#include "SpatialBase.h"

namespace utec {
namespace spatial {

/**
 * BasicSpatial implementation
 */
template <typename Point>
class BasicSpatial : public SpatialBase<Point> {
 private:
  std::vector<Point> points;

 public:
  BasicSpatial(){};
  void insert(const Point& new_point) override { points.push_back(new_point); }

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point& reference) override { return Point({0}); }

  std::vector<Point> range(const Point& min, const Point& max) override {
    std::vector<Point> result;

    for (const auto& point : points) {
      if (point.get(0) <= max.get(0) && min.get(0) <= point.get(0))
        result.push_back(point);
    }

    return result;
  }
};

}  // namespace spatial
}  // namespace utec
