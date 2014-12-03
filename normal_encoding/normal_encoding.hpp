#pragma once

#if defined(__clang__) && __clang_major__ <= 3 && __clang_minor__ <= 1
  #include <stdint.h>
#else
  #include <cstdint>
#endif

struct float3
{
	float x, y, z;
};

// encode normalized 3d vector to 16-bits unsigned integer
uint16_t encode(const float3& v);

float3 decode(uint16_t s);
