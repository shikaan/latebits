#pragma once

#include <memory>
#include <string>

#include "latebit/core/events/EventTarget.h"
#include "latebit/core/world/SceneGraph.h"

namespace lb {
class WorldManager;
class Scene : public EventTarget {
  friend class WorldManager;

 private:
  // All objects. This vector owns all the objects in the graph and its
  // responsible for their memory management
  vector<unique_ptr<Object>> objects = {};
  // Pointer to the scene graph this scene controls
  SceneGraph* graph = nullptr;
  // Scene label
  string label = "";
  // Set the scene graph for this scene
  void setSceneGraph(SceneGraph& graph);
  // Set the scene label
  void setLabel(const string label);

  // Scene activation flag
  bool active = false;

 public:
  Scene() = default;
  virtual ~Scene() = default;

  // Called after the scene is activated
  virtual void onActivated(){};
  // Called after the scene is deactivated
  virtual void onDeactivated(){};

  // Add object to the scene
  void addObject(unique_ptr<Object> o);
  // Remove object from the scene
  void removeObject(Object* o);

  // Runs the scene activation sequence (i.e., add objects to the graph)
  void activate();
  // Runs the scene deactivation sequence
  void deactivate();

  // Returns the objects in the scene
  [[nodiscard]] auto getObjects() const -> const vector<unique_ptr<Object>>&;
  // Returns the scene label
  [[nodiscard]] auto getLabel() const -> const string&;
  // Returns true if the scene is active
  [[nodiscard]] auto isActive() const -> bool override;
};
}  // namespace lb