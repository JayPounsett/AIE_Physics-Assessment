#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

class Box : public Rigidbody
{
public:
  Box(
    glm::vec2 position,
    glm::vec2 extents,
    glm::vec2 velocity,
    float mass,
    glm::vec4 colour)
    : Rigidbody(BOX, position, velocity, 0, mass)
  {
    m_extents = extents;
    m_colour = colour;
  };

  ~Box();

  virtual void draw();

  glm::vec4 getColour() { return m_colour; }
  glm::vec2 getExtents() { return m_extents; };
  
  //TODO: Add rotation to add to the draw function
  float setRotation(float rotation) { m_orientation = rotation; }
  const glm::mat4 getRotation()
  {
    return glm::mat4(
      1.0f,
      0.0f,
      0.0f,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      0.0f,
      0.0f,
      m_orientation);
  }
  glm::vec2 setVelocity(glm::vec2 velocity) { this->m_velocity = velocity; }

protected:
  glm::vec4 m_colour;
  glm::vec2 m_extents;
  float m_orientation;
};
