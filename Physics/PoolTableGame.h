#pragma once

#include <Font.h>
#include <Input.h>
#include <Renderer2D.h>

#include <string>
#include <vector>

#include "Box.h"
#include "PhysicsScene.h"
#include "Plane.h"
#include "Sphere.h"

class PhysicsScene;

class PoolTableGame
{
public:
  PoolTableGame(PhysicsScene* poolTableScene, float timeStep, glm::vec2 gravity)
  {
    m_poolTableScene = poolTableScene;
    m_timeStep = timeStep;
    m_gravity = gravity;

    m_poolTableScene->setTimeStep(m_timeStep);
    m_poolTableScene->setGravity(m_gravity);
  }

  ~PoolTableGame();

  void update(float deltaTime, aie::Input* input);
  void draw(aie::Renderer2D* renderer, aie::Font* font);

  // Pool Table Game
  void setupPoolTableGame(
    PhysicsScene*
      physicsScene); // All functions to create table, place balls, etc

  glm::vec2 getPosition() { return this->m_cuePosition; }

  glm::mat4 getCueMatrix()
  {
    this->updateCueVectors();

    glm::vec3 positionVec3 =
      glm::vec3(this->getCueTipPosition().x, this->getCueTipPosition().y, 0.0f);

    this->m_cueMatrix = glm::lookAt(
      positionVec3, positionVec3 + this->m_front, glm::vec3(0, 0, 1));

    return this->m_cueMatrix;
  }

  // Getters
  Box* getCue() { return m_cue; }

  bool hasBallVelocity()
  {
    for (auto ball : m_balls)
    {
      if (
        glm::abs(ball->getVelocity().x) > 0.5f ||
        glm::abs(ball->getVelocity().y) > 0.5f)
      {
        return true;
      }
    }
    return false;
  }

  glm::vec2 getCueTipPosition()
  {
    return m_cueTipPosition = glm::vec2(
             this->getPosition().x - m_cueExtents.x, this->getPosition().y);
  }

  // Setting up the player inputs
  // void playerInput(float deltaTime, aie::Input* input);
  void drawScoreBoard(aie::Renderer2D* renderer, aie::Font* font);
  void updateScore();
  // void rotateCue(); // Using A & D to rotate the cue around the white ball

protected:
  void updateCueVectors()
  {
    // Store front vector
    this->m_front =
      glm::vec3(getCue()->getFacing().x, getCue()->getFacing().y, 0.0f);
    this->m_cuePosition =
      glm::vec2(getCueTipPosition().x, getCueTipPosition().y);
  };

  void updateInput(const float& deltaTime, aie::Input* input)
  {
    this->updateKeyboardInput(deltaTime, input);
  };

  void updateKeyboardInput(const float& deltaTime, aie::Input* input)
  {
    if (this->m_cueAngle > 360.0f || this->m_cueAngle < -360.0f)
    {
      this->m_cueAngle = 0.0f;
    }

    if (input->isKeyDown(aie::INPUT_KEY_W))
    {
      // Check if the cue is at the ball, stop input if it is
      if (
        m_cue->getPosition().x <=
        (m_whiteBall->getPosition().x + m_ballRadius) + m_cueExtents.x)
      {
        return;
      }

      m_cuePosition = m_cue->getPosition();

      m_cuePosition +=
        glm::vec2(getCue()->getFacing().x, getCue()->getFacing().y) * m_speed *
        deltaTime;
      m_cue->setPosition(m_cuePosition);
    }
    if (input->isKeyDown(aie::INPUT_KEY_S))
    {
      // Check if the cue is at it's max distance, stop input if it is
      if (
        m_cue->getPosition().x >=
        (m_whiteBall->getPosition().x + m_ballRadius) + m_cueExtents.x +
          m_cueMaxDistance)
      {
        return;
      }

      m_cuePosition = m_cue->getPosition();

      m_cuePosition -=
        glm::vec2(getCue()->getFacing().x, getCue()->getFacing().y) * m_speed *
        deltaTime;
      m_cue->setPosition(m_cuePosition);
    }

    if (input->isKeyDown(aie::INPUT_KEY_A))
    {
      m_cueAngle += 1 * this->m_speed * deltaTime;
      m_cue->setOrientation(m_cueAngle);
      setFacing();
    }
    if (input->isKeyDown(aie::INPUT_KEY_D))
    {
      m_cueAngle -= 1 * this->m_speed * deltaTime;
      m_cue->setOrientation(m_cueAngle);
      setFacing();
    }

    if (input->isKeyDown(aie::INPUT_KEY_SPACE))
    {
      m_cuePosition = m_cue->getPosition();
      glm::vec2 whiteBallPos = m_whiteBall->getPosition();

      glm::vec2 cueVelocity = glm::vec2(m_cuePosition - whiteBallPos);

      m_cue->setVelocity(-cueVelocity * 2.0f);

      /*m_cue->applyForce(
        -m_cuePosition,
        whiteBallPos);*/
    }
  };

protected:
  glm::mat4 setFacing()
  {
    // Cue to always face the white ball
    glm::vec3 cuePosition =
      glm::vec3(this->getPosition().x, this->getPosition().y, 0.0f);

    glm::vec3 whiteBallPosition = glm::vec3(
      m_whiteBall->getPosition().x, m_whiteBall->getPosition().y, 0.0f);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // up vector for the cue

    // TODO: Get angle between the vectors
    // Rotating left and right around white ball
    // - Set position
    //- Calculate angle between cue and ball using vector & dot product.
    //	- Always do it against the x-axis for simplicity.
    //		1. Grab vector from cuePosition to whiteBallPosition.
    //		2. Normalise the vector.
    //		3. angleDegrees = glm::degrees(-Cos(dot product))
    //		4. m_cue->setOrientation(angleDegrees)

    return m_cueMatrix = glm::lookAt(cuePosition, whiteBallPosition, up);
  };

protected:
  bool isCueOnTable = false;

  glm::mat3 m_cueMatrix = glm::mat3(1.0f);
  glm::vec3 m_front = glm::vec3(0.0f);
  glm::vec3 m_right = glm::vec3(0.0f);
  float m_speed = 20.0f;
  // float m_sensitivity = 10.0f; // Mouse Sensitivity

  // Scene Variables
  PhysicsScene* m_poolTableScene = new PhysicsScene();
  glm::vec2 m_gravity = glm::vec2(0, 0);
  float m_timeStep = 0.01f;

  // Global Variables
  std::string m_scoreBoardScore;
  int m_score = 0;
  const glm::vec2 m_zeroVelocity = glm::vec2(0, 0);

  // Cuestick
  Box* m_cue = nullptr;
  glm::vec2 m_cuePosition{};
  glm::vec2 m_cueTipPosition{};
  glm::vec2 m_cueOffset{}; // Offset of cue from white ball
  glm::vec2 m_cueOffTablePosition = glm::vec2(
    200, 200); // Location to put cue off the table when balls are in motion

  glm::vec2 m_cueExtents = glm::vec2(20, 1); // Size of cue stick
  float m_cueAngle = 0.0f;
  const float m_cueMass = 5.0f;
  const float m_cueElasticity = 0.1f;
  const float m_cueMaxDistance = 20.0f;
  const glm::vec2 m_cueMaxVelocity = glm::vec2(10, 10);
  float m_cueActualDistance = 0.0f;

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
  Sphere* m_whiteBall{};
  Sphere* m_redBall{};
  Sphere* m_greenBall{};
  Sphere* m_blueBall{};
  Sphere* m_yellowBall{};
  Sphere* m_cyanBall{};
  Sphere* m_magentaBall{};

  // Pool Table Boundaries
  Plane* m_tableEdgeLeft{};
  Plane* m_tableEdgeRight{};
  Plane* m_tableEdgeTop{};
  Plane* m_tableEdgeBottom{};

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
  Sphere* m_pocketTopLeft{};
  Sphere* m_pocketTopMid{};
  Sphere* m_pocketTopRight{};
  Sphere* m_pocketBottomLeft{};
  Sphere* m_pocketBottomMid{};
  Sphere* m_pocketBottomRight{};

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
};
