#include <iostream>
#include <memory>
#include <vector>

#include "Validator.h"
#include <iostream>

namespace utec {
namespace spatial {

template <typename Point>
Validator<Point>::Validator() {}

template <typename Point>
void Validator<Point>::insert(const Point& new_point) {
  value boost_point(new_point.get(0), new_point.get(1));
  rtree.insert(boost_point);
}

template <typename Point>
Point Validator<Point>::nearest_neighbor(const Point& reference) {
  const int n = 1;
  std::vector<value> result_n;
  rtree.query(bgi::nearest(value(reference.get(0), reference.get(1)), n),
              std::back_inserter(result_n));
  return Point({result_n[0].get<0>(), result_n[0].get<1>()});
}

}  // namespace spatial
}  // namespace utec
