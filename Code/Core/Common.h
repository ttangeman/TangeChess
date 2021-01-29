#pragma once

#include <stdint.h>
#include <vector>
#include <array>
#include <memory>
#include <string>
#include <atomic>
#include <unordered_map>
#include <functional>

#include "Core/Debug.h"

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef size_t usize;

#include "Core/Math.h"

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))
