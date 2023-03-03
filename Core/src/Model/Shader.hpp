#pragma once

#include <string>

#include "Math/Math.hpp"

typedef unsigned int uint;

class Shader
{
public:
	Shader() = default;
	
	//Shader(const std::string& vertex, const std::string& fragment) const;

	//void Use() const;
	//void Disable() const;

	const uint ID() const { return this->id; }

public:
	//void SetBool(const std::string& name, const bool value) const;
	//void SetInt(const std::string& name, const int value) const;
	//void SetFloat(const std::string& name, const float value) const;

	//void SetFloat2(const std::string& name, const glm::vec2&) const;
	//void SetFloat2(const std::string& name, const float x, const float y) const;

	//void SetFloat3(const std::string& name, const glm::vec3&) const;
	//void SetFloat3(const std::string& name, const float x, const float y, const float z) const;

	//void SetFloat4(const std::string& name, const glm::vec4&) const;

	//void SetMat3(const std::string& name, const glm::mat3&) const;
	//void SetMat4(const std::string& name, const glm::mat4&) const;
private:
	uint id;
};