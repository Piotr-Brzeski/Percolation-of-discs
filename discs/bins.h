//
//  bins.h
//  discs
//
//  Created by Piotr Brzeski on 2021-05-24.
//

#pragma once

#include "position.h"
#include <map>
#include <vector>
#include <functional>

using bin_type = std::vector<id_type>;
using key_type = std::pair<bin_coordinate_type, bin_coordinate_type>;

class Bins {
public:
  static void configure(float_type disc_radius);
  
  bin_type& operator[](Position const& position);
  std::vector<std::reference_wrapper<bin_type const>> neighbours(Position const& position) const;
  
private:
  std::map<key_type, bin_type> bins;
};
