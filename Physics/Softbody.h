#pragma once
#include <string>

#include "PhysicsScene.h"

class Softbody
{
public:
  static void Build(
    PhysicsScene* scene,
    glm::vec2 position,
    float spacing,
    float springForce,
    float damping,
    std::vector<std::string>& strings);
};
