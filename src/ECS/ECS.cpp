#include "ECS.hpp"

#include<vector>
#include<algorithm>

int Entity::GetId() const {
  return id;
}

void System::AddEntityToSystem(Entity entity) {
  // add entity to the end of the vector
  entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
  // implementation of the erase-remove idiom
  // swap the found entities to the end and then erase them from the end of the vector
  entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {
    return entity.GetId() == other.GetId();
  }), entities.end());
}

std::vector<Entity> System::GetSystemEntities() const {
  return entities;
}

const Signature& System::GetComponentSignature() const {
  return componentSignature;
}
