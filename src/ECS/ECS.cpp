#include "ECS.hpp"
#include "../Logger/Logger.hpp"

#include<vector>
#include<algorithm>

int IComponent::nextId = 0;

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
    return entity == other;
  }), entities.end());
  // in C++20, we can do this instead
  // std::erase_if(entities, [&entity](Entity other) {
  //               return entity == other;
  //               });
}

std::vector<Entity> System::GetSystemEntities() const {
  return entities;
}

const Signature& System::GetComponentSignature() const {
  return componentSignature;
}

Entity Registry::CreateEntity() {
  int entityId;

  entityId = numEntities++;

  Entity entity(entityId);
  entitiesToBeAdded.insert(entity);

  Logger::Log("Entity created with id = " + std::to_string(entityId));

  return entity;

}

void Registry::Update() {
  // TODO: Add the entities that are waiting to be created to the active Systems
  // TODO: Remove the entities that are waiting to be killed from the active Systems
}


