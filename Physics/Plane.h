#pragma once
#include "PhysicsObject.h"
#include "glm/glm.hpp"

class Plane : public PhysicsObject
{
public:
  Plane(glm::vec2 normal, float distance, glm::vec4 colour)
    : PhysicsObject::PhysicsObject(PLANE)
  {
    this->m_normal = normal;
    this->m_distanceToOrigin = distance;
    this->m_colour = colour;
  };

  Plane() : PhysicsObject(ShapeType::PLANE)
  {
    m_distanceToOrigin = 0;
    m_normal = glm::vec2(0, 1);
  };
  ~Plane();

  virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
  virtual void draw();
  virtual void resetPosition(ShapeType PLANE);

  glm::vec2 getNormal() { return m_normal; }
  float getDistance() { return m_distanceToOrigin; }

protected:
  glm::vec4 m_colour = {0, 0, 0, 0};
  glm::vec2 m_normal = glm::vec2(0, 1);
  float m_distanceToOrigin = 0.f;
};
