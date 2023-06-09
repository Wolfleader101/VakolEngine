#pragma once

#include <string>

#include "Controller/Animator.hpp"
#include "Model/Assets/Model.hpp"

namespace Vakol::Controller {
    /**
     * @brief Load a model with specified parameters.
     *
     * @param path The file path of the model.
     * @param scale The scale factor for the model.
     * @param animated Flag indicating whether the model is animated.
     * @return std::pair<Model::Assets::Model, Animator> A pair of the loaded model and animator.
     */
    std::pair<Model::Assets::Model, Animator> LoadModel(const std::string& path, float scale, bool animated);
}  // namespace Vakol::Controller