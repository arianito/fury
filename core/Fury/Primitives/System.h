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

#define NULLPTR nullptr
#define TRUE true
#define FALSE false

using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;

using I8 = int8_t;
using I16 = int16_t;
using I32 = int32_t;
using I64 = int64_t;

using F32 = float;
using F64 = double;
using Char=char;
using Boolean=bool;
using String = std::string;

template<typename T>
using Vector = std::vector<T>;
template<typename A, typename B>
using Map = std::map<A, B>;
template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T>
using Shared = std::shared_ptr<T>;

using Segment = std::size_t;


#endif