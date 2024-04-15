#pragma once

#include "Application.h"
#include "PhysicsScene.h"
#include "Renderer2D.h"

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
  

protected:
  aie::Renderer2D* m_2dRenderer;
  aie::Texture* m_texture;
  aie::Texture* m_shipTexture;
  aie::Font* m_font;

  PhysicsScene* m_physicsScene;

  float m_timer = 0;
};
