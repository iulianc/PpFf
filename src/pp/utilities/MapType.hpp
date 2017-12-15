#ifndef MAP_TYPE_HPP
#define MAP_TYPE_HPP

#ifdef __ORDERED_MAP__
#define MapType std::map
#else
#define MapType std::unordered_map
#endif

#endif
