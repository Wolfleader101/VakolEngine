#pragma once

#include <memory>

namespace Vakol
{
    class Layer;

    // Functions should be called in this order
    void Init();
    void PushLayer(std::shared_ptr<Layer> layer);
    void Run();

} // namespace Vakol