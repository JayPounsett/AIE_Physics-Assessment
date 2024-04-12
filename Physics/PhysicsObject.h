#pragma once
#include <glm/vec2.hpp>

enum ShapeType
{
  PLANE = 0,
  SPHERE = 1,
  BOX = 2,
  SHAPE_COUNT = 3
};

class PhysicsObject
{
protected:
  PhysicsObject(ShapeType a_shapeID, float a_elasticity = 1.0f) : m_shapeID(a_shapeID), m_elasticity(a_elasticity) {}

public:
  virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
  virtual void draw() = 0;
  virtual void resetPosition() {}
  int getShapeID() const { return m_shapeID; }
  float getElasticity() const { return m_elasticity; }

protected:
  ShapeType m_shapeID;
  float m_elasticity;
};
