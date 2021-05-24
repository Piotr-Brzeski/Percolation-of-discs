//
//  disc.cpp
//  discs
//
//  Created by Piotr Brzeski on 2021-05-23.
//

#include "disc.h"
#include "configuration.h"

Disc::Disc(Position position, id_type  cluster)
  : position(position)
  , cluster(cluster)
{
}

bool Disc::touches(Disc disc) const {
  static constexpr auto max_distance_squared = disc_radius*disc_radius;
  auto distance_squared = position.distance_squared_to(disc.position);
  return distance_squared < max_distance_squared;
}

bool Disc::touches_edge() const {
  static constexpr auto min_radius = 1.0 - disc_radius;
  static constexpr auto min_radius_squared = min_radius*min_radius;
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
