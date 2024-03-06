#pragma once
#include "PhysicsObject.h"
#include "glm/glm.hpp"

class Plane : public PhysicsObject
{
public:
  Plane(glm::vec2 normal, float distance, glm::vec4 colour);
  Plane() : PhysicsObject(ShapeType::PLANE)
  {
    this->distanceToOrigin = distanceToOrigin;
    this->normal = normal;
  }
  ~Plane();

  virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
  virtual void Draw();
  virtual void ResetPosition();

  glm::vec2 GetNormal() { return normal; }
  float GetDistance() { return distanceToOrigin; }

protected:
  glm::vec4 colour = {0, 0, 0, 0};
  glm::vec2 normal = glm::vec2(0,1);
  float distanceToOrigin = 0.f;
};
