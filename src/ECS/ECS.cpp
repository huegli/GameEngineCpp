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
  entity.registry = this;
  entitiesToBeAdded.insert(entity);

  if (entityId >= entityComponentSignatures.size()) {
    entityComponentSignatures.resize(entityId + 1);
  }
  
  Logger::Log("Entity created with id = " + std::to_string(entityId));

  return entity;

}

void Registry::AddEntityToSystem(Entity entity) {
  const auto entityId = entity.GetId();

  // TODO: match entityComponentSignature <---> systemCompomentSignature
  const auto& entityComponentSignature = entityComponentSignatures[entityId];
  
  // Loop all the systems
  for (auto& system: systems) {
    const auto& systemCompomentSignature = system.second->GetComponentSignature();

    bool isInterested = (entityComponentSignature & systemCompomentSignature) == systemCompomentSignature;

    if (isInterested) {
      system.second->AddEntityToSystem(entity);
    }
  }
}

void Registry::Update() {
  // Add the entities that are waiting to be created to the active Systems
  for (auto entity: entitiesToBeAdded) {
    AddEntityToSystem(entity);
  }
  entitiesToBeAdded.clear();
  
  // TODO: Remove the entities that are waiting to be killed from the active Systems
}


