#pragma once
#include <vector>

#include <glm/vec2.hpp>

#include "Sphere.h"

class PhysicsObject;

class PhysicsScene
{
public:
  PhysicsScene();
  ~PhysicsScene();

  void addActor(PhysicsObject* actor);
  void removeActor(PhysicsObject* actor);
  void update(float dt);
  void draw();

  void setGravity(const glm::vec2 gravity) { this->m_gravity = gravity; }; 
  glm::vec2 getGravity() const { return m_gravity; }

  void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
  float getTimeStep() const { return m_timeStep; }

  static bool plane2Plane(PhysicsObject*, PhysicsObject*);
  static bool plane2Sphere(PhysicsObject*, PhysicsObject*);
  static bool plane2Box(PhysicsObject*, PhysicsObject*);
  static bool sphere2Plane(PhysicsObject*, PhysicsObject*);
  static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);

protected:
  glm::vec2 m_gravity;
  float m_timeStep = 0.1f;

  // Vector collection that stores all the physical objects in the scene,
  // along with functions to add and remove objects to and from this list.
  std::vector<PhysicsObject*> m_actors;
};
