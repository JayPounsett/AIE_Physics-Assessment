#include "RigidBody.h"

#include <iostream>

#include <glm/glm.hpp>

#include "PhysicsScene.h"

Rigidbody::~Rigidbody() {}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
  float cs = cosf(m_orientationRadians);
  float sn = sinf(m_orientationRadians);
  m_localX = glm::normalize(glm::vec2(cs, sn));
  m_localY = glm::normalize(glm::vec2(-sn, cs));

  // If kinematic, it will not move and react to collisions
  if (m_isKinematic)
  {
    m_velocity = glm::vec2(0, 0);
    m_angularVelocity = 0.0f;
    return;
  }

  m_position += m_velocity * timeStep;
  applyForce(gravity * m_mass * timeStep, glm::vec2(0));

  m_orientationRadians += m_angularVelocity * timeStep;

  m_velocity -= m_velocity * m_linearDrag * timeStep;
  m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

  if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
  {
    m_velocity = glm::vec2(0, 0);
  }

  if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
  {
    m_angularVelocity = 0.0f;
  }
}

void Rigidbody::applyForce(glm::vec2 force, glm::vec2 position)
{
  // In the applyForce() function you will need to calculate acceleration and
  // add it to the m_velocity member variable.You should be able to write this
  // as one line of code.

  if (m_isKinematic) return;

  if (getMass() == 0)
  {
    m_velocity += force / 1.0f;
  }
  else
  {
    m_velocity += force / getMass();
  }

  m_angularVelocity +=
    (force.y * position.x + force.x * position.y) / getMoment();
}

void Rigidbody::resolveCollision(
  Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal, float pen)
{
  glm::vec2 normal = glm::normalize(
    collisionNormal ? *collisionNormal : actor2->m_position - m_position);

  glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

  float elasticity = (getElasticity() + actor2->getElasticity()) / 2.0f;

  // get the vector perpendicular to the collision normal
  glm::vec2 perp(normal.y, -normal.x);

  // determine the total velocity of the contact points for the two objects, for
  // both liinear and rotational

  // 'r' is the radius from axis to application of force
  float r1 = glm::dot(contact - getPosition(), -perp);
  float r2 = glm::dot(contact - actor2->getPosition(), perp);

  // velocity of the contact point on this object
  float v1 = glm::dot(getVelocity(), normal) - r1 * getAngularVelocity();

  // velocity of contact point on actor2
  float v2 =
    glm::dot(actor2->getVelocity(), normal) + r2 * actor2->getAngularVelocity();

  if (v1 > v2) // they're moving closer
  {
    // calculate the effective mass at contact point for each object
    // ie how much the contact point will move due to the force applied
    float mass1 = 1.0f / (1.0f / getMass() + (r1 * r1) / getMoment());
    float mass2 =
      1.0f / (1.0f / actor2->getMass() + (r2 * r2) / actor2->getMoment());

    glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) *
                      (v1 - v2) * normal;

    // apply equal and opposite forces
    applyForce(-force, contact - getPosition());
    actor2->applyForce(force, contact - actor2->getPosition());
  }

  if (pen > 0) PhysicsScene::ApplyContactForces(this, actor2, normal, pen);
}
