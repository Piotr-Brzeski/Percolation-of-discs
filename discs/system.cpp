//
//  system.cpp
//  discs
//
//  Created by Piotr Brzeski on 2021-05-20.
//

#include "system.h"
#include <algorithm>
#include <numeric>
#include <cassert>

namespace {

/// Add new value to the sorted vector
template<typename T>
void add(T value, std::vector<T>& vector) {
  auto it = std::lower_bound(vector.begin(), vector.end(), value);
  if(it == vector.end() || *it != value) {
    vector.insert(it, value);
  }
}

auto find_connected_clusters(Disc& new_disc, std::vector<Disc> const& discs, std::map<id_type, std::vector<float_type>>& edge_clusters) {
  auto connected_clusters = std::vector<id_type>();
  for(auto& disc : discs) {
    if(new_disc.touches(disc)) {
      add(disc.get_cluster(), connected_clusters);
    }
  }
  return connected_clusters;
}

void merge_edge_clusters(id_type cluster1, id_type cluster2, std::map<id_type, std::vector<float_type>>& edge_clusters) {
  if(auto cluster2_it = edge_clusters.find(cluster2); cluster2_it != edge_clusters.end()) {
    if(auto cluster1_it = edge_clusters.find(cluster1); cluster1_it != edge_clusters.end()) {
      auto& vector1 = cluster1_it->second;
      auto& vector2 = cluster2_it->second;
      assert(!vector1.empty());
      assert(!vector2.empty());
      auto vector1_size = vector1.size();
      vector1.insert(vector1.end(), vector2.begin(), vector2.end());
      std::inplace_merge(vector1.begin(), vector1.begin() + vector1_size, vector1.end());
    }
    else {
      edge_clusters.emplace(cluster1, std::move(cluster2_it->second));
    }
    edge_clusters.erase(cluster2_it);
  }
}

void merge_clusters(id_type cluster1, id_type cluster2, std::vector<Disc>& discs, std::map<id_type, std::vector<float_type>>& edge_clusters) {
  for(auto& disc : discs) {
    if(disc.get_cluster() == cluster2) {
      disc.set_cluster(cluster1);
    }
  }
  merge_edge_clusters(cluster1, cluster2, edge_clusters);
}

void merge_clusters(std::vector<id_type> const& clusters, std::vector<Disc>& discs, std::map<id_type, std::vector<float_type>>& edge_clusters) {
  assert(clusters.size() > 2);
  assert(std::is_sorted(clusters.begin(), clusters.end()));
  auto new_cluster = clusters.front();
  auto begin = clusters.begin() + 1;
  auto end = clusters.end();
  for(auto& disc : discs) {
    if(std::find(begin, end, disc.get_cluster()) != end) {
      disc.set_cluster(new_cluster);
    }
  }
  for(auto it = begin; it != end; ++it) {
    merge_edge_clusters(new_cluster, *it, edge_clusters);
  }
}

} //  namespace

void System::add_disc() {
  auto position = Position::random();
  auto cluster = discs.size();
  auto disc = Disc(position, cluster);
  auto connected_clusters = find_connected_clusters(disc, discs, edge_clusters);
  if(!connected_clusters.empty()) {
    disc.set_cluster(connected_clusters.front());
    switch(connected_clusters.size()) {
      case 1:
        break;
      case 2:
        merge_clusters(connected_clusters[0], connected_clusters[1], discs, edge_clusters);
        break;
      default:
        merge_clusters(connected_clusters, discs, edge_clusters);
        break;
    }
  }
  discs.push_back(disc);
  if(disc.touches_edge()) {
    auto& edge_cluster = edge_clusters[disc.get_cluster()];
    add(disc.get_position().angle(), edge_cluster);
  }
}

bool System::is_done() const {
  // TODO: Temporary
  std::size_t size = 0;
  size = std::reduce(edge_clusters.begin(), edge_clusters.end(), size, [](auto value, auto& pair){ return value + pair.second.size(); });
  return size >= 2000;
}

std::size_t System::number_of_discs() const {
  return discs.size();
}

std::size_t System::number_of_edge_clusters() const {
  return edge_clusters.size();
}