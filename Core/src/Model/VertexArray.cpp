#include "VertexArray.hpp"

namespace Vakol::Model {
    const std::vector<Vertex>& VertexArray::GetVerticeVec() { return vertices; }

    const std::vector<unsigned int> VertexArray::GetIndiceVec() { return indices; }
}  // namespace Vakol::Model