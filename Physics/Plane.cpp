#include "Plane.h"

#include "Gizmos.h"

Plane::Plane(glm::vec2 normal, float distance, glm::vec4 colour)
  : PhysicsObject::PhysicsObject(PLANE)
{
  this->normal = normal;
  this->distanceToOrigin = distance;
  this->colour = colour;
}

Plane::~Plane() {}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep) {}

void Plane::Draw()
{
  float lineSegmentLength = 300.f;
  glm::vec2 centerPoint = normal * distanceToOrigin;
  // easy to rotate normal through 90 degrees around z-axis

  glm::vec2 parallel(normal.y, -normal.x);

  glm::vec4 colourFade = colour;
  colourFade.a = 0;

  glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
  glm::vec2 end = centerPoint - (parallel * lineSegmentLength);

  aie::Gizmos::add2DTri(
    start, end, start - normal * 10.f, colour, colour, colourFade);
  aie::Gizmos::add2DTri(
    end,
    end - normal * 10.f,
    start - normal * 10.f,
    colour,
    colourFade,
    colourFade);
}

void Plane::ResetPosition() {}
