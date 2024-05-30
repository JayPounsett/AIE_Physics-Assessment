#pragma once

#include <string>

#include <glm/glm.hpp>

#include "Application.h"
#include "PhysicsScene.h"
#include "Renderer2D.h"
#include "Sphere.h"

class Box;
class Plane;
class PoolTableGame;

class PhysicsApp : public aie::Application
{
public:
  PhysicsApp();
  virtual ~PhysicsApp();

  virtual bool startup();
  virtual void shutdown();

  virtual void update(float deltaTime);
  virtual void draw();

  // Simulations
  void projectilePhysicsNumerical();
  void dropBalls();
  void newtonsCradle();
  void dropCubes();
  void dropBallsAndCubes();
  void kinematicTest();
  void ropeTest(int num);
  void softBodyTest();

protected:
  aie::Renderer2D* m_2dRenderer;
  aie::Texture* m_texture;
  aie::Texture* m_shipTexture;
  aie::Font* m_font;

  PhysicsScene* m_physicsScene;
  PoolTableGame* m_poolTableGame;

  float m_timer = 0;
};
