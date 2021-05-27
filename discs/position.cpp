//
//  position.cpp
//  discs
//
//  Created by Piotr Brzeski on 2021-05-20.
//

#include "position.h"
#include "random.h"

namespace {

float_type max_radius = 0;
float_type max_radius_squared = 0;

} //  namespace

void Position::configure(float_type disc_radius) {
  max_radius = 1.0 + disc_radius;
  max_radius_squared = max_radius*max_radius;
}

Position::Position(float_type x, float_type y)
  : x(x)
  , y(y)
{
}

Position Position::random() {
  thread_local auto random_generator = Random(-max_radius, max_radius);
  while(true) {
    auto position = Position(random_generator(), random_generator());
    if(position.is_valid()) {
      return position;
    }
  }
}

bool Position::is_valid() const {
  return radius_squared() <= max_radius_squared;
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
  return two_pi + std::atan2(y, x);
}

float_type Position::distance_squared_to(Position const& position) const {
  auto distance_x = x - position.x;
  auto distance_y = y - position.y;
  return distance_x*distance_x + distance_y*distance_y;
}
