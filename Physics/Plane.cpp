#include "Plane.h"

#include "Gizmos.h"
#include "RigidBody.h"
#include <iostream>

Plane::~Plane() {}

void Plane::fixedUpdate(glm::vec2 gravity, float timeStep) {}

void Plane::draw()
{
  float lineSegmentLength = 300.f;
  glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
  // easy to rotate normal through 90 degrees around z-axis

  glm::vec2 parallel(m_normal.y, -m_normal.x);

  glm::vec4 colourFade = m_colour;
  colourFade.a = 0;

  glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
  glm::vec2 end = centerPoint - (parallel * lineSegmentLength);

  // aie::Gizmos::add2DLine(start, end, colour);
  aie::Gizmos::add2DTri(
    start, end, start - m_normal * 10.f, m_colour, m_colour, colourFade);
  aie::Gizmos::add2DTri(
    end,
    end - m_normal * 10.f,
    start - m_normal * 10.f,
    m_colour,
    colourFade,
    colourFade);
}

void Plane::resetPosition(ShapeType PLANE)
{
  // Need to work out resetting the position of the plane
  // setPosition is not accessible as a Plane as it is a PhysicsObject

  // this->setPosition(glm::vec2(0, 0));
}

void Plane::resolveCollision(Rigidbody* actor2)
{
    // Because plane is static (immovable), 
    // - We only need to apply the calculated impulse to the sphere
    // - The relative velocity will be the sphere's velocity
    // - The collision normal will be the plane's normal

  glm::vec2 normal = m_normal;
  glm::vec2 relativeVelocity = actor2->getVelocity();

  // if the objects are already moving apart, we don't need to do anything
  if (glm::dot(normal, relativeVelocity) >= 0) return;

  float elasticity = 1;

  float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal);

  glm::vec2 force = normal * j;

  actor2->applyForce(force);
}
