//
//  types.h
//  discs
//
//  Created by Piotr Brzeski on 2021-05-20.
//

#pragma once

#include <cstdint>
#include <limits>

using float_type = double;

using id_type = uint64_t;
constexpr auto invalid_id = std::numeric_limits<id_type>::max();
