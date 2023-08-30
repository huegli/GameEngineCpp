#pragma once

#include "../Components/RigidBodyComponent.hpp"
#include "../Components/TranformComponent.hpp"
#include "../ECS/ECS.hpp"

class MovementSystem : public System
{
  public:
    MovementSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void Update(double deltaTime)
    {
        // Loop all entities that the system is interested in
        for (auto entity : GetSystemEntities())
        {
            // Update entity position based on its velocity
            auto &transform = entity.GetComponent<TransformComponent>();
            const auto &rigidbody = entity.GetComponent<RigidBodyComponent>();

            transform.position.x += rigidbody.velocity.x * deltaTime;
            transform.position.y += rigidbody.velocity.y * deltaTime;

            // Logger::Log("Entity id = " + std::to_string(entity.GetId()) +
            //             " positition is now (" +
            //             std::to_string(transform.position.x) + ", " +
            //             std::to_string(transform.position.y) + ")");
        }
    }
};
