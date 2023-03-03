#pragma once

#include "Math.hpp"

struct Vector2
{
	Vector2() = default;

	Vector2(const float x, const float y) { this->vector = glm::vec2(x, y); }
	Vector2(const glm::vec2& vec) { this->vector = vec; }

	glm::vec2 vector;
};

Vector2 operator+(const Vector2& left, const Vector2& right)
{
	return Vector2(left.vector + right.vector);
}

Vector2 operator-(const Vector2& left, const Vector2& right)
{
	return Vector2(left.vector - right.vector);
}

Vector2 operator*(const Vector2& vec, const float k)
{
	return Vector2(vec.vector * k);
}

Vector2 operator/(const Vector2& vec, const float k)
{
	return Vector2(vec.vector / k);
}

bool operator==(const Vector2& left, const Vector2& right)
{
	return left.vector == right.vector;
}

bool operator!=(const Vector2& left, const Vector2& right)
{
	return left.vector != right.vector;
}