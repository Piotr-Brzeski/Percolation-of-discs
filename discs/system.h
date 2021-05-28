//
//  system.h
//  discs
//
//  Created by Piotr Brzeski on 2021-05-20.
//

#pragma once

#include "disc.h"
#include "bins.h"
#include <vector>
#include <map>
#include <optional>

class System {
public:
  System(float_type disc_radius, std::size_t number_of_orientations);
  void add_disc();
  void add_disc(float_type x, float_type y);
  
  bool is_done() const;
  
  std::size_t number_of_discs() const;
  std::size_t number_of_edge_clusters() const;
  
  std::vector<std::size_t> const& get_percolations() const;
  
  void print_stats() const;
  
  // For Graphical Test Application
  std::vector<Disc> const& get_discs() const;
  std::map<id_type, std::vector<float_type>> const& get_edge_clusters() const;
  std::vector<float_type> const& get_main_edge_cluster() const;
  
private:
  void add_disc(Position const& position);
  std::vector<id_type> find_connected_clusters(Disc& new_disc) const;
  void add_recursive(id_type disc_index, std::vector<id_type>& clusters) const;
  void merge_clusters(id_type cluster1, id_type cluster2);
  void merge_clusters(std::vector<id_type> const& clusters);
  void merge_edge_clusters(id_type cluster1, id_type cluster2);
  std::size_t edge_cluster_size(id_type cluster) const;
  std::vector<float_type> const& updated_main_cluster(std::size_t previous_size, id_type cluster);
  
  Bins                                                 bins;
  std::vector<Disc>                                    discs;
  std::map<id_type, std::vector<float_type>>           edge_clusters; // cluster -> sorted vector of angles
  std::optional<id_type>                               main_cluster;
  std::vector<float_type> const                        orientations;
  std::vector<std::size_t>                             orientations_to_check;
  std::vector<std::size_t>                             percolations;
};
