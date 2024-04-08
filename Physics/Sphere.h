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
    float orientation,
    glm::vec4 colour)
    : Rigidbody(
        SPHERE,
        position,
        velocity,
        mass,
        orientation,
        m_moment) {
    m_radius = radius;
    m_moment = 0.5f * m_mass * radius * radius;
    m_colour = colour;
  };

  ~Sphere();

  virtual void draw();

  float getRadius() { return m_radius; }
  glm::vec4 getColour() { return m_colour; }
  glm::vec2 setVelocity(glm::vec2 velocity) { this->m_velocity = velocity; }

protected:
  float m_radius;
  glm::vec4 m_colour;
};
