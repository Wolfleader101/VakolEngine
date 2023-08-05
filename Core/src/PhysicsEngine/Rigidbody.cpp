#include "Rigidbody.hpp"

namespace Vakol::Physics {
    Particle::Particle() {
        friction = 0.95f;
        gravity = Vec3(0.0f, -9.82f, 0.0f);
        mass = 1.0f;
        bounce = 0.7f;
    }

    void Particle::Update(float deltaTime) {
        oldPos = pos;
        Vec3 a = forces * (1.0f / mass);
        velocity = velocity * friction + a * deltaTime;
        pos = pos + velocity * deltaTime;
    }

    void Particle::Render() {
        Sphere visual(pos, 0.1f);

        Vakol::Physics::Render(visual);
    }

    void Particle::ApplyForces() { forces = gravity; }
    void Particle::SolveConstraints(const std::vector<OBB>& constraints) {
        // todo
        for (size_t i = 0; i < constraints.size(); i++) {
        }
    }

}  // namespace Vakol::Physics