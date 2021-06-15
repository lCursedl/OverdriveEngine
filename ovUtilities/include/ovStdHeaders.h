/*****************************************************************************/
/**
* @file    ovStdHeaders.h
* @author  Gustavo Alvarez (idv18c.galvarez@uartesdigitales.edu.mx)
* @date    2021/03/09
* @brief   Header for the STD libraries
*/
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/*
* C type objects
*/
/*****************************************************************************/
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>
/*****************************************************************************/
/*
* Memory management
*/
/*****************************************************************************/
#include <memory>
/*****************************************************************************/
/*
* STL containers
*/
/*****************************************************************************/
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <set>
#include <list>
#include <forward_list>
#include <deque>
#include <queue>
#include <bitset>
#include <array>
#include <unordered_map>
#include <unordered_set>
/*****************************************************************************/
/*
* STL algorithms and functions
*/
/*****************************************************************************/
#include <algorithm>
#include <functional>
#include <limits>
#include <iterator>
/*****************************************************************************/
/*
* C++ Stream
*/
/*****************************************************************************/
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <chrono>

namespace ovEngineSDK {
  /**
  * @brief       Double ended queue for fast insertion and removal at
  *               begginning and end.
  */
  template<typename T, typename A = std::allocator<T>>
  using Deque = std::deque<T, A>;

  /**
  * @brief       Dynamically sized array that stores element contiguosly.
  */
  template<typename T, typename A = std::allocator<T>>
  using Vector = std::vector<T, A>;

  /**
  * @brief       Double linked list that supports constant insertion and
  *              removal, but without fast random access to elements.
  */
  template<typename T, typename A = std::allocator<T>>
  using List = std::list<T, A>;

  /**
  * @brief       Single linked list that supports constant insertion and
  *              removal, but without fast random access to elements.
  */
  template<typename T, typename A = std::allocator<T>>
  using ForwardList = std::forward_list<T, A>;

  /**
  * @brief       FILO data structure.
  */
  template<typename T, typename A = std::allocator<T>>
  using Stack = std::stack<T, std::deque<T, A>>;

  /**
  * @brief       FIFO data structure.
  */
  template<typename T, typename A = std::allocator<T>>
  using Queue = std::queue<T, std::deque<T, A>>;

  /**
   * @brief      Associative container containing an ordered set of elements.
   */
  template<typename T, typename P = std::less<T>, typename A = std::allocator<T>>
  using Set = std::set<T, P, A>;

  /**
   * @brief      Associative container containing an ordered set of key-value
   *             pairs.
   */
  template<typename K,
    typename V,
    typename P = std::less<K>,
    typename A = std::allocator<std::pair<const K, V>>>
    using Map = std::map<K, V, P, A>;

  /**
   * @brief      Associative container containing an ordered set of elements
   *             where multiple elements can have the same key.
   */
  template<typename T, typename P = std::less<T>, typename A = std::allocator<T>>
  using MultiSet = std::multiset<T, P, A>;

  /**
   * @brief      Associative container containing an ordered set of key-value
   *             pairs where multiple elements can have the same key.
   */
  template<typename K,
    typename V,
    typename P = std::less<K>,
    typename A = std::allocator<std::pair<const K, V>>>
    using MultiMap = std::multimap<K, V, P, A>;

  using String = std::string;

  using WString = std::wstring;

  template<typename T>
  using UPtr = std::unique_ptr<T>;

  template<typename T>
  using SPtr = std::shared_ptr<T>;

  template<typename T>
  using WPtr = std::weak_ptr<T>;

  using std::make_shared;
  using std::make_unique;
  using std::dynamic_pointer_cast;
  using std::static_pointer_cast;
}