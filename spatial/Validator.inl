#include <iostream>
#include <memory>
#include <vector>

#include "Validator.h"

namespace utec {
namespace spatial {

template <typename Point>
Validator<Point>::Validator() {}

template <typename Point>
void Validator<Point>::insert(const Point& new_point) {
  value boost_point(new_point.get(0));
  rtree.insert(boost_point);
}

template <typename Point>
Point Validator<Point>::nearest_neighbor(const Point& reference) {
  return Point({0});
}

// template <typename Point>
// Point Validator<Point>::nearest_neighbor(const Point& reference) {
//   const int n = 1;
//   std::vector<value> result_n;
//   rtree.query(bgi::nearest(value(reference.get(0), reference.get(1)), n),
//               std::back_inserter(result_n));
//   return Point({result_n[0].get<0>(), result_n[0].get<1>()});
// }

template <typename Point>
std::vector<Point> Validator<Point>::range(const Point& min, const Point& max) {
  std::vector<value> result;

  box query(index_point(min.get(0)), index_point(max.get(0)));
  rtree.query(bgi::intersects(query), std::back_inserter(result));

  std::vector<Point> point_result;  //(result.size());
  for (auto& point : result) point_result.push_back(Point({point.get<0>()}));

  return point_result;
}

}  // namespace spatial
}  // namespace utec
