//
//  disc.cpp
//  discs
//
//  Created by Piotr Brzeski on 2021-05-23.
//

#include "disc.h"

namespace {

float_type max_distance_squared = 0;
float_type min_radius_squared = 0;

}

void Disc::configure(float_type disc_radius) {
  auto disc_diameter = 2*disc_radius;
  max_distance_squared = disc_diameter*disc_diameter;
  
  auto min_radius = 1 - disc_radius;
  min_radius_squared = min_radius*min_radius;
}

Disc::Disc(Position position, id_type  cluster)
  : position(position)
  , cluster(cluster)
{
}

bool Disc::touches(Disc disc) const {
  auto distance_squared = position.distance_squared_to(disc.position);
  return distance_squared < max_distance_squared;
}

bool Disc::touches_edge() const {
  return position.radius_squared() >= min_radius_squared;
}

Position const& Disc::get_position() const {
  return position;
}

id_type Disc::get_cluster() const {
  return cluster;
}

void Disc::set_cluster(id_type new_cluster)
{
  cluster = new_cluster;
}
