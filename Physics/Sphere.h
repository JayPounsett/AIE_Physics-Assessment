#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

class Sphere : public Rigidbody {
public:
  Sphere(
    glm::vec2 position,
    glm::vec2 velocity,
    float mass,
    float radius,
    float elasticity,
    glm::vec4 colour)
    : Rigidbody(
        SPHERE,
        position,
        velocity,
        mass,
        elasticity,
        m_orientationRadians = 0.0f,
        0.5f * mass * radius * radius) {
    m_radius = radius;
    m_colour = colour;
  };

  ~Sphere();

  virtual void draw();

  float getRadius() { return m_radius; }
  glm::vec4 getColour() { return m_colour; }
  void setVelocity(glm::vec2 velocity) { this->m_velocity = velocity; }

protected:
  bool isCollidable = true;
  float m_radius;
  glm::vec4 m_colour;
};
