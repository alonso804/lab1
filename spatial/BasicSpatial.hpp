#pragma once

#include "SpatialBase.h"
#include <utility>
#include <vector>
#include <iostream>
#include <limits>
#include <unordered_map>

using namespace std;

namespace utec {
	namespace spatial {
		template <typename Point>
		class BasicSpatial : public SpatialBase<Point> {
		private:
			/*
			 *unordered_map<size_t, Point> points;
			 *size_t idxPoint;
			 */
			vector<Point> points;

		public:
			BasicSpatial() {
				/*
				 *this->idxPoint = 0;
				 */
			}

			void insert(const Point& new_point) override {
				/*
				 *this->points[idxPoint] = new_point;
				 *idxPoint++;
				 */
				this->points.push_back(new_point); // O(1)
			}

			Point nearest_neighbor(const Point& reference) override { 
				Point result = Point({0, 0});
				double distance = INT_MAX;

				for (const auto& point : this->points) { // O(n * D) => n = cantidad de puntos, D = dimensiones
					double newDistance = reference.distance(point);

					if (newDistance < distance) {
						result = point;
						distance = newDistance;
					}
				}

				return result;
/*
 *        pair<size_t, double> result = make_pair(0, this->points[0].distance(reference)); 
 *
 *        for (auto point : points) {
 *          auto distance = point.second.distance(reference);
 *          if (distance < result.second) {
 *            result = make_pair(point.first, distance); 
 *          }
 *        }
 *
 *        return this->points[result.first];
 */
			}
		};

	}
}
