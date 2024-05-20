#pragma once

#include <string>

#include <glm/glm.hpp>

#include "Application.h"
#include "PhysicsScene.h"
#include "Renderer2D.h"
#include "Sphere.h"

class Box;
class Plane;

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

  // Pool Table Game
  void setupPoolTableGame(); // All functions to create table, place balls, etc

protected:
  aie::Renderer2D* m_2dRenderer;
  aie::Texture* m_texture;
  aie::Texture* m_shipTexture;
  aie::Font* m_font;

  PhysicsScene* m_physicsScene;

  float m_timer = 0;

  // Pool Table Game Functions
protected:
  // Setting up the player inputs
  void playerInput(float deltaTime);
  void drawScoreBoard();
  void drawCueAimLine();
  void updateScore();

  // Pool Table Game Member Variables
protected:
  // Global Variables
  std::string m_scoreBoardScore;
  int m_score = 0;
  const glm::vec2 m_zeroVelocity = glm::vec2(0, 0);

  // Colours
  const glm::vec4 m_colourWhite = glm::vec4(1, 1, 1, 1);
  const glm::vec4 m_colourBlack = glm::vec4(0, 0, 0, 1); // Ball Pockets
  const glm::vec4 m_colourBrown =
    glm::vec4(0.55, 0.27, 0.07, 1); // Cuestick, saddlebrown
  const glm::vec4 m_colourRed = glm::vec4(1, 0, 0, 1);
  const glm::vec4 m_colourGreen = glm::vec4(0, 1, 0, 1);
  const glm::vec4 m_colourBlue = glm::vec4(0, 0, 1, 1);
  const glm::vec4 m_colourYellow = glm::vec4(1, 1, 0, 1);
  const glm::vec4 m_colourCyan = glm::vec4(0, 1, 1, 1);
  const glm::vec4 m_colourMagenta = glm::vec4(1, 0, 1, 1);

  // Cuestick
  Box* m_cue;
  glm::vec2 m_cuePosition{};
  glm::vec2 m_cueOffset; // Offset of cue from white ball
  glm::vec2 m_cueOffTablePosition = glm::vec2(
    200, 200); // Location to put cue off the table when balls are in motion
  
  glm::vec2 m_cueExtents = glm::vec2(20, 1); // Size of cue stick
  const float m_cueMass = 5.0f;
  const float m_cueAngle = 0.0f;
  const float m_cueElasticity = 0.1f;
  const float m_cueMaxDistance = 100.0f;

  const glm::vec2 m_cueMaxVelocity = glm::vec2(10, 10);
  float m_cueActualDistance = 0.0f;
  float m_cueRotationAngle = 0.0f;
  
  // Ball Variables
  const float m_ballMass = 1.0f;
  const float m_ballRadius = 4.0f;
  const float m_ballElasticity = 0.4f;

  // Ball Start Position Constants
  const glm::vec2 m_whiteBallStartPos = glm::vec2(50, 0);
  const glm::vec2 m_redBallStartPos = glm::vec2(-40, 0);
  const glm::vec2 m_greenBallStartPos = glm::vec2(-48, 5);
  const glm::vec2 m_blueBallStartPos = glm::vec2(-48, -5);
  const glm::vec2 m_yellowBallStartPos = glm::vec2(-56, 10);
  const glm::vec2 m_cyanBallStartPos = glm::vec2(-56, -0);
  const glm::vec2 m_magentaBallStartPos = glm::vec2(-56, -10);

  // Balls
  std::vector<Sphere*> m_balls;
  Sphere* m_whiteBall;
  Sphere* m_redBall;
  Sphere* m_greenBall;
  Sphere* m_blueBall;
  Sphere* m_yellowBall;
  Sphere* m_cyanBall;
  Sphere* m_magentaBall;

  // Pool Table Boundaries
  Plane* m_tableEdgeLeft;
  Plane* m_tableEdgeRight;
  Plane* m_tableEdgeTop;
  Plane* m_tableEdgeBottom;

  // Pocket Variables
  std::vector<Sphere*> m_pockets;
  const glm::vec2 m_pocketTopLeftPosition = glm::vec2(-95, 52);
  const glm::vec2 m_pocketTopMidPosition = glm::vec2(0, 52);
  const glm::vec2 m_pocketTopRightPosition = glm::vec2(95, 52);
  const glm::vec2 m_pocketBottomLeftPosition = glm::vec2(-95, -52);
  const glm::vec2 m_pocketBottomMidPosition = glm::vec2(0, -52);
  const glm::vec2 m_pocketBottomRightPosition = glm::vec2(95, -52);
  const float m_pocketElasticity = 0.0f;
  const float m_pocketMass = 100.0f;
  const float m_pocketRadius = 7.0f;

  // Ball Pockets
  Sphere* m_pocketTopLeft = new Sphere(
    m_pocketTopLeftPosition,
    m_zeroVelocity,
    m_pocketMass,
    m_pocketRadius,
    m_pocketElasticity,
    m_colourBlack);
  Sphere* m_pocketTopMid = new Sphere(
    m_pocketTopMidPosition,
    m_zeroVelocity,
    m_pocketMass,
    m_pocketRadius,
    m_pocketElasticity,
    m_colourBlack);
  Sphere* m_pocketTopRight = new Sphere(
    m_pocketTopRightPosition,
    m_zeroVelocity,
    m_pocketMass,
    m_pocketRadius,
    m_pocketElasticity,
    m_colourBlack);
  ;
  Sphere* m_pocketBottomLeft = new Sphere(
    m_pocketBottomLeftPosition,
    m_zeroVelocity,
    m_pocketMass,
    m_pocketRadius,
    m_pocketElasticity,
    m_colourBlack);
  Sphere* m_pocketBottomMid = new Sphere(
    m_pocketBottomMidPosition,
    m_zeroVelocity,
    m_pocketMass,
    m_pocketRadius,
    m_pocketElasticity,
    m_colourBlack);
  Sphere* m_pocketBottomRight = new Sphere(
    m_pocketBottomRightPosition,
    m_zeroVelocity,
    m_pocketMass,
    m_pocketRadius,
    m_pocketElasticity,
    m_colourBlack);
};
