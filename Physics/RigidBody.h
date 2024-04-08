#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject {
public:
  Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity,
            float orientation, float mass)
      : PhysicsObject(shapeID) {
    m_shapeID = shapeID;
    m_position = position;
    m_velocity = velocity;
    m_orientation = orientation;
    m_mass = mass;
  };

  ~Rigidbody();

  void fixedUpdate(glm::vec2 gravity, float timeStep);
  void applyForce(glm::vec2 force, glm::vec2 position);
  // void applyForceToActor(Rigidbody *actor2, glm::vec2 force);

  glm::vec2 getPosition() { return m_position; }
  float getOrientation() { return m_orientation; }
  glm::vec2 getVelocity() { return m_velocity; }
  float getMass() { return m_mass; }
  void setMass(float m) { m_mass = m; }
  float getMoment() { return m_moment; }
  float getAngularVelocity() { return m_angularVelocity; }
  float setOrientation(float angle)
  {
    return m_orientation = angle * glm::pi<float>() / 180;
  }

  void resolveCollision(Rigidbody *actor2, glm::vec2 contact);
  float getKineticEnergy() {
    return 0.5f * this->m_mass * glm::dot(this->m_velocity, this->m_velocity);
  };

protected:
  glm::vec2 m_position = glm::vec2(0, 0);
  glm::vec2 m_velocity = glm::vec2(0, 0);
  float m_mass = 0.0f;
  float m_orientation = 0.0f;
  float m_angularVelocity = 0.0f;
  float m_moment = 0.0f;
};
