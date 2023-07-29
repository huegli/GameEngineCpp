#include "ECS.hpp"

#include<vector>
#include<algorithm>

int Entity::GetId() const {
  return id;
}

void System::AddEntityToSystem(Entity entity) {
  // check if entity already exists and return if so
  // auto it = std::find(entities.begin(), entities.end(), entity);

  // if (it == entities.end()) {

  // }

  // add entity to the end of the vector
  entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
  entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {
    return entity.GetId() == other.GetId();
  }));
}

std::vector<Entity> System::GetSystemEntities() const {
  return entities;
}

const Signature& System::GetComponentSignature() const {
  return componentSignature;
}
