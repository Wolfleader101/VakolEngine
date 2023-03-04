#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

#include <algorithm>
#include <cmath>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Vakol::Model::Math {
static inline bool NearEqual(const float a, const float b, const float epsilon) { return abs(a - b) < epsilon; }
static inline float Clamp01(const float value) { return std::max(0.0f, std::min(value, 1.0f)); }

static inline float Clamp(const float value, const float lower, const float upper) {
    return std::max(lower, std::min(value, upper));
}

static const inline double PI = M_PI;

static const inline float FLOAT_MAX = FLT_MAX;
static const inline float FLOAT_MIN = FLT_MIN;

static const inline float Deg2Rad = PI / 180.0f;
static const inline float Rad2Deg = 180.0f / PI;
}  // namespace Vakol::Model::Math