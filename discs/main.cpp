//
//  main.cpp
//  discs
//
//  Created by Piotr Brzeski on 2021-05-20.
//

#include "system.h"
#include <iostream>

int main(int argc, const char * argv[]) {
  try {
    auto system = System(120);
    while(!system.is_done()) {
      system.add_disc();
    }
    auto& percolations = system.get_percolations();
    for(auto percolation : percolations) {
      std::cout << percolation << "\n";
    }
    std::cout << std::endl;
    std::cout << "n = " << system.number_of_discs() << std::endl;
    std::cout << "c = " << system.number_of_edge_clusters() << std::endl;
    return 0;
  }
  catch(std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  catch(...) {
    std::cerr << "Unknown error." << std::endl;
  }
  return 1;
}
