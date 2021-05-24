//
//  system.h
//  discs
//
//  Created by Piotr Brzeski on 2021-05-20.
//

#pragma once

#include "disc.h"
#include <vector>
#include <map>

class System {
public:
  void add_disc();
  bool is_done() const;
  
  std::size_t number_of_discs() const;
  std::size_t number_of_edge_clusters() const;
  
private:
  std::vector<Disc>                          discs;
  std::map<id_type, std::vector<float_type>> edge_clusters; // cluster -> sorted vector of angles
};
