#pragma once

#include <string>
#include <unordered_map>

namespace Vakol::Rendering::Assets
{
    struct Model;
}

namespace Vakol
{

    class ModelLibrary
    {
        public:
            /*
            *   @brief Find a model at a given path
            *   @param inputPath The path to the model
            */
            Rendering::Assets::Model& FindModel(const std::string& path);
            /*
            *   @brief Get a model from m_models with a given path. If not found, import new model
            *   @param inputPath The path to the model
            */
            Rendering::Assets::Model& GetModel(const std::string& path, float scale = 1.0f);
            /*
            *   @brief Add a model to m_models
            *   @param inputModel The model to add
            */
            void AddModel(const Rendering::Assets::Model& inputModel);  

        private:
            /*
            *   @brief Get the error model
            *   @param inputScale The scale of the model
            */
            Rendering::Assets::Model& GetErrorModel(float scale);

            /*
            *   @brief A map of models with their corisponding paths
            *
            */
            std::unordered_map<std::string, Rendering::Assets::Model> m_models;
    };
} // namespace Vakol