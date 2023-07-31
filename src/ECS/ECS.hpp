#pragma once

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>


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
  template <typename T> void RequireComponent();
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
  std::unordered_map<std::type_index, System*> systems;

public:
  Registry() = default;

  // TODO:
  // CreateEntity()
  // KillEntity
  //
  // AddComponent(Entity entity)
  // RemoveComponent(Entity entity)
  // HasComponenent(Entity entity)
  // GetComponenent(Entity entity)
  //
  // AddSystem()
  // RemoveSystem()
  // HasSystem()
  // GetSystem()
  // 
};

// Implementation of the function template
template <typename T> void System::RequireComponent() {
  const auto componentId = Component<T>::GetId();
  componentSignature.set(componentId);
}
