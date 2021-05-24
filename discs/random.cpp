//
//  random.cpp
//  discs
//
//  Created by Piotr Brzeski on 2021-05-20.
//

#include "random.h"

Random::Random(float_type min, float_type max)
  : minimum(min)
  , generator(device())
  , distribution(min, max)
{
}

float_type Random::operator()() {
  while(true) {
    auto result = distribution(generator);
    if(result != minimum) {
      return result;
    }
  }
}
