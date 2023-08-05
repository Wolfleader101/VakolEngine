#include "PhysicsEngine.hpp"

#include <glad/glad.h>

#include "DrawPrimitives.hpp"

namespace Vakol::Physics {

    void PhysicsEngine::AddRigidbody(Rigidbody* body) { this->bodies.push_back(body); }

    void PhysicsEngine::AddConstraint(const OBB& obb) { constraints.push_back(obb); }

    void PhysicsEngine::ClearRigidbodys() { bodies.clear(); }

    void PhysicsEngine::ClearConstraints() { constraints.clear(); }

    void PhysicsEngine::Render() {
        static const float rigidbodyDiffuse[]{200.0f / 255.0f, 0.0f, 0.0f, 0.0f};
        static const float rigidbodyAmbient[]{200.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 0.0f};
        static const float constraintDiffuse[]{0.0f, 200.0f / 255.0f, 0.0f, 0.0f};
        static const float constraintAmbient[]{50.0f / 255.0f, 200.0f / 255.0f, 50.0f / 255.0f, 0.0f};
        static const float zero[] = {0.0f, 0.0f, 0.0f, 0.0f};

        glColor3f(rigidbodyDiffuse[0], rigidbodyDiffuse[1], rigidbodyDiffuse[2]);
        glLightfv(GL_LIGHT0, GL_AMBIENT, rigidbodyAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, rigidbodyDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, zero);

        for (size_t i = 0, size = bodies.size(); i < size; ++i) {
            bodies[i]->Render();
        }

        glColor3f(constraintDiffuse[0], constraintDiffuse[1], constraintDiffuse[2]);
        glLightfv(GL_LIGHT0, GL_AMBIENT, constraintAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, constraintDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, zero);

        for (size_t i = 0; i < constraints.size(); ++i) {
            Vakol::Physics::Render(constraints[i]);
        }
    }

    void PhysicsEngine::Update(double deltaTime) {
        for (size_t i = 0, size = bodies.size(); i < size; ++i) {
            bodies[i]->ApplyForces();
        }
        for (size_t i = 0, size = bodies.size(); i < size; ++i) {
            bodies[i]->Update(deltaTime);
        }
        for (size_t i = 0, size = bodies.size(); i < size; ++i) {
            bodies[i]->SolveConstraints(constraints);
        }
    }
}  // namespace Vakol::Physics