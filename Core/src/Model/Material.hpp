#pragma once

#include <string>

#include "Math/Math.hpp"

namespace Vakol::Model
{
	class Material
	{
	public:
		virtual ~Material() = default;

		virtual void Bind(const unsigned int type) const = 0;
        virtual void Unbind() const = 0;

        virtual const unsigned int GetID() const = 0;
        
        virtual void SetBool(const std::string& name, const bool value) = 0;
        virtual void SetInt(const std::string& name, const int value) = 0;
        virtual void SetFloat(const std::string& name, const float value) = 0;

        virtual void SetVec2(const std::string& name, const glm::vec2& value) = 0;
        virtual void SetVec2(const std::string& name, const float x, const float y) = 0;

        virtual void SetVec3(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetVec3(const std::string& name, const float x, const float y, const float z) = 0;

        virtual void SetVec4(const std::string& name, const glm::vec4& value) = 0;

        virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
	};
}