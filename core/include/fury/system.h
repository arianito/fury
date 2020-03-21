//
// Created by Aryan on 10/28/18.
//

#ifndef FURY_PRIMITIVES_SYSTEM_H
#define FURY_PRIMITIVES_SYSTEM_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <list>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using uptr = std::uintptr_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using iptr = std::intptr_t;

using f32 = float;
using f64 = double;
using String = std::string;

template<typename T>
using Vector = std::vector<T>;
template<typename T>
using List = std::list<T>;
template<typename A, typename B>
using Map = std::map<A, B>;
template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T>
using Shared = std::shared_ptr<T>;

using size = std::size_t;


#endif