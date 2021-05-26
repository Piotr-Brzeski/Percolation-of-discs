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
bool add(T value, std::vector<T>& vector) {
  auto it = std::lower_bound(vector.begin(), vector.end(), value);
  if(it == vector.end() || *it != value) {
    vector.insert(it, value);
    return true;
  }
  return false;
}

float_type max_distance(std::vector<float_type> const& angles) {
  assert(angles.size() > 2);
  auto max_distance = angles.front() + two_pi - angles.back();
  for(std::size_t i = 1; i < angles.size(); ++i) {
    auto distance = angles[i] - angles[i - 1];
    max_distance = std::max(distance, max_distance);
  }
  return max_distance;
}

std::vector<float_type> generate_orientations(std::size_t number_of_orientations) {
  auto const step = two_pi/3/number_of_orientations;
  auto orientations = std::vector<float_type>();
  orientations.reserve(number_of_orientations);
  for(float_type angle = 0; angle < two_pi/3; angle += step) {
    orientations.push_back(angle);
  }
  return orientations;
}

std::vector<std::size_t> generate_sequence(std::size_t number_of_values) {
  auto sequence = std::vector<std::size_t>(number_of_values);
  std::iota(sequence.begin(), sequence.end(), 0);
  return sequence;
}

bool contains_value_in_range(float_type from, float_type to, std::vector<float_type> const& values) {
  auto it = std::lower_bound(values.begin(), values.end(), from);
  return it != values.end() && *it < to;
}

bool contains_angle_in_range(float_type from, float_type to, std::vector<float_type> const& angles) {
  assert(from < two_pi);
  if(contains_value_in_range(from, std::min(to, two_pi), angles)) {
    return true;
  }
  if(to > two_pi) {
    return contains_value_in_range(0, to - two_pi, angles);
  }
  return false;
}

bool is_percolation(std::vector<float_type> const& angles, float_type orientation) {
  constexpr auto step = two_pi/3;
  return
    contains_angle_in_range(orientation, orientation + step, angles) &&
    contains_angle_in_range(orientation + step, orientation + 2*step, angles) &&
    contains_angle_in_range(orientation + 2*step, orientation + 3*step, angles);
}

} //  namespace

System::System(std::size_t number_of_orientations)
  : orientations(generate_orientations(number_of_orientations))
  , orientations_to_check(generate_sequence(number_of_orientations))
  , percolations(number_of_orientations, 0)
{
}

void System::add_disc() {
  auto position = Position::random();
  add_disc(position);
}

void System::add_disc(float_type x, float_type y) {
  auto position = Position(x, y);
  if(position.is_valid()) {
    add_disc(position);
  }
}

void System::add_disc(Position const& position) {
  auto cluster = discs.size();
  auto disc = Disc(position, cluster);
  auto connected_clusters = find_connected_clusters(disc);
  auto main_cluster_size = main_cluster ? edge_cluster_size(*main_cluster) : 0;
  if(!connected_clusters.empty()) {
    cluster = connected_clusters.front();
    disc.set_cluster(cluster);
    switch(connected_clusters.size()) {
      case 1:
        break;
      case 2:
        merge_clusters(connected_clusters[0], connected_clusters[1]);
        break;
      default:
        merge_clusters(connected_clusters);
        break;
    }
  }
  bins[disc.get_position()].push_back(discs.size());
  discs.push_back(disc);
  if(disc.touches_edge()) {
    auto& edge_cluster = edge_clusters[disc.get_cluster()];
    add(disc.get_position().angle(), edge_cluster);
  }
  if(auto& angles = updated_main_cluster(main_cluster_size, cluster); !angles.empty()) {
    auto it = orientations_to_check.begin();
    while(it != orientations_to_check.end()) {
      auto index = *it;
      if(is_percolation(angles, orientations[index])) {
        percolations[index] = discs.size();
        it = orientations_to_check.erase(it);
      }
      else {
        ++it;
      }
    }
  }
}

bool System::is_done() const {
  return orientations_to_check.empty();
}

std::size_t System::number_of_discs() const {
  return discs.size();
}

std::size_t System::number_of_edge_clusters() const {
  return edge_clusters.size();
}

std::vector<std::size_t> const& System::get_percolations() const {
  return percolations;
}

std::vector<Disc> const& System::get_discs() const {
  return discs;
}

std::map<id_type, std::vector<float_type>> const& System::get_edge_clusters() const {
  return edge_clusters;
}

std::vector<float_type> const& System::get_main_edge_cluster() const {
  static auto const empty = std::vector<float_type>();
  if(main_cluster) {
    auto it = edge_clusters.find(*main_cluster);
    assert(it != edge_clusters.end());
    auto& vector = it->second;
    return vector;
  }
  return empty;
}

// MARK: - private
std::vector<id_type> System::find_connected_clusters(Disc& new_disc) const {
  auto connected_clusters = std::vector<id_type>();
  auto neighbours = bins.neighbours(new_disc.get_position());
  for(auto& bin : neighbours) {
    auto& disc_indices = bin.get();
    for(auto disc_index : disc_indices) {
      auto& disc = discs[disc_index];
      if(new_disc.touches(disc)) {
        add_recursive(disc_index, connected_clusters);
      }
    }
  }
  return connected_clusters;
}

void System::add_recursive(id_type disc_index, std::vector<id_type>& clusters) const {
  while(true) {
    auto added = add(disc_index, clusters);
    if(!added) {
      return;
    }
    auto cluster = discs[disc_index].get_cluster();
    if(cluster == disc_index) {
      return;
    }
    disc_index = cluster;
  }
}

void System::merge_clusters(id_type cluster1, id_type cluster2) {
  auto& disc = discs[cluster2];
  if(disc.get_cluster() == cluster2) {
    disc.set_cluster(cluster1);
  }
  merge_edge_clusters(cluster1, cluster2);
}

void System::merge_clusters(std::vector<id_type> const& clusters) {
  assert(clusters.size() > 2);
  assert(std::is_sorted(clusters.begin(), clusters.end()));
  auto new_cluster = clusters.front();
  auto begin = clusters.begin() + 1;
  auto end = clusters.end();
  for(auto it = begin; it != end; ++it) {
    auto cluster = *it;
    merge_clusters(new_cluster, cluster);
  }
}

void System::merge_edge_clusters(id_type cluster1, id_type cluster2) {
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
    if(main_cluster == cluster2) {
      main_cluster = cluster1;
    }
  }
}

std::size_t System::edge_cluster_size(id_type cluster) const {
  if(auto it = edge_clusters.find(cluster); it != edge_clusters.end()) {
    return it->second.size();
  }
  return 0;
}

std::vector<float_type> const& System::updated_main_cluster(std::size_t previous_size, id_type cluster) {
  static auto const not_updated = std::vector<float_type>();
  static constexpr auto min_angle = 2*two_pi/3;
  if(main_cluster) {
    auto it = edge_clusters.find(*main_cluster);
    assert(it != edge_clusters.end());
    auto& vector = it->second;
    if(vector.size() > previous_size) {
      return vector;
    }
  }
  else {
    if(auto& angles = edge_clusters.find(cluster)->second; angles.size() > 2 && max_distance(angles) < min_angle) {
      main_cluster = cluster;
      return angles;
    }
  }
  return not_updated;
}
