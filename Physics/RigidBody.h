#pragma once
#include <glm/glm.hpp>

#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject
{
public:
  Rigidbody(
    ShapeType shapeID,
    glm::vec2 position,
    glm::vec2 velocity,
    float orientation,
    float mass);
  ~Rigidbody();

  void fixedUpdate(glm::vec2 gravity, float timeStep);
  void applyForce(glm::vec2 force);
  void applyForceToActor(Rigidbody* actor2, glm::vec2 force);

  glm::vec2 getPosition() { return m_position; }
  float getOrientation() { return m_orientation; }
  glm::vec2 getVelocity() { return m_velocity; }
  float getMass() { return m_mass; }
  void setMass(float m) { m_mass = m; }

  void resolveCollision(Rigidbody* actor2);
  float getKineticEnergy()
  {
    return 0.5f * this->m_mass * glm::dot(this->m_velocity, this->m_velocity);
  };

protected:
  glm::vec2 m_position;
  glm::vec2 m_velocity;
  float m_mass;
  float m_orientation; // 2D so we only need a single float to represent
                       // orientation
};
