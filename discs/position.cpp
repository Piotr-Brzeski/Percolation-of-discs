//
//  position.cpp
//  discs
//
//  Created by Piotr Brzeski on 2021-05-20.
//

#include "configuration.h"
#include "position.h"
#include "random.h"
#include <cmath>

Position::Position(float_type x, float_type y)
  : x(x)
  , y(y)
{
}

Position Position::random() {
  static constexpr float_type max_radius = 1.0 + disc_radius;
  static constexpr float_type max_radius_squared = max_radius*max_radius;
  thread_local auto random_generator = Random(-max_radius, max_radius);
  while(true) {
    auto position = Position(random_generator(), random_generator());
    if(position.radius_squared() <= max_radius_squared) {
      return position;
    }
  }
}

float_type Position::radius_squared() const {
  return x*x + y*y;
}

float_type Position::radius() const {
  return std::sqrt(radius_squared());
}

/// Angle between x-axis and vector (x, y)
float_type Position::angle() const {
  if(y >= 0.0) {
    return std::atan2(y, x);
  }
  return 2*M_PI + std::atan2(y, x);
}

float_type Position::distance_squared_to(Position const& position) const {
  auto distance_x = x - position.x;
  auto distance_y = y - position.y;
  return distance_x*distance_x + distance_y*distance_y;
}
