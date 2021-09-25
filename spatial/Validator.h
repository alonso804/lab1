#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>

#include "SpatialBase.h"

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

namespace utec {
namespace spatial {

typedef bg::model::point<double, 1, bg::cs::cartesian> index_point;
typedef index_point value;
typedef bg::model::box<index_point> box;

/**
 * Validator implementation
 */
template <typename Point>
class Validator : public SpatialBase<Point> {
 private:
  bgi::rtree<value, bgi::linear<16, 4>> rtree;

 public:
  Validator();
  void insert(const Point& new_point) override;
  Point nearest_neighbor(const Point& reference) override;
  std::vector<Point> range(const Point& min, const Point& max) override;
};

}  // namespace spatial
}  // namespace utec

#include "Validator.inl"
