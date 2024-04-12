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
  void setupContinuousDemo(
    glm::vec2 startPosition, float inclination, float speed, float gravity);
  void projectilePhysicsNumerical();
  void dropBalls();
  void newtonsCradle();
  void dropCubes();
  void dropBallsAndCubes();
  

protected:
  aie::Renderer2D* m_2dRenderer = nullptr;
  aie::Texture* m_texture = nullptr;
  aie::Texture* m_shipTexture = nullptr;
  aie::Font* m_font = nullptr;

  PhysicsScene* m_physicsScene = nullptr;

  float m_timer = 0;
};
