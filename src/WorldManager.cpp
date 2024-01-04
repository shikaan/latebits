#include "WorldManager.h"

#include "EventCollision.h"
#include "LogManager.h"
#include "ObjectListIterator.h"
#include "utils.h"

#define WM df::WorldManager::getInstance()

namespace df {

WorldManager::WorldManager() {
  setType("WorldManager");
  m_deletions = ObjectList();
  m_updates = ObjectList();
  LM.writeLog("WorldManager::WorldManager(): Created WorldManager");
}

WorldManager& WorldManager::getInstance() {
  static WorldManager instance;
  return instance;
}

int WorldManager::startUp() {
  m_deletions = ObjectList();
  m_updates = ObjectList();
  LM.writeLog("WorldManager::startUp(): Started successfully");
  return Manager::startUp();
}

void WorldManager::shutDown() {
  auto updates = m_updates;  // create a copy
  auto iterator = ObjectListIterator(&updates);
  for (iterator.first(); !iterator.isDone(); iterator.next()) {
    // This is not leaving a danglig null reference!
    // In the destructor of Object, we also remove it from the world
    delete iterator.currentObject();
  }

  m_updates.clear();

  Manager::shutDown();
  LM.writeLog("WorldManager::shutDown(): Shut down successfully");
}

int WorldManager::insertObject(Object* p_o) { return m_updates.insert(p_o); }

int WorldManager::removeObject(Object* p_o) { return m_updates.remove(p_o); }

ObjectList WorldManager::getAllObjects() const { return m_updates; }

ObjectList WorldManager::objectsOfType(std::string type) const {
  ObjectList result;
  auto i_updates = ObjectListIterator(&m_updates);

  for (i_updates.first(); !i_updates.isDone(); i_updates.next()) {
    if (i_updates.currentObject()->getType() == type) {
      result.insert(i_updates.currentObject());
    }
  }

  return result;
}

ObjectList WorldManager::getCollisions(Object* p_o, Vector where) const {
  ObjectList collisions;
  auto i_updates = ObjectListIterator(&m_updates);

  for (i_updates.first(); !i_updates.isDone(); i_updates.next()) {
    auto p_current = i_updates.currentObject();
    if (p_current == p_o) continue;

    if (p_current->isSolid() &&
        positionsMatch(p_current->getPosition(), where)) {
      collisions.insert(p_current);
    }
  }

  return collisions;
}

int WorldManager::moveObject(Object* p_o, Vector where) {
  // Spectral objects can just move
  if (!p_o->isSolid()) {
    p_o->setPosition(where);
    return 0;
  }

  ObjectList collisions = getCollisions(p_o, where);

  // In absence of collisions, just move
  if (collisions.isEmpty()) {
    p_o->setPosition(where);
    return 0;
  }

  bool move = true;
  auto i_collisions = ObjectListIterator(&collisions);

  for (i_collisions.first(); !i_collisions.isDone(); i_collisions.next()) {
    auto p_current = i_collisions.currentObject();
    auto event = EventCollision(p_o, p_current, where);
    p_o->eventHandler(&event);
    p_current->eventHandler(&event);

    if (p_o->getSolidness() == HARD && p_current->getSolidness() == HARD) {
      move = false;
      break;
    }
  }

  if (move) {
    p_o->setPosition(where);
    return 0;
  } else {
    return -1;
  }

  return 0;
}

void WorldManager::update() {
  auto i_deletions = ObjectListIterator(&m_deletions);
  for (i_deletions.first(); !i_deletions.isDone(); i_deletions.next()) {
    removeObject(i_deletions.currentObject());
    delete i_deletions.currentObject();
  }
  m_deletions.clear();

  auto i_updates = ObjectListIterator(&m_updates);
  for (i_updates.first(); !i_updates.isDone(); i_updates.next()) {
    auto object = i_updates.currentObject();
    auto old_position = object->getPosition();
    auto new_position = object->predictPosition();

    if (old_position != new_position) {
      moveObject(object, new_position);
    }
  }
}

int WorldManager::markForDelete(Object* p_o) {
  // Prevents marking the same object for deletion twice
  auto i_deletions = ObjectListIterator(&m_deletions);
  for (i_deletions.first(); !i_deletions.isDone(); i_deletions.next()) {
    if (i_deletions.currentObject() == p_o) return 0;
  }

  return m_deletions.insert(p_o);
}
void WorldManager::draw() {
  auto iterator = new ObjectListIterator(&m_updates);

  for (int i = 0; i < MAX_ALTITUDE; i++) {
    for (iterator->first(); !iterator->isDone(); iterator->next()) {
      auto object = iterator->currentObject();

      if (object != nullptr && object->getAltitude() == i) object->draw();
    }
  }
}
}  // namespace df
