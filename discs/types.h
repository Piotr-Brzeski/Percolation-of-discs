//
//  types.h
//  discs
//
//  Created by Piotr Brzeski on 2021-05-20.
//

#pragma once

#include <cstdint>
#include <limits>
#include <cmath>

using float_type = double;
constexpr auto two_pi = static_cast<float_type>(2*M_PI);

using id_type = uint64_t;
constexpr auto invalid_id = std::numeric_limits<id_type>::max();

using bin_coordinate_type = int16_t;

constexpr auto max_coordinate = static_cast<float_type>(std::numeric_limits<bin_coordinate_type>::max());
constexpr auto min_radius = 1 / (2*max_coordinate - 1);
