//
//  bins.cpp
//  discs
//
//  Created by Piotr Brzeski on 2021-05-24.
//

#include "bins.h"
#include <limits>
#include <iostream>

namespace {

float_type disc_diameter = 0;

int coordinate(float_type position) {
  if(position < 0) {
    return static_cast<int>(position/disc_diameter) - 1;
  }
  return static_cast<int>(position/disc_diameter);
}

void append(key_type const& key, std::map<key_type, bin_type> const& bins, std::vector<std::reference_wrapper<bin_type const>>& neighbours) {
  if(auto it = bins.find(key); it != bins.end()) {
    neighbours.emplace_back(it->second);
  }
}

} //  namespace

void Bins::configure(float_type disc_radius) {
  disc_diameter = 2*disc_radius;
}

bin_type& Bins::operator[](Position const& position) {
  auto key = key_type(coordinate(position.x), coordinate(position.y));
  auto& bin = bins[key];
  return bin;
}

std::vector<std::reference_wrapper<bin_type const>> Bins::neighbours(Position const& position) const {
  auto neighbours = std::vector<std::reference_wrapper<bin_type const>>();
  auto key = key_type(coordinate(position.x), coordinate(position.y));
  for(auto x : {-1, 0, 1}) {
    for(auto y : {-1, 0, 1}) {
      append(key_type(key.first + x, key.second + y), bins, neighbours);
    }
  }
  return neighbours;
}

void Bins::print_stats() const {
  auto max_coordinate = 2*coordinate(1 + disc_diameter/2);
  std::cout << "Max number of bins: " << max_coordinate*max_coordinate << std::endl;
  std::cout << "Number of bins: " << bins.size() << std::endl;
  auto min = std::numeric_limits<std::size_t>::max();
  auto max = std::numeric_limits<std::size_t>::min();
  for(auto& bin : bins) {
    auto size = bin.second.size();
    min = std::min(min, size);
    max = std::max(max, size);
  }
  std::cout << "Min number of discs: " << min << std::endl;
  std::cout << "Max number of discs: " << max << std::endl;
}
