//
//  position.h
//  discs
//
//  Created by Piotr Brzeski on 2021-05-20.
//

#pragma once

#include "types.h"

class Position {
public:
  Position() = default;
  Position(float_type x, float_type y);
  
  static Position random();
  
  float_type distance_squared_to(Position const& position) const;
  
  float_type radius_squared() const;
  float_type radius() const;
  float_type angle() const;
  
private:
  float_type x = 0.0;
  float_type y = 0.0;
};
