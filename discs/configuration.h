//
//  configuration.h
//  discs
//
//  Created by Piotr Brzeski on 2021-05-21.
//

#pragma once

#include "types.h"

// TODO: Make configurable
constexpr float_type disc_radius = 0.01;
constexpr float_type disc_diameter = 2*disc_radius;

static_assert(disc_radius > min_radius);
