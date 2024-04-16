#pragma once
#include "PhysicsObject.h"
#include "Rigidbody.h"

class Spring : public PhysicsObject
{
public:
  Spring();
  ~Spring();

  void fixedUpdate(glm::vec2 gravity, float timeStep);
  glm::vec2 getContact1();
  glm::vec2 getContact2();

  protected:
      Rigidbody* m_body1;
      Rigidbody* m_body2;

      glm::vec2 m_contact1;
      glm::vec2 m_contact2;

      float m_damping;
      float m_restLength;
      float m_springCoefficient;    // the restoring force
};
