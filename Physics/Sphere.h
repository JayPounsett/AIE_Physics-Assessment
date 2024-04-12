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
    : Rigidbody(SPHERE,
                position,
                velocity,
                mass,
                elasticity,
                0.0f,
                m_moment) {
    m_radius = radius;
    m_colour = colour;
    m_moment = setMoment();
  };

  ~Sphere();

  virtual void draw() override;

  // Getters
  float getRadius() const { return m_radius; }
  glm::vec4 getColour() const { return m_colour; }

protected:
  float setMoment() {
    return m_moment = 0.5f * getMass() * getRadius() * getRadius();
  }
  float m_radius = 0.0f;
  glm::vec4 m_colour{0, 0, 0, 1};
};
