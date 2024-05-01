#include "PoolTableGame.h"
#include "Application.h"
#include "Plane.h"

aie::Application* instance;

void PoolTableGame::playingPool()
{
  // Hold LMB and drag for ball direction
  // Press & hold space to set the force. Release to strike ball.

  instance->setBackgroundColour(0.0f, 0.25f, 0.0f);
  m_poolTablePhysicsScene->setGravity(glm::vec2(0));

  createTable();
  setupColouredBalls();
  setupWhiteBall();
  setupCue();
}

void PoolTableGame::createTable()
{
  // Table Edges
  Plane* tableEdgeLeft = new Plane(glm::vec2(1, 0), -95, 0.6f);
  Plane* tableEdgeRight = new Plane(glm::vec2(-1, 0), -95, 0.6f);
  Plane* tableEdgeTop = new Plane(glm::vec2(0, -1), -51, 0.6f);
  Plane* tableEdgeBottom = new Plane(glm::vec2(0, 1), -51, 0.6f);

  // 6 Ball Pockets
  Sphere* holeTopLeft = new Sphere(
    glm::vec2(-95, 50), glm::vec2(0), 5.0f, 7.0f, 0.0f, glm::vec4(0, 0, 0, 1));
  holeTopLeft->setKinematic(true);

  Sphere* holeTopMid = new Sphere(
    glm::vec2(0, 50), glm::vec2(0), 5.0f, 7.0f, 0.0f, glm::vec4(0, 0, 0, 1));
  holeTopLeft->setKinematic(true);

  Sphere* holeTopRight = new Sphere(
    glm::vec2(95, 50), glm::vec2(0), 5.0f, 7.0f, 0.0f, glm::vec4(0, 0, 0, 1));
  holeTopLeft->setKinematic(true);

  Sphere* holeBottomLeft = new Sphere(
    glm::vec2(-95, -52), glm::vec2(0), 5.0f, 7.0f, 0.0f, glm::vec4(0, 0, 0, 1));
  holeTopLeft->setKinematic(true);

  Sphere* holeBottomMid = new Sphere(
    glm::vec2(0, -52), glm::vec2(0), 5.0f, 7.0f, 0.0f, glm::vec4(0, 0, 0, 1));
  holeTopLeft->setKinematic(true);

  Sphere* holeBottomRight = new Sphere(
    glm::vec2(95, -52), glm::vec2(0), 5.0f, 7.0f, 0.0f, glm::vec4(0, 0, 0, 1));
  holeTopLeft->setKinematic(true);

  // Add objects to scene
  m_poolTablePhysicsScene->addActor(tableEdgeLeft);
  m_poolTablePhysicsScene->addActor(tableEdgeRight);
  m_poolTablePhysicsScene->addActor(tableEdgeTop);
  m_poolTablePhysicsScene->addActor(tableEdgeBottom);

  m_poolTablePhysicsScene->addActor(holeTopLeft);
  m_poolTablePhysicsScene->addActor(holeTopMid);
  m_poolTablePhysicsScene->addActor(holeTopRight);
  m_poolTablePhysicsScene->addActor(holeBottomLeft);
  m_poolTablePhysicsScene->addActor(holeBottomMid);
  m_poolTablePhysicsScene->addActor(holeBottomRight);
}

void PoolTableGame::setupColouredBalls()
{
  Sphere* redBall = new Sphere(
    glm::vec2(-40, 0), glm::vec2(0), 1.0f, 4.0f, 0.8f, glm::vec4(1, 0, 0, 1));

  Sphere* greenBall = new Sphere(
    glm::vec2(-48, 5), glm::vec2(0), 1.0f, 4.0f, 0.8f, glm::vec4(0, 1, 0, 1));

  Sphere* blueBall = new Sphere(
    glm::vec2(-48, -5), glm::vec2(0), 1.0f, 4.0f, 0.8f, glm::vec4(0, 0, 1, 1));

  Sphere* yellowBall = new Sphere(
    glm::vec2(-56, 10), glm::vec2(0), 1.0f, 4.0f, 0.8f, glm::vec4(1, 1, 0, 1));

  Sphere* cyanBall = new Sphere(
    glm::vec2(-56, 0), glm::vec2(0), 1.0f, 4.0f, 0.8f, glm::vec4(1, 0, 1, 1));

  Sphere* magentaBall = new Sphere(
    glm::vec2(-56, -10), glm::vec2(0), 1.0f, 4.0f, 0.8f, glm::vec4(0, 1, 1, 1));

  // Add coloured balls to a vector
  m_colouredBalls.push_back(redBall);
  m_colouredBalls.push_back(greenBall);
  m_colouredBalls.push_back(blueBall);
  m_colouredBalls.push_back(yellowBall);
  m_colouredBalls.push_back(cyanBall);
  m_colouredBalls.push_back(magentaBall);

  // Add objects to scene
  m_poolTablePhysicsScene->addActor(redBall);
  m_poolTablePhysicsScene->addActor(greenBall);
  m_poolTablePhysicsScene->addActor(blueBall);
  m_poolTablePhysicsScene->addActor(yellowBall);
  m_poolTablePhysicsScene->addActor(cyanBall);
  m_poolTablePhysicsScene->addActor(magentaBall);
}

void PoolTableGame::setupWhiteBall()
{
  m_whiteBall = new Sphere(
    glm::vec2(50, 0), glm::vec2(0), 1.0f, 4.0f, 0.8f, glm::vec4(1, 1, 1, 1));

  m_whiteBall->setKinematic(true);
  m_poolTablePhysicsScene->addActor(m_whiteBall);
}

void PoolTableGame::setupCue()
{
  m_cueStick = new Box(
    glm::vec2(60, 0),
    glm::vec2(5, 1),
    glm::vec2(0, 0),
    5.0f,
    0.0f,
    0.1f,
    glm::vec4(1, 1, 1, 1));

  m_cueStick->isKinematic(true);

  m_cueSpring = new Spring(m_cueStick, m_whiteBall, 70, 7);

  m_poolTablePhysicsScene->addActor(m_cueStick);
  m_poolTablePhysicsScene->addActor(m_cueSpring);
}

void PoolTableGame::playerInput()
{
  // If spring is attached to cue and space is pressed
  if (input->isKeyDown(aie::INPUT_KEY_SPACE))
  {
    m_isSpaceDown = true;
    m_cueStick->applyForce(glm::vec2(10, 0), m_cueStick->getPosition());
  }

  if (m_isSpaceDown && input->isKeyUp(aie::INPUT_KEY_SPACE))
  {
    if (glm::distance(m_whiteBall->getPosition(), m_cueStick->getPosition()) < 11.0f)
    {
      m_whiteBall->setKinematic(false);
      m_poolTablePhysicsScene->removeActor(m_cueSpring);

      // m_cue->setKinematic(true);
      // m_physicsScene->removeActor(m_cue);
      // setupCue();
    }
  }
}
