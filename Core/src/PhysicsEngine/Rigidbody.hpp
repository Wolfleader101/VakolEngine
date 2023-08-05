#pragma once

#include <vector>

#include "Colliders.hpp"
#include "DrawPrimitives.hpp"

namespace Vakol::Physics {

    class Rigidbody {
       public:
        Rigidbody() {}
        virtual ~Rigidbody() {}
        virtual void Update(float deltaTime) = 0;
        virtual void Render() = 0;
        virtual void ApplyForces() = 0;
        virtual void SolveConstraints(const std::vector<OBB>& constraints) = 0;

       private:
    };

    class Particle : public Rigidbody {
       public:
        Particle();

        void Update(float deltaTime);

        void Render();
        void ApplyForces();
        void SolveConstraints(const std::vector<OBB>& constraints);

        void SetPosition(const Vec3& newPos) { pos = oldPos = newPos; }
        Vec3 GetPosition() { return pos; }
        void SetBounce(float b) { bounce = b; }
        float GetBounce() { return bounce; }

       private:
        Vec3 pos;
        Vec3 oldPos;
        Vec3 forces;
        Vec3 velocity;
        float mass;
        float bounce;
        Vec3 gravity;
        float friction;
    };
}  // namespace Vakol::Physics