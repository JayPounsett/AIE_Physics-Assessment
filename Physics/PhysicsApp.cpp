#include "PhysicsApp.h"

#include <iostream>

#include <glm/ext.hpp>

#include "Box.h"
#include "Font.h"
#include "Gizmos.h"
#include "Input.h"
#include "Plane.h"
#include "Softbody.h"
#include "Sphere.h"
#include "Spring.h"
#include "Texture.h"

aie::Input* input;

PhysicsApp::PhysicsApp() {}

PhysicsApp::~PhysicsApp() {}

bool PhysicsApp::startup()
{
  // increase the 2D line count to maximize the number of objects we can draw
  aie::Gizmos::create(255U, 255U, 65535U, 65535U);

  m_2dRenderer = new aie::Renderer2D();

  m_font = new aie::Font("./font/consolas.ttf", 32);

  input = aie::Input::getInstance();

  m_physicsScene = new PhysicsScene();
  m_physicsScene->setTimeStep(0.01f);

  // -= Simulations =-
  // dropBallsAndCubes();
  // kinematicTest();
  // ropeTest(10);
  // softBodyTest();
  playingPool();

  return true;
}

void PhysicsApp::shutdown()
{
  delete m_font;
  delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime)
{
  aie::Gizmos::clear();

  input = aie::Input::getInstance();

  m_physicsScene->update(deltaTime);
  m_physicsScene->draw();

  // exit the application
  if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
  {
    quit();
  }

  playerInput();
}

void PhysicsApp::draw()
{
  // wipe the screen to the background m_colour
  clearScreen();

  // begin drawing sprites
  m_2dRenderer->begin();

  // demonstrate animation
  static float aspectRatio = 16 / 9.f;
  aie::Gizmos::draw2D(glm::ortho<float>(
    -100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

  // output some text, uses the last used m_colour
  m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

  // done drawing sprites
  m_2dRenderer->end();
}

// Simulations
void PhysicsApp::newtonsCradle()
{
  m_physicsScene->setGravity(glm::vec2(0));

  m_physicsScene->addActor(new Plane(glm::vec2(1, 0), -50, 0.6f));
  m_physicsScene->addActor(new Plane(glm::vec2(-1, 0), -50, 0.6f));

  // basic Newton's Cradle - 5 balls of equal mass
  m_physicsScene->addActor(new Sphere(
    glm::vec2(-50, 20),
    glm::vec2(100, 0),
    1.0f,
    5,
    0.8f,
    glm::vec4(1, 0, 0, 1)));
  m_physicsScene->addActor(new Sphere(
    glm::vec2(-15, 20), glm::vec2(0, 0), 1.0f, 5, 0.8f, glm::vec4(1, 0, 0, 1)));
  m_physicsScene->addActor(new Sphere(
    glm::vec2(-5, 20), glm::vec2(0, 0), 1.0f, 5, 0.8f, glm::vec4(1, 0, 0, 1)));
  m_physicsScene->addActor(new Sphere(
    glm::vec2(5, 20), glm::vec2(0, 0), 1.0f, 5, 0.8f, glm::vec4(1, 0, 0, 1)));
  m_physicsScene->addActor(new Sphere(
    glm::vec2(15, 20), glm::vec2(0, 0), 1.0f, 5, 0.8f, glm::vec4(1, 0, 0, 1)));
}

void PhysicsApp::projectilePhysicsNumerical()
{
  m_physicsScene = new PhysicsScene();
  m_physicsScene->setGravity(glm::vec2(0, -10));

  glm::vec2 const startPosition(-40, 0);
  float inclination = 45;
  float const radius = 1.0f;
  float const speed = 30.0f;

  float const mass = 1.0f;
  glm::vec4 const colourRed{1, 0, 0, 1};

  // Initial Velocity
  float velocityX = speed * glm::cos(glm::radians(inclination));
  float velocityY = speed * glm::sin(glm::radians(inclination));

  glm::vec2 velocity{velocityX, velocityY};

  printf("Angle: %f\n", inclination);
  printf("VelocityX: %f\n", velocityX);
  printf("VelocityX: %f\n", velocityY);

  m_physicsScene->addActor(
    new Sphere(startPosition, velocity, mass, radius, 0.8f, colourRed));
}

void PhysicsApp::dropBalls()
{
  m_physicsScene->setGravity(glm::vec2(0, -9.82f));

  Plane* plane = new Plane(glm::vec2(0, 1), -30, 0.6f);

  Sphere* ball1 = new Sphere(
    glm::vec2(-45, 20), glm::vec2(0), 10.0f, 4, 0.8f, glm::vec4(1, 0, 0, 1));

  Sphere* ball2 = new Sphere(
    glm::vec2(-45, 20), glm::vec2(0), 8.0f, 4, 0.5f, glm::vec4(0, 1, 0, 1));

  Sphere* ball3 = new Sphere(
    glm::vec2(50, 20), glm::vec2(0), 10.0f, 4, 0.8f, glm::vec4(1, 0, 0, 1));
  Sphere* ball4 = new Sphere(
    glm::vec2(51, 30), glm::vec2(0), 10.0f, 4, 0.5f, glm::vec4(0, 1, 0, 1));

  m_physicsScene->addActor(plane);
  m_physicsScene->addActor(ball1);
  m_physicsScene->addActor(ball2);
  m_physicsScene->addActor(ball3);
  m_physicsScene->addActor(ball4);
}

void PhysicsApp::dropCubes()
{
  m_physicsScene->setGravity(glm::vec2(0, -9.82f));

  Plane* plane = new Plane(glm::vec2(0, 1), -30, 0.6f);

  Box* box1 = new Box(
    glm::vec2(-5, 5),
    glm::vec2(3),
    glm::vec2(0),
    1.0f,
    30.0f,
    0.3f,
    glm::vec4(1, 1, 0, 1));

  Box* box2 = new Box(
    glm::vec2(-5, 5),
    glm::vec2(3),
    glm::vec2(0),
    1.0f,
    10.0f,
    0.3f,
    glm::vec4(1, 0, 1, 1));

  m_physicsScene->addActor(plane);
  m_physicsScene->addActor(box1);
  m_physicsScene->addActor(box2);
}

void PhysicsApp::dropBallsAndCubes()
{
  m_physicsScene->setGravity(glm::vec2(0, -9.82f));

  Plane* plane = new Plane(glm::vec2(0, 1), -30, 0.6f);

  Box* box1 = new Box(
    glm::vec2(-20, 5),
    glm::vec2(3),
    glm::vec2(0),
    10.0f,
    0.0f,
    0.3f,
    glm::vec4(1, 1, 0, 1));

  Box* box2 = new Box(
    glm::vec2(-20, 5),
    glm::vec2(3),
    glm::vec2(0),
    100.0f,
    30.0f,
    0.3f,
    glm::vec4(1, 0, 1, 1));

  Sphere* ball1 = new Sphere(
    glm::vec2(21, 5), glm::vec2(0), 5.0f, 4, 0.3f, glm::vec4(1, 0, 0, 1));

  Sphere* ball2 = new Sphere(
    glm::vec2(20, 5), glm::vec2(0), 10.0f, 4, 0.3f, glm::vec4(0, 1, 0, 1));

  Box* box3 = new Box(
    glm::vec2(0, 20),
    glm::vec2(3),
    glm::vec2(0),
    100.0f,
    0.3f,
    glm::vec4(0, 1, 1, 1));

  Sphere* ball3 = new Sphere(
    glm::vec2(0, 0), glm::vec2(0), 5.0f, 4, 0.3f, glm::vec4(0, 0, 1, 1));

  m_physicsScene->addActor(plane);
  m_physicsScene->addActor(box1);
  m_physicsScene->addActor(box2);
  m_physicsScene->addActor(box3);
  m_physicsScene->addActor(ball1);
  m_physicsScene->addActor(ball2);
  m_physicsScene->addActor(ball3);
}

void PhysicsApp::kinematicTest()
{
  m_physicsScene->setGravity(glm::vec2(0, -9.82f));

  Plane* plane = new Plane(glm::vec2(0, 1), -30, 0.6f);

  Sphere* pinball = new Sphere(
    glm::vec2(-10, 30),
    glm::vec2(0),
    100.0f,
    4.0f,
    0.8f,
    glm::vec4(1, 0, 1, 1));

  Box* bumper1 = new Box(
    glm::vec2(-10, 10),
    glm::vec2(10, 2),
    glm::vec2(0),
    1.0f,
    -25.0f,
    1.0f,
    glm::vec4(1, 1, 0, 1));

  Box* bumper2 = new Box(
    glm::vec2(15, -20),
    glm::vec2(10, 2),
    glm::vec2(0),
    1.0f,
    25.0f,
    1.0f,
    glm::vec4(1, 1, 0, 1));

  bumper1->setKinematic(true);
  bumper2->setKinematic(true);

  m_physicsScene->addActor(plane);
  m_physicsScene->addActor(pinball);
  m_physicsScene->addActor(bumper1);
  m_physicsScene->addActor(bumper2);
}

void PhysicsApp::ropeTest(int num)
{
  m_physicsScene->setGravity(glm::vec2(0, -9.82f));

  Sphere* prev = nullptr;
  for (int i = 0; i < num; i++)
  {
    // spawn a sphere to the right and below the previous one, so that the whole
    // rope acts under gravity and swings
    Sphere* sphere = new Sphere(
      glm::vec2(i * 3, 30 - i * 5),
      glm::vec2(0),
      10.0f,
      2,
      1.0f,
      glm::vec4(0, 1, 0, 1));

    if (i == 0)
    {
      sphere->setKinematic(true);
    }
    m_physicsScene->addActor(sphere);

    if (prev)
    {
      m_physicsScene->addActor(new Spring(sphere, prev, 500, 10));
    }
    prev = sphere;
  }

  // add a kinematic box at an angle for the rope to drape over
  Box* box = new Box(
    glm::vec2(0, -20),
    glm::vec2(8, 2),
    glm::vec2(0),
    20.0f,
    17.0f,
    1.0f,
    glm::vec4(0, 0, 1, 1));

  box->setKinematic(true);
  m_physicsScene->addActor(box);
}

void PhysicsApp::softBodyTest()
{
  m_physicsScene->setGravity(glm::vec2(0, -9.82f));
  Plane* plane = new Plane(glm::vec2(0, 1), -30, 0.6f);
  m_physicsScene->addActor(plane);

  std::vector<std::string> sb;
  sb.push_back("000000......000.........000...");
  sb.push_back("000000......000.........000...");
  sb.push_back("00.......000000000...000000000");
  sb.push_back("00.......000000000...000000000");
  sb.push_back("000000......000.........000...");
  sb.push_back("000000......000.........000...");
  Softbody::Build(m_physicsScene, glm::vec2(-75, 0), 5.0f, 50.0f, 50.0f, sb);
}

void PhysicsApp::playingPool()
{
  // TODO: Hold LMB and drag for ball direction
  
  // Input: Press & hold space to set the force. Release to strike ball.

  aie::Application::setBackgroundColour(0.0f, 0.25f, 0.0f);
  m_physicsScene->setGravity(glm::vec2(0));

  createTable();
  setupColouredBalls();
  setupWhiteBall();
  setupCue();
}

void PhysicsApp::createTable()
{
  // Table Edges
  Plane* tableEdgeLeft = new Plane(glm::vec2(1, 0), -95, 0.6f);
  Plane* tableEdgeRight = new Plane(glm::vec2(-1, 0), -95, 0.6f);
  Plane* tableEdgeTop = new Plane(glm::vec2(0, -1), -51, 0.6f);
  Plane* tableEdgeBottom = new Plane(glm::vec2(0, 1), -51, 0.6f);

  // 6 Ball Pockets
  Sphere* holeTopLeft = new Sphere(
    glm::vec2(-95, 50),
    glm::vec2(0),
    5000.0f, // Tested with very high mass, still moves after hit enough times yet isKinematic = true
    7.0f,
    0.0f,
    glm::vec4(0, 0, 0, 1));
  holeTopLeft->setKinematic(true);

  Sphere* holeTopMid = new Sphere(
    glm::vec2(0, 50), glm::vec2(0), 5000.0f, 7.0f, 0.0f, glm::vec4(0, 0, 0, 1));
  holeTopLeft->setKinematic(true);

  Sphere* holeTopRight = new Sphere(
    glm::vec2(95, 50),
    glm::vec2(0),
    5000.0f,
    7.0f,
    0.0f,
    glm::vec4(0, 0, 0, 1));
  holeTopLeft->setKinematic(true);

  Sphere* holeBottomLeft = new Sphere(
    glm::vec2(-95, -52),
    glm::vec2(0),
    5000.0f,
    7.0f,
    0.0f,
    glm::vec4(0, 0, 0, 1));
  holeTopLeft->setKinematic(true);

  Sphere* holeBottomMid = new Sphere(
    glm::vec2(0, -52),
    glm::vec2(0),
    5000.0f,
    7.0f,
    0.0f,
    glm::vec4(0, 0, 0, 1));
  holeTopLeft->setKinematic(true);

  Sphere* holeBottomRight = new Sphere(
    glm::vec2(95, -52), glm::vec2(0), 5000.0f, 7.0f, 0.0f, glm::vec4(0, 0, 0, 1));
  holeTopLeft->setKinematic(true);

  // Add objects to scene
  m_physicsScene->addActor(tableEdgeLeft);
  m_physicsScene->addActor(tableEdgeRight);
  m_physicsScene->addActor(tableEdgeTop);
  m_physicsScene->addActor(tableEdgeBottom);

  m_physicsScene->addActor(holeTopLeft);
  m_physicsScene->addActor(holeTopMid);
  m_physicsScene->addActor(holeTopRight);
  m_physicsScene->addActor(holeBottomLeft);
  m_physicsScene->addActor(holeBottomMid);
  m_physicsScene->addActor(holeBottomRight);
}

void PhysicsApp::setupColouredBalls()
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
  m_physicsScene->addActor(redBall);
  m_physicsScene->addActor(greenBall);
  m_physicsScene->addActor(blueBall);
  m_physicsScene->addActor(yellowBall);
  m_physicsScene->addActor(cyanBall);
  m_physicsScene->addActor(magentaBall);
}

void PhysicsApp::setupWhiteBall()
{
  m_whiteBall = new Sphere(
    glm::vec2(50, 0), glm::vec2(0), 1.0f, 4.0f, 0.8f, glm::vec4(1, 1, 1, 1));

  m_whiteBall->setKinematic(true);
  m_physicsScene->addActor(m_whiteBall);
}

void PhysicsApp::setupCue()
{
  m_cue = new Box(
    glm::vec2(60, 0),
    glm::vec2(5, 1),
    glm::vec2(0, 0),
    5.0f,
    0.0f,
    0.1f,
    glm::vec4(1, 1, 1, 1));

  m_cue->isKinematic(true);

  m_cueSpring = new Spring(m_cue, m_whiteBall, 70, 7);

  m_physicsScene->addActor(m_cue);
  m_physicsScene->addActor(m_cueSpring);
}

void PhysicsApp::playerInput()
{
  // If spring is attached to cue and space is pressed
  if (input->isKeyDown(aie::INPUT_KEY_SPACE))
  {
    m_isSpaceDown = true;
    m_cue->applyForce(glm::vec2(10, 0), m_cue->getPosition());
  }

  if (m_isSpaceDown && input->isKeyUp(aie::INPUT_KEY_SPACE))
  {
    if (glm::distance(m_whiteBall->getPosition(), m_cue->getPosition()) <
      11.0f)
    {
      m_whiteBall->setKinematic(false);
      m_physicsScene->removeActor(m_cueSpring);

      //m_cue->setKinematic(true);
      //m_physicsScene->removeActor(m_cue);
      //setupCue();
    }
  }
}
