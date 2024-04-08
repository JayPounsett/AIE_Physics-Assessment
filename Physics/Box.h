#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

class Box : public Rigidbody {
public:
  Box(
    glm::vec2 position,
    glm::vec2 extents,
    glm::vec2 velocity,
    float mass,
    float angle,
    glm::vec4 colour)
    : Rigidbody(BOX, position, velocity, mass, m_orientation, m_moment) {
    m_extents = extents;
    m_colour = colour;
    m_moment = 1.0f / 12.0f * m_mass * m_extents.x * m_extents.y;
    m_orientation = setOrientation(angle);
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

  glm::vec4 getColour() const { return m_colour; }
  glm::vec2 getExtents() const { return m_extents; }
  glm::vec2 getLocalX() const { return m_localX; }
  glm::vec2 getLocalY() const { return m_localY; }

  float getMoment() { return m_moment; }

  float getHeight() { return 2.0f * m_extents.y; }
  float getWidth() { return 2.0f * m_extents.x; }

protected:
  float setOrientation(float angle) {
    return m_orientation = angle * glm::pi<float>() / 180;
  }

protected:
  glm::vec2 m_extents; // the half-edge lengths
  glm::vec4 m_colour;

  // store the local x,y axes of the box based on its angle of rotation
  glm::vec2 m_localX{};
  glm::vec2 m_localY{};
};
