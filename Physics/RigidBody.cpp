#include "RigidBody.h"

#include <iostream>

#include <glm/glm.hpp>

#include "PhysicsScene.h"

Rigidbody::~Rigidbody() {}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep) {
  m_position += getVelocity() * timeStep;
  applyForce(gravity * getMass() * timeStep, getPosition());

  m_orientation += getAngularVelocity() * timeStep;

  m_velocity -= getVelocity() * m_linearDrag * timeStep;
  m_angularVelocity -= getAngularVelocity() * m_angularDrag * timeStep;

  if (length(getVelocity()) < MIN_LINEAR_THRESHOLD) {
    m_velocity = glm::vec2(0, 0);
  }

  if (abs(getAngularVelocity()) < MIN_ANGULAR_THRESHOLD) {
    m_angularVelocity = 0.0f;
  }
}

void Rigidbody::applyForce(glm::vec2 force, glm::vec2 pos) {
  // In the applyForce() function you will need to calculate acceleration and
  // add it to the m_velocity member variable.You should be able to write this
  // as one line of code.

  // Check to make sure we don't divide by 0
  if (getMass() == 0) {
    m_velocity += force / 1.0f;
  } else {
    m_velocity += force / getMass();
  }

  m_angularVelocity +=
    (force.y * pos.x - force.x * pos.y) / getMoment();
}

void Rigidbody::resolveCollision(
  Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal) {
  // find the vector between their centres, or use the provided direction of
  // force and make sure it's normalised
  glm::vec2 normal = glm::normalize(
    collisionNormal ? *collisionNormal : actor2->getPosition() - getPosition());
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
}
