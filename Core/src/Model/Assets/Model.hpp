#pragma once

#include <memory>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Animation.hpp"

namespace Vakol::Model::Assets 
{
    class Model 
    {
    public:
        Model() = default;

        Model(Mesh& mesh) { m_meshes.push_back(std::move(mesh)); }
        Model(Mesh&& mesh) { m_meshes.push_back(std::move(mesh)); }

        Model(std::vector<Mesh>&& meshes) : m_meshes(std::move(meshes)) {}
        Model(std::vector<Mesh>& meshes) : m_meshes(std::move(meshes)) {}

        //Model(std::vector<Mesh>&& meshes, Animation&& animation) : m_meshes(std::move(meshes)), m_animation(std::make_shared<Animation>(std::move(animation))), m_animated(true) {}
        //Model(std::vector<Mesh>& meshes, Animation& animation) : m_meshes(std::move(meshes)), m_animation(std::make_shared<Animation>(std::move(animation))), m_animated(true) {}

        void set_shader(const std::string& path)  {  this->m_shader = std::make_shared<Shader>(path); this->m_shader->Bind(); }

        [[nodiscard]] bool isAnimated() const { return m_animated; }
        
        std::shared_ptr<Shader>& shader() { return m_shader; }
        [[nodiscard]] const std::shared_ptr<Shader>& c_shader() const { return m_shader; }

        //std::shared_ptr<Animation>& animation() { return m_animation; }
        //[[nodiscard]] const std::shared_ptr<Animation>& c_animation() { return m_animation; }

        //[[nodiscard]] int numTransforms() const { return m_animation->numTransforms(); }
        //[[nodiscard]] const std::vector<glm::mat4>& transforms() const { return m_animation->transforms(); }
        
        [[nodiscard]] Mesh mesh(const int index = 0) const { return m_meshes.at(index); }

        [[nodiscard]] const std::vector<Mesh>& meshes() const { return m_meshes; }
        [[nodiscard]] std::vector<Mesh>& meshes() { return m_meshes; }

    	[[nodiscard]] int nMeshes() const { return static_cast<int>(m_meshes.size()); }

    private:
        std::vector<Mesh> m_meshes;
        //std::shared_ptr<Animation> m_animation = nullptr;
        std::shared_ptr<Shader> m_shader = nullptr;

        bool m_animated = false;
    };

}