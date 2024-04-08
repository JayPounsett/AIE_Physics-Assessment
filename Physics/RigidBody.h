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
    float orientation,
    float moment)
    : PhysicsObject(shapeID) {
    m_shapeID = shapeID;
    m_position = position;
    m_velocity = velocity;
    m_mass = mass;
    m_orientation = orientation;
    m_moment = moment;
  };

  ~Rigidbody();

  void fixedUpdate(glm::vec2 gravity, float timeStep);
  void applyForce(glm::vec2 force, glm::vec2 position);

  glm::vec2 getPosition() const { return m_position; }
  float getOrientation() { return m_orientation; }
  glm::vec2 getVelocity() { return m_velocity; }
  float getMass() { return m_mass; }
  void setMass(float m) { m_mass = m; }
  float getMoment() { return m_moment; }
  float getAngularVelocity() { return m_angularVelocity; }

  void resolveCollision(
    Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

  float getKineticEnergy() {
    return 0.5f * this->m_mass * glm::dot(this->m_velocity, this->m_velocity);
  }

  // float getPotentialEnergy() {
  //   PhysicsScene scene;
  //   return -getMass() * glm::dot(scene.getGravity(), getPosition());
  // };

protected:
  glm::vec2 m_position = glm::vec2(0, 0);
  glm::vec2 m_velocity = glm::vec2(0, 0);
  float m_mass;
  float m_orientation;
  float m_angularVelocity = 0.0f;
  float m_moment;

  float m_linearDrag = 0.3f;
  float m_angularDrag = 0.3f;

  const float MIN_LINEAR_THRESHOLD = 0.1f;
  const float MIN_ANGULAR_THRESHOLD = 0.01f;
};
