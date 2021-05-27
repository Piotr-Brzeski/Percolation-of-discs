//
//  main.cpp
//  discs
//
//  Created by Piotr Brzeski on 2021-05-20.
//

#include "system.h"
#include <iostream>
#include <string>
#include <vector>

namespace {

class Arguments {
public:
  Arguments(int argc, const char ** argv) {
    for(auto i = 1; i <argc; ++i) {
      arguments.emplace_back(argv[i]);
    }
  }
  
  std::string const& next() {
    if(index < arguments.size()) {
      return arguments[index++];
    }
    throw std::runtime_error("Invalid number of arguments.");
  }
  
private:
  std::vector<std::string> arguments;
  std::size_t              index = 0;
};

void print_usage(const char* name) {
  std::cout << "Usage:" << std::endl;
  std::cout << name << " -radius disc_radius -orientations number_of_orientations" << std::endl;
  std::cout << name << " -help" << std::endl;
}

} // namespace

int main(int argc, const char * argv[]) {
  try {
    auto arguments = Arguments(argc, argv);
    float_type disc_radius = 0;
    std::size_t number_of_orientations = 0;
    while(disc_radius == 0 || number_of_orientations == 0) {
      auto& name = arguments.next();
      if(name == "-radius") {
        disc_radius = std::stod(arguments.next());
      }
      else if(name == "-orientations") {
        number_of_orientations = std::stoull(arguments.next());
      }
      else if(name == "-help") {
        print_usage(argv[0]);
        return 0;
      }
      else {
        throw std::runtime_error("Invalid argument.");
      }
    }
    auto system = System(disc_radius, number_of_orientations);
    while(!system.is_done()) {
      system.add_disc();
    }
    auto& percolations = system.get_percolations();
    for(auto percolation : percolations) {
      std::cout << percolation << "\n";
    }
    return 0;
  }
  catch(std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  catch(...) {
    std::cerr << "Unknown error." << std::endl;
  }
  print_usage(argv[0]);
  return 1;
}
