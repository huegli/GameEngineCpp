#pragma once

#include <bitset>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;

//////////////////////////////////////////////////////////////////////////////////
// Signature
//////////////////////////////////////////////////////////////////////////////////
// We use a bitset (1s and 0s) to keep track of which components an entity has
// and also helps keep track of which entities a system is interested in
//////////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
protected:
  static int nextId;
};

// Used to assign a unique id to a component type
template <typename T> class Component : public IComponent {
public:
  // Returns the unique id of Component<T>
  static int GetId() {
    static auto id = nextId++;
    return id;
  }
};

class Entity {
private:
  int id;

public:
  Entity(int id) : id(id){};
  Entity(const Entity &entity) = default;
  int GetId() const;

  Entity &operator=(const Entity &other) = default;
  bool operator==(const Entity &other) const { return id == other.id; }
  bool operator!=(const Entity &other) const { return id != other.id; }
  bool operator>(const Entity &other) const { return id > other.id; }
  bool operator<(const Entity &other) const { return id < other.id; }
};

//////////////////////////////////////////////////////////////////////////////////
// System
//////////////////////////////////////////////////////////////////////////////////
// The system processes entities that contain a specific signature
//////////////////////////////////////////////////////////////////////////////////

class System {
private:
  Signature componentSignature;
  std::vector<Entity> entities;

public:
  System() = default;
  ~System() = default;

  void AddEntityToSystem(Entity entity);
  void RemoveEntityFromSystem(Entity entity);
  std::vector<Entity> GetSystemEntities() const;
  const Signature &GetComponentSignature() const;

  // Define the component Type T that entities must have to be
  // considered by the system
  template <typename TComponent> void RequireComponent();
};

//////////////////////////////////////////////////////////////////////////////////
// Pool
//////////////////////////////////////////////////////////////////////////////////
// A pool is just a vector (contiguous data) of objects of type T
//////////////////////////////////////////////////////////////////////////////////
class IPool {
public:
  virtual ~IPool() {}
};

template <typename T> class Pool : IPool {
private:
  std::vector<T> data;

public:
  Pool(int size = 100) { data.resize(size); }

  virtual ~Pool() = default;

  bool isEmpty() const { return data.empty(); }

  int GetSize() const { return data.size(); }

  void Resize(int n) { data.resize(n); }

  void Clear() { data.clear(); }

  void Add(T object) { data.push_back(object); }

  void Set(int index, T object) { data[index] = object; }

  T &Get(int index) { return static_cast<T &>(data[index]); }

  T &operator[](unsigned int index) { return data[index]; }
};

//////////////////////////////////////////////////////////////////////////////////
// Registry
//////////////////////////////////////////////////////////////////////////////////
// The registry manages the creation and destruction of entities, add systems
// and components
//////////////////////////////////////////////////////////////////////////////////
class Registry {
private:
  // Keep track of how many entities were added to the scene
  int numEntities = 0;

  // Vector of component polls, each pool contains all the data for a certain
  // component Vector index = component type id Pool index = entity id
  std::vector<IPool> componentPools;

  // Vector of component signatures
  // The signature let's us know which components are turned "on" for an entity
  // (vector index = entity id)
  std::vector<Signature> entityComponentSignatures;

  // Map of active systems (index = system typeid)
  std::unordered_map<std::type_index, System *> systems;

  // Set of entities that are flagged to be added or removed in the next
  // registry Update()
  std::set<Entity> entitiesToBeAdded;
  std::set<Entity> entitiesToBeKilled;

public:
  Registry() = default;

  // The registry Update() finally processes the entities that are waiting to be
  // added/killed
  void Update();

  // Entity management
  Entity CreateEntity();

  // Compoment management
  template <typename TComponent, typename... TArgs>
  void AddComponent(Entity entity, TArgs &&...args);

  template <typename T> void RemoveComponent(Entity entity);

  template <typename T> bool HasComponenent(Entity entity) const;

  template <typename T> T &GetComponenent(Entity entity) const;

  // void AddEntityToSystem(Entity entity);

  // TODO:
  // CreateEntity()
  // KillEntity
  //
  // AddSystem()
  // RemoveSystem()
  // HasSystem()
  // GetSystem()
  //
};

// Implementation of the function template
template <typename TComponent> void System::RequireComponent() {
  const auto componentId = Component<TComponent>::GetId();
  componentSignature.set(componentId);
}

template <typename TComponent, typename... TArgs>
void Registry::AddComponent(Entity entity, TArgs &&...args) {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  // if componentId not already in componentPools, increase the size
  // of the componentPools.
  if (componentId >= componentPools.size()) {
    componentPools.resize(componentId + 1, nullptr);
  }

  // If there is no pointer to a componentPool already at componentId index
  // then create a new componentPool
  if (!componentPools[componentId]) {
    Pool<TComponent> newComponentPool = new Pool<TComponent>;
    componentPools[componentId] = newComponentPool;
  }

  // pointer to componentPool of componentId
  Pool<TComponent> *componentPool =
      Pool<TComponent>(componentPools[componentId]);

  // ensure that we have space for the entityId in componentPool
  if (entityId >= componentPool->GetSize()) {
    componentPool->Resize(numEntities);
  }

  // create new Component of propert type
  TComponent newComponent(std::forward<TArgs>(args)...);

  //
  componentPool->Set(entityId, newComponent);

  entityComponentSignatures[entityId].set(componentId);
}

template <typename T> void Registry::RemoveComponent(Entity entity) {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.GetId();

  entityComponentSignatures[entityId].set(componentId, false);
}

template <typename T> bool Registry::HasComponenent(Entity entity) const {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.GetId();

  return entityComponentSignatures[entityId].test(componentId);
}
