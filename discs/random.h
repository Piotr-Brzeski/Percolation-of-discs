//
//  random.hpp
//  discs
//
//  Created by Piotr Brzeski on 2021-05-20.
//

#pragma once

#include "types.h"
#include <random>

class Random {
public:
  Random(float_type min, float_type max);
  float_type operator()();
  
private:
  float_type                                 minimum;
  std::random_device                         device;
  std::mt19937                               generator;
  std::uniform_real_distribution<float_type> distribution;
};
