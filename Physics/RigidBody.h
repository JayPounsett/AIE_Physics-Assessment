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
    m_moment = moment;
  };

  ~Rigidbody();

  void fixedUpdate(glm::vec2 gravity, float timeStep);
  void applyForce(glm::vec2 force, glm::vec2 position);

  void resolveCollision(
    Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

  float getAngularVelocity() const { return m_angularVelocity; }
  
float getKineticEnergy() {
    return 0.5f * m_mass * glm::dot(m_velocity, m_velocity);
  }

  float getMass() {
    if (m_mass != 0)
      return m_mass;
    else
      return 1;
  }

  float getMoment() const { return m_moment; }
  glm::vec2 getPosition() const { return m_position; }
  float getOrientation() const { return m_orientation; }
  glm::vec2 getVelocity() const { return m_velocity; }

  // float getPotentialEnergy() {
  //   PhysicsScene scene;
  //   return -getMass() * glm::dot(scene.getGravity(), getPosition());
  // };

protected:
  glm::vec2 m_position{0, 0};
  glm::vec2 m_velocity{0, 0};
  float m_mass = 0.1f;
  float m_orientation = 0.0f;
  float m_angularVelocity = 0.0f;
  float m_moment = 0.0f;

  float m_linearDrag = 0.3f;
  float m_angularDrag = 0.3f;

  const float MIN_LINEAR_THRESHOLD = 0.01f;
  const float MIN_ANGULAR_THRESHOLD = 0.01f;
};
