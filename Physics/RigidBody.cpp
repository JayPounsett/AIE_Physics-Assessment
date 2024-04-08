#include "RigidBody.h"

#include <iostream>

#include <glm/glm.hpp>

#include "PhysicsScene.h"

Rigidbody::~Rigidbody() {}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep) {
  m_position += m_velocity * timeStep;
  applyForce(gravity * m_mass * timeStep, m_position);

  m_orientation += m_angularVelocity * timeStep;

  m_velocity -= m_velocity * timeStep;
  m_angularVelocity -= m_angularVelocity * timeStep;
}

void Rigidbody::applyForce(glm::vec2 force, glm::vec2 position) {
  // In the applyForce() function you will need to calculate acceleration and
  // add it to the m_velocity member variable.You should be able to write this
  // as one line of code.

  m_velocity += force / getMass();
  m_angularVelocity +=
    (force.y * position.x - force.x * position.y) / getMoment();
}

void Rigidbody::resolveCollision(
  Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal) {
  // find the vector beween their centres, or use the provided direction of
  // force and make sure it's normalised
  glm::vec2 normal = glm::normalize(
    collisionNormal ? *collisionNormal : actor2->m_position - m_position);

  // get the vector perpendicular to the collision normal
  glm::vec2 perp(normal.y, -normal.x);

  // determine the total velocity of the contact points for the two objects, for
  // both liinear and rotational

  // 'r' is the radius from axis to application of force
  float r1 = glm::dot(contact - m_position, -perp);
  float r2 = glm::dot(contact - actor2->m_position, perp);

  // velocity of the contact point on this object
  float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;

  // velocity of contact point on actor2
  float v2 =
    glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

  if (v1 > v2) // they're moving closer
  {
    // calculate the effective mass at contact point for each object
    // ie how much the contact point will move due to the force applied
    float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
    float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2 * r2) / actor2->m_moment);

    float elasticity = 1.0f;

    glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) *
                      (v1 - v2) * normal;

    // apply equal and opposite forces
    applyForce(-force, contact - m_position);
    actor2->applyForce(force, contact - actor2->m_position);
  }
}
