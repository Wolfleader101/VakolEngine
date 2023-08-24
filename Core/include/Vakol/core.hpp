#pragma once

#include <memory>

namespace Vakol
{
    class Layer;

    void Run();

    void PushLayer(std::shared_ptr<Layer> layer);

} // namespace Vakol