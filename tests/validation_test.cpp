#include <gtest/gtest.h>

#include <cstddef>
#include <iostream>
#include <iomanip>
#include <limits>
#include <time.h>

#include "Point.h"
#include "Validator.h"
#include "BasicSpatial.hpp"

using namespace utec::spatial;

template <typename T>
T genRandomNumber(T startRange, T endRange) {
  return startRange + (T)rand() / ((T)RAND_MAX / (T)(endRange - startRange));
}

TEST(SimpleTest, basicTest) {
  using data_t = int;
  using point_t = Point<data_t, 2>;

  Validator<point_t> validator;
  BasicSpatial<point_t> instancia;

  const std::size_t num_points = 10000;
  const std::size_t min = 0, max = 1000;

  auto cmp = [](point_t a, point_t b) {
    const int x = 0, y = 1;
    return (a.get(x) < b.get(x)) ||
           ((a.get(x) == b.get(x)) && (a.get(y) < b.get(y)));
  };

  std::set<point_t, decltype(cmp)> points(cmp);

  for (std::size_t I = 0; I < num_points; I++)
    points.insert(point_t(
        {genRandomNumber<int>(min, max), genRandomNumber<int>(min, max)}));

  for (auto& p : points) {
    validator.insert(p);
    instancia.insert(p);
  }
  clock_t referenceStart, referenceEnd;
  clock_t start, end;

  // solo hay un punto o sin puntos
  for (size_t i = 0; i < 1000; i++) {
    std::array<data_t, 2> x = {genRandomNumber<int>(min, max), genRandomNumber<int>(min, max)};

    referenceStart = clock();
    auto reference_result = validator.nearest_neighbor(point_t(x));
    referenceEnd = clock();
    //cout << reference_result.distance(x) << endl;
    //std::cout << "@@@@@@@@@@@" << std::endl;
    start = clock();
    auto result = instancia.nearest_neighbor(point_t(x));
    end = clock();
    //cout << result.distance(x) << endl;
    //cout << "===================\n";
    double referenceTaken = double(referenceEnd - referenceStart) / double(CLOCKS_PER_SEC);
    double taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "RTree: " << fixed << referenceTaken << setprecision(5) << endl;
    cout << "Me:    " << fixed << taken << setprecision(5) << endl;
    cout << referenceEnd << endl;
    cout << "================" << endl;
    
    EXPECT_EQ(reference_result.distance(x), result.distance(x));
  }
}

int main(int argc, char** argv) {
  srand((unsigned)time(0));

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
