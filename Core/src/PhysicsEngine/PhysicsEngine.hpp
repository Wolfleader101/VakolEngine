#pragma once

#include <PhysicsEngine/Rigidbody.hpp>

namespace Vakol::Physics {

    class PhysicsEngine {
       public:
        void Update(double deltaTime);
        void Render();
        void AddRigidbody(Rigidbody* body);
        void AddConstraint(const OBB& constraint);
        void ClearRigidbodys();
        void ClearConstraints();

       private:
        std::vector<Rigidbody*> bodies;
        std::vector<OBB> constraints;
    };

}  // namespace Vakol::Physics