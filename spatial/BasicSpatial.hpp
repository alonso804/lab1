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
 public:
  BasicSpatial(){};
  void insert(const Point& new_point) override {}

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point& reference) override {
    return Point({0, 0});
  }
};
}  // namespace spatial
}  // namespace utec