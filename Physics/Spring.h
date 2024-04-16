#pragma once
#include "PhysicsObject.h"
#include "Rigidbody.h"

class Spring : public PhysicsObject
{
public:
  Spring(
    Rigidbody* body1,
    Rigidbody* body2,
    float springCoefficient,
    float damping,
    glm::vec2 contact1 = glm::vec2(0, 0),
    glm::vec2 contact2 = glm::vec2(0, 0))
    : PhysicsObject(JOINT),
      m_body1(body1),
      m_body2(body2),
      m_springCoefficient(springCoefficient), 
      m_damping(damping),
      m_contact1(contact1),
      m_contact2(contact2)
  {
    m_restLength = glm::length(getContact1() - getContact2());
  }

  ~Spring();

  void fixedUpdate(glm::vec2 gravity, float timeStep);
  virtual void draw();
  glm::vec2 getContact1();
  glm::vec2 getContact2();

protected:
  Rigidbody* m_body1;
  Rigidbody* m_body2;

  glm::vec2 m_contact1;
  glm::vec2 m_contact2;

  float m_damping;
  float m_restLength = 0.0f;
  float m_springCoefficient; // the restoring force
};
