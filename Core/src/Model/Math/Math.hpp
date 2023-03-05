#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <math.h>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace Vakol::Model::Math
{
	/// <summary>
	/// A simple float comparison function that compares the difference of two float vlaues
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="epsilon"></param>
	/// <returns></returns>
	static inline bool NearEqual(const float a, const float b, const float epsilon)
	{
		return abs(a - b) < epsilon;
	}

	/// <summary>
	/// Clamp a float value between 0 and 1
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	static inline float Clamp01(const float value)
	{
		return std::max(0.0f, std::min(value, 1.0f));
	}

	static inline float Clamp(const float value, const float lower, const float upper)
	{
		return std::max(lower, std::min(value, upper));
	}

	static const inline double PI = M_PI;

	static const inline float FLOAT_MAX = FLT_MAX;
	static const inline float FLOAT_MIN = FLT_MIN;

	static const inline float Deg2Rad = PI / 180.0f;
	static const inline float Rad2Deg = 180.0f / PI;
}