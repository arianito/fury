#pragma once

#include <cstdio>

using EntityId = std::size_t;
using SystemId = std::size_t;
using ComponentId = std::size_t;
using PoolId = std::size_t;
static constexpr EntityId INVALID_ENTITY_ID = 0;