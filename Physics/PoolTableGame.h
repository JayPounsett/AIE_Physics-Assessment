#pragma once
#include <vector>
#include "Box.h"
#include "Sphere.h"
#include "Spring.h"
#include "PhysicsScene.h"

namespace AIE
{
    class Application;
};

class PoolTableGame : PhysicsObject
{
public:
  void playingPool();
  void createTable();
  void setupColouredBalls();
  void setupWhiteBall();
  void setupCue();
  void playerInput();

protected:
  PhysicsScene* m_poolTablePhysicsScene;
  std::vector<Sphere*> m_colouredBalls;
  Sphere* m_whiteBall;
  Spring* m_cueSpring;
  Box* m_cueStick;
  bool m_isSpaceDown = false;
};
