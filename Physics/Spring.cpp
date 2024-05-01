#include "Spring.h"
#include <Gizmos.h>

Spring::~Spring() {}

void Spring::fixedUpdate(glm::vec2 gravity, float timeStep)
{
    // get the world coords of the ends of the springs
  glm::vec2 p1 = getContact1();
  glm::vec2 p2 = getContact2();

  float length = glm::distance(p1, p2);
  glm::vec2 direction = glm::normalize(p2 - p1);

  // apply damping
  glm::vec2 relativeVelocity = m_body2->getVelocity() - m_body1->getVelocity();

  // F = -kX - bv
  glm::vec2 force = direction * m_springCoefficient * (m_restLength - length) -
                    m_damping * relativeVelocity;

  m_body1->applyForce(-force * timeStep, p1 - m_body1->getPosition());
  m_body2->applyForce(force * timeStep, p2 - m_body2->getPosition());
}

void Spring::draw()
{
  aie::Gizmos::add2DLine(getContact1(), getContact2(), glm::vec4(0.25, 0.25, 0.25, 1));
}

glm::vec2 Spring::getContact1()
{
  return m_body1 ? m_body1->toWorld(m_contact1) : m_contact1;
}

glm::vec2 Spring::getContact2()
{
  return m_body2 ? m_body2->toWorld(m_contact2) : m_contact2;
}
