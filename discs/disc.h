//
//  disc.h
//  discs
//
//  Created by Piotr Brzeski on 2021-05-20.
//

#pragma once

#include "position.h"

class Disc {
public:
  Disc() = default;
  Disc(Position position, id_type  cluster);
  
  bool touches(Disc disc) const;
  bool touches_edge() const;
  
  Position const& get_position() const;
  id_type get_cluster() const;
  void set_cluster(id_type new_cluster);
  
private:
  Position position;
  id_type  cluster = invalid_id;
};
