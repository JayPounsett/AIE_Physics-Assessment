#pragma once
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject {
public:
  Rigidbody(
    ShapeType shapeID,
    glm::vec2 position,
    glm::vec2 velocity,
    float mass,
    float elasticity,
    float orientation,
    float moment)
    : PhysicsObject(shapeID) {
    m_shapeID = shapeID;
    m_position = position;
    m_velocity = velocity;
    m_mass = mass;
    m_elasticity = elasticity;
    m_orientation = orientation;
    m_angularVelocity = 0.0f;
    m_moment = moment;
  };

  ~Rigidbody();

  void fixedUpdate(glm::vec2 gravity, float timeStep);
  void applyForce(glm::vec2 force, glm::vec2 pos);

  void resolveCollision(
    Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

  // Getters
  float getKineticEnergy() const {
    return 0.5f * getMass() * glm::dot(getVelocity(), getVelocity());
  }
  float getAngularDrag() const { return m_angularDrag; }
  float getAngularVelocity() const { return m_angularVelocity; }
  float getLinearDrag() const { return m_linearDrag; }
  float getMass() const { return m_mass; }
  float getMoment() const { return m_moment; }
  float getOrientation() const { return m_orientation; }
  glm::vec2 getPosition() const { return m_position; }
  glm::vec2 getVelocity() const { return m_velocity; }



protected:
  glm::vec2 m_position{0, 0};
  glm::vec2 m_velocity{0, 0};
  float m_orientation = 0.0f;
  float m_mass = 0.1f;
  float m_moment = 0.0f;

  float m_angularVelocity = 0.0f;

  float m_linearDrag = 0.3f;
  float m_angularDrag = 0.3f;

  const float MIN_LINEAR_THRESHOLD = 0.01f;
  const float MIN_ANGULAR_THRESHOLD = 0.01f;
};
