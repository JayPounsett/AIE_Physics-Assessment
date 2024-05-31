#pragma once
#include "PhysicsObject.h"
#include "glm/glm.hpp"

class Rigidbody;

// The implementation of all of these functions, excepting the draw()
// function,are all relatively trivial.Because all plane objects will be static,
// the fixedUpdate() function will be empty. The other functions will simply set
// various member variables. You should be able to complete these by yourself.

class Plane : public PhysicsObject
{
public:
  Plane(glm::vec2 normal, float distance, float elasticity)
    : PhysicsObject::PhysicsObject(PLANE)
  {
    this->m_normal = normal;
    this->m_distanceToOrigin = distance;
    this->m_elasticity = elasticity;
  };

  Plane() : PhysicsObject(ShapeType::PLANE)
  {
    m_distanceToOrigin = 0;
    m_normal = glm::vec3(0, 1, 0);
  };
  ~Plane();

  virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
  virtual void draw();
  virtual void resetPosition(ShapeType PLANE);

  glm::vec2 getNormal() const { return m_normal; }
  float getDistance() const { return m_distanceToOrigin; }

  void setColour(glm::vec4 colour) { m_colour = colour; }

  void resolveCollision(
    Rigidbody* actor2, glm::vec2 contact);

protected:
  glm::vec4 m_colour = glm::vec4(0.25, 0.25, 0.25, 1);
  glm::vec2 m_normal = glm::vec2(0, 1);
  float m_distanceToOrigin = 0.0f;
};
