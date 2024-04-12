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
    float elasticity,
    glm::vec4 colour)
    : Rigidbody(BOX,
                position,
                velocity,
                mass,
                elasticity,
                0.0f,
                m_moment) {
    m_extents = extents;
    m_colour = colour;
    m_moment = setMoment();
  };

  ~Box();

  void fixedUpdate(glm::vec2 gravity, float timeStep);

  virtual void draw();

  bool checkBoxCorners(
    const Box& box,
    glm::vec2& contact,
    int& numContacts,
    float& pen,
    glm::vec2& edgeNormal);

  // Getters
  glm::vec4 getColour() const { return m_colour; }
  float getElasticity() const { return m_elasticity; }
  glm::vec2 getExtents() const { return m_extents; }
  glm::vec2 getLocalX() const { return m_localX; }
  glm::vec2 getLocalY() const { return m_localY; }
  float getOrientation() const { return m_orientation; }
  glm::vec2 getPosition() const { return m_position; }

  float getHeight() const { return 2.0f * m_extents.y; }
  float getWidth() const { return 2.0f * m_extents.x; }

  // Setters
  float setOrientation(float angle) {
    return m_orientation = angle * glm::pi<float>() / 180;
  }

  float setMoment() { return m_moment = 1.0f / 12.0f * m_mass * getWidth() * getHeight(); }

protected:
  glm::vec2 m_extents{}; // the half-edge lengths
  glm::vec4 m_colour{};
  float m_orientation = 0.0f;

  // store the local x,y axes of the box based on its angle of rotation
  glm::vec2 m_localX{};
  glm::vec2 m_localY{};
};
