#pragma once 

#include <glm/common.hpp>
#include <glm/glm.hpp>

#define MAX_BONE_INFLUENCE 4

namespace Vakol::Model::Assets
{
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec3 tangent;
        glm::vec3 bitangent;
        	//bone indexes which will influence this vertex
	        int m_BoneIDs[MAX_BONE_INFLUENCE];
	        //weights from each bone
	        float m_Weights[MAX_BONE_INFLUENCE];
    };
}
