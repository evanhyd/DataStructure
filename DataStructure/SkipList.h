#pragma once
#include <vector>
#include <memory>
#include <chrono>
#include <random>
#include <cassert>

template <typename Key, typename Value, typename Predicate = std::less<Key>>
class SkipList {

  struct Tower {
    std::unique_ptr<Key> _key;
    std::unique_ptr<Value> _value;
    std::unique_ptr<int[]> _nodes; //denote the "next tower index" that it connects to
    int _size; //height = size - 1

    //sentinel
    Tower() : _key(nullptr), _value(nullptr), _nodes(std::make_unique<int[]>(1)), _size(1) {}

    Tower(const std::pair<Key, Value>& keyValue)
      : _key(std::make_unique<Key>(keyValue.first)), _value(std::make_unique<Value>(keyValue.second)), _nodes(), _size(1) {
      static std::default_random_engine engine(std::chrono::system_clock().now().time_since_epoch().count());
      static std::bernoulli_distribution dist;
      while (dist(engine)) {
        ++_size;
      }
      _nodes = std::make_unique<int[]>(_size);
    }

    bool IsSentinel() const {
      return !_key;
    }

    Key& GetKey() {
      return *_key;
    }

    Value& GetValue() {
      return *_value;
    }
  };

  std::vector<Tower> towers;

  std::vector<int> GetPredecessors(const Key& key) {
    using namespace std;
    const Predicate predicate{};
    int index = 0;
    int height = towers.front()._size - 1;
    vector<int> predecessors = { 0 };
    while (height > 0) {
      --height;
      while (true) {
        int nextIndex = towers[index].nodes[height];
        if (!towers[nextIndex].IsSentinel() && predicate(towers[nextIndex].GetKey(), towers[index].GetKey())) {
          index = nextIndex;
        } else {
          break;
        }
      }
      predecessors.push_back(index);
    }
    return predecessors;
  }
};

