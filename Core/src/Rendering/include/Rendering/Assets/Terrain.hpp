#pragma once

namespace Vakol::Rendering::Assets
{
    struct Terrain
    {
        float min = 0.0f;
        float max = 0.0f;

      private:
        int size = 0; // size of terrain (heightmap)
    };
} // namespace Vakol::Rendering::Assets