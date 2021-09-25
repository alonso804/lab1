#pragma once

#include "SpatialBase.h"

namespace utec {
namespace spatial {

/**
 * RangeBST implementation
 */
template <typename Point>
class RangeBST : public SpatialBase<Point> {
 private:
 public:
  RangeBST(){};
  void insert(const Point& new_point) override {}

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point& reference) override { return Point({0}); }
  std::vector<Point> range(const Point& min, const Point& max) override {
    return {};
  };
};

}  // namespace spatial
}  // namespace utec
