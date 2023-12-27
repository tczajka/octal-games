#ifndef CONTAINER_UTIL_H
#define CONTAINER_UTIL_H

#include <map>
#include <stdexcept>

template<typename K, typename V>
const V &map_get(const std::map<K, V> &m, const K &key) {
  auto it = m.find(key);
  if (it == m.end()) throw std::runtime_error("key not found in map");
  return it->second;
}

#endif
