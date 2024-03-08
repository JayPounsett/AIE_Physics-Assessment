#include "RigidBody.h"

#include <glm/glm.hpp>
#include <iostream>

Rigidbody::Rigidbody(
  ShapeType shapeID,
  glm::vec2 position,
  glm::vec2 velocity,
  float orientation,
  float mass)
  : PhysicsObject(shapeID)
{
  m_shapeID = shapeID;
  m_position = position;
  m_velocity = velocity;
  m_orientation = orientation;
  m_mass = mass;
}

Rigidbody::~Rigidbody() {}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
  m_position += m_velocity * timeStep;
  applyForce(gravity * m_mass * timeStep);
}

void Rigidbody::applyForce(glm::vec2 force)
{
  // In the applyForce() function you will need to calculate acceleration and
  // add it to the m_velocity member variable.You should be able to write this
  // as one line of code.

  m_velocity += force / m_mass;
}

void Rigidbody::applyForceToActor(Rigidbody* actor2, glm::vec2 force)
{
  // In the applyForceToActor() function you must first apply the input force to
  // the input actor(by calling the applyForce() function of that actor), and
  // then apply the opposite force to the current actor(by calling applyForce()
  // on this using the negative force).

  actor2->applyForce(force); // Input actor & input force
  this->applyForce(-force); // Current actor & negative input force
}

void Rigidbody::resolveCollision(Rigidbody* actor2)
{
  glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
  glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

  // if the objects are already moving apart, we don't need to do anything
  if (glm::dot(normal, relativeVelocity) >= 0) return;

  float elasticity = 1;
  float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
            ((1 / m_mass) + (1 / actor2->getMass()));

  glm::vec2 force = normal * j;

  float kePre = getKineticEnergy() + actor2->getKineticEnergy();

  applyForceToActor(actor2, force);

  float kePost = getKineticEnergy() + actor2->getKineticEnergy();

  float deltaKE = kePost - kePre;
  if (deltaKE > kePost * 0.01f)
    std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";
}
