#include "PhysicsApp.h"

#include <iostream>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "Box.h"
#include "Font.h"
#include "Gizmos.h"
#include "Input.h"
#include "Plane.h"
#include "PoolTableGame.h"
#include "Softbody.h"
#include "Sphere.h"
#include "Spring.h"
#include "Texture.h"

aie::Input* input;

PhysicsApp::PhysicsApp() {}

PhysicsApp::~PhysicsApp() {}

/* TODO Pool Table Game Notes from Open Room Discussion
 * W brings it back towards the ball (fine control)
 * S increases distance between cue and ball
 * A/D rotate around ball
 * Spacebar: max velocity stored (maxV), a maxDist, actualDist... (ie. MaxV *
 actualDist/maxDist)

 * Cue has max velocity that can be reached
 * Move cue off table until all balls velocity = ~0, move cue back to the white
 ball.
 * Rotating around ball should be something like cos angle, sin angle (unit
 circle maths)

 * If sink white ball, wait for ball velocity = 0, set it up at starting
 position with cue.

 * Scoreboard, text object that iterates when ball sunk (or removes a point
 for white ball).

 * When cue off table, disable input? Bring it back on the table when all balls
 are at 0 velocity.
 */

bool PhysicsApp::startup()
{
  // increase the 2D line count to maximize the number of objects we can draw
  aie::Gizmos::create(255U, 255U, 65535U, 65535U);

  m_2dRenderer = new aie::Renderer2D();

  m_font = new aie::Font("../bin/font/consolas.ttf", 32);

  input = aie::Input::getInstance();

  // Set background colour
  aie::Application::setBackgroundColour(0.0f, 0.25f, 0.0f);

  // Create physics scene and set gravity/timestep
  m_physicsScene = new PhysicsScene();
  m_physicsScene->setTimeStep(0.01f);
  m_physicsScene->setGravity(glm::vec2(0));


  // -= Simulations =-
  // dropBallsAndCubes();
  // kinematicTest();
  // ropeTest(10);
  // softBodyTest();


  // Create Pool Table Game
  m_poolTableGame = new PoolTableGame(m_physicsScene, 0.01f, glm::vec2(0));

  // Setup Pool Table Game
  m_poolTableGame->setupPoolTableGame(m_physicsScene);

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

  m_poolTableGame->update(deltaTime, input);

  // exit the application
  if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
  {
    quit();
  }
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

    m_poolTableGame->draw(m_2dRenderer, m_font);

  // done drawing sprites
  m_2dRenderer->end();
}

#pragma region Simulations
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
#pragma endregion Simulations
