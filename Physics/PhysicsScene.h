#pragma once
#include <vector>

#include <glm/vec2.hpp>

class Rigidbody;
class PhysicsObject;

class PhysicsScene {
public:
  PhysicsScene();
  ~PhysicsScene();

  void addActor(PhysicsObject* actor);
  void removeActor(PhysicsObject* actor);
  void update(float dt);
  void draw();
  void checkForCollision();
  static void ApplyContactForces(
    Rigidbody* body1, Rigidbody* body2, glm::vec2 norm, float pen);

  // Getters
  glm::vec2 getGravity() const { return m_gravity; }
  float getTimeStep() const { return m_timeStep; }

  // Setters
  void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
  void setGravity(glm::vec2 gravity) { this->m_gravity = gravity; }

  // Collision Checks
  static bool plane2Plane(PhysicsObject*, PhysicsObject*);
  static bool plane2Sphere(PhysicsObject*, PhysicsObject*);
  static bool plane2Box(PhysicsObject*, PhysicsObject*);
  static bool sphere2Plane(PhysicsObject*, PhysicsObject*);
  static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);
  static bool sphere2Box(PhysicsObject*, PhysicsObject*);
  static bool box2Plane(PhysicsObject*, PhysicsObject*);
  static bool box2Sphere(PhysicsObject*, PhysicsObject*);
  static bool box2Box(PhysicsObject*, PhysicsObject*);

protected:
  glm::vec2 m_gravity;
  float m_timeStep = 0.1f;

  // Vector collection that stores all the physical objects in the scene,
  // along with functions to add and remove objects to and from this list.
  std::vector<PhysicsObject*> m_actors;
};
