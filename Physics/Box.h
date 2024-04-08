#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

class Box : public Rigidbody {
public:
  Box(glm::vec2 position, glm::vec2 extents, glm::vec2 velocity, float mass,
      glm::vec4 colour)
      : Rigidbody(BOX, position, velocity, 0.0f, mass) {
    m_extents = extents;
    m_colour = colour;
    m_moment = 1.0f / 12.0f * m_mass * m_extents.x * m_extents.y;
  };

  ~Box();

  void fixedUpdate(glm::vec2 gravity, float timeStep);
  bool checkBoxCorners(
    const Box& box,
    glm::vec2& contact,
    int& numContacts,
    float& pen,
    glm::vec2& edgeNormal);
  virtual void draw();

  glm::vec4 getColour() { return m_colour; }
  glm::vec2 getExtents() const { return m_extents; }
  glm::vec2 getLocalX() { return m_localX; }
  glm::vec2 getLocalY() { return m_localY; }
  
  float getHeight() { return 2.0f * m_extents.y; }
  float getWidth() { return 2.0f * m_extents.x; }

protected:
  glm::vec2 m_extents; // the half-edge lengths
  glm::vec4 m_colour;

  // store the local x,y axes of the box based on its angle of rotation
  glm::vec2 m_localX{};
  glm::vec2 m_localY{};
};
