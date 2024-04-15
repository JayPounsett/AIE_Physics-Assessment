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
    float elasticity,
    glm::vec4 colour,
    bool isKinematic = false)
    : Rigidbody(
        BOX, position, velocity, mass, elasticity, m_orientation, m_moment, isKinematic) {
    m_extents = extents;
    m_colour = colour;
    m_orientation = setOrientation(angle);
    m_moment = setMoment(mass, extents.x, extents.y);
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
  glm::vec2 getPosition() const { return m_position; }

  float getMoment() { return m_moment; }

  float getHeight() { return 2.0f * m_extents.y; }
  float getWidth() { return 2.0f * m_extents.x; }

protected:
  float setOrientation(float angle) { return angle * glm::pi<float>() / 180; }

  float setMoment(float mass, float x, float y) {
    return 1.0f / 12.0f * mass * (2 * x) * (2 * y);
  }

protected:
  glm::vec2 m_extents; // the half-edge lengths
  glm::vec4 m_colour;


  // store the local x,y axes of the box based on its angle of rotation
  glm::vec2 m_localX{};
  glm::vec2 m_localY{};
};
