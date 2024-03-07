#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

class Sphere : public RigidBody
{
public:
  Sphere(
    glm::vec2 position,
    glm::vec2 velocity,
    float mass,
    float radius,
    glm::vec4 colour)
    : RigidBody(SPHERE, position, velocity, 0, mass)
  {
    m_radius = radius;
    m_colour = colour;
  };

  ~Sphere();

  virtual void draw();

  float getRadius() { return m_radius; }
  glm::vec4 getColour() { return m_colour; }

protected:
  float m_radius;
  glm::vec4 m_colour;
};
