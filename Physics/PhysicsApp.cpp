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

  m_font = new aie::Font("../bin/font/consolas.ttf", 32);

  input = aie::Input::getInstance();

  m_physicsScene = new PhysicsScene();
  m_physicsScene->setTimeStep(0.01f);

  // -= Simulations =-
  // dropBallsAndCubes();
  // kinematicTest();
  // ropeTest(10);
  // softBodyTest();
  setupPoolTableGame();

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

  playPoolTableGame();

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

  // output 'some text, uses the last used m_colour
  m_2dRenderer->drawText(m_font, "Press ESC to quit", 180, 5);

  drawScoreBoard();

  // done drawing sprites
  m_2dRenderer->end();
}

void PhysicsApp::setupPoolTableGame()
{
  aie::Application::setBackgroundColour(0.0f, 0.25f, 0.0f);
  m_physicsScene->setGravity(glm::vec2(0));

  createTable();
  setupColouredBalls();
  setupWhiteBall();
  setupCue();
}

void PhysicsApp::playPoolTableGame() { 
    playerInput();
    
}

void PhysicsApp::createTable()
{
  // Table Edges
  m_tableEdgeLeft = new Plane(glm::vec2(1, 0), -95, 0.4f);
  m_tableEdgeRight = new Plane(glm::vec2(-1, 0), -95, 0.4f);
  m_tableEdgeTop = new Plane(glm::vec2(0, -1), -51, 0.4f);
  m_tableEdgeBottom = new Plane(glm::vec2(0, 1), -51, 0.4f);

  // Set 6 ball pockets to kinematic
  m_pocketTopMid->setKinematic(true);
  m_pocketTopRight->setKinematic(true);
  m_pocketBottomLeft->setKinematic(true);
  m_pocketBottomMid->setKinematic(true);
  m_pocketBottomRight->setKinematic(true);

  // Add objects to scene
  m_physicsScene->addActor(m_tableEdgeLeft);
  m_physicsScene->addActor(m_tableEdgeRight);
  m_physicsScene->addActor(m_tableEdgeTop);
  m_physicsScene->addActor(m_tableEdgeBottom);
  m_physicsScene->addActor(m_pocketTopLeft);
  m_physicsScene->addActor(m_pocketTopMid);
  m_physicsScene->addActor(m_pocketTopRight);
  m_physicsScene->addActor(m_pocketBottomLeft);
  m_physicsScene->addActor(m_pocketBottomMid);
  m_physicsScene->addActor(m_pocketBottomRight);
}

void PhysicsApp::setupWhiteBall()
{
  m_whiteBall = new Sphere(
    m_whiteBallStartPos,
    m_zeroVelocity,
    m_ballMass,
    m_ballRadius,
    m_ballElasticity,
    m_colourWhite);

  m_cuePosition =
    m_whiteBallStartPos + glm::vec2(m_ballRadius + m_cueExtents.x, 0);

  m_whiteBall->setKinematic(true);
  m_physicsScene->addActor(m_whiteBall);
}

void PhysicsApp::setupColouredBalls()
{
  m_redBall = new Sphere(
    m_redBallStartPos,
    m_zeroVelocity,
    m_ballMass,
    m_ballRadius,
    m_ballElasticity,
    m_colourRed);

  m_greenBall = new Sphere(
    m_greenBallStartPos,
    m_zeroVelocity,
    m_ballMass,
    m_ballRadius,
    m_ballElasticity,
    m_colourGreen);

  m_blueBall = new Sphere(
    m_blueBallStartPos,
    m_zeroVelocity,
    m_ballMass,
    m_ballRadius,
    m_ballElasticity,
    m_colourBlue);

  m_yellowBall = new Sphere(
    m_yellowBallStartPos,
    m_zeroVelocity,
    m_ballMass,
    m_ballRadius,
    m_ballElasticity,
    m_colourYellow);

  m_cyanBall = new Sphere(
    m_cyanBallStartPos,
    m_zeroVelocity,
    m_ballMass,
    m_ballRadius,
    m_ballElasticity,
    m_colourCyan);

  m_magentaBall = new Sphere(
    m_magentaBallStartPos,
    m_zeroVelocity,
    m_ballMass,
    m_ballRadius,
    m_ballElasticity,
    m_colourMagenta);

  // Add objects to scene
  m_physicsScene->addActor(m_redBall);
  m_physicsScene->addActor(m_greenBall);
  m_physicsScene->addActor(m_blueBall);
  m_physicsScene->addActor(m_yellowBall);
  m_physicsScene->addActor(m_cyanBall);
  m_physicsScene->addActor(m_magentaBall);
}

void PhysicsApp::setupCue()
{
  // TODO Create cue stick at a distance from the white ball
  // [X] Create m_variables for maxVelocity, maxDistance, actualDistance
  // [X] Create m_variable for location near white ball wherever it is on
  // table
  // [ ] Create m_variable for off-table spawn location after white
  // ball is hit
  // [X] Create/add cue stick to scene

  m_cue = new Box(
    m_cuePosition,
    m_cueExtents,
    m_zeroVelocity,
    m_cueMass,
    m_cueAngle,
    m_cueElasticity,
    m_colourBrown);

  m_cue->isKinematic(true);
  m_physicsScene->addActor(m_cue);
}

void PhysicsApp::playerInput()
{ // TODO Setup Player Input

  if (input->isKeyDown(aie::INPUT_KEY_W))
  {
    // [ ] W: brings it back towards the ball (fine control)
  }

  if (input->isKeyDown(aie::INPUT_KEY_S))
  {
    // [ ] S: increases distance between cue and ball
  }

  if (input->isKeyDown(aie::INPUT_KEY_SPACE))
  {
    // [ ] Spacebar: max velocity is stored(maxV), maxDist, actualDist
    // [ ] Math: MaxV* actualDist / maxDist
  }

  if (input->isKeyDown(aie::INPUT_KEY_A))
  {
    // [ ] A: rotate counter clockwise around ball
    m_cueRotationAngle -= 0.1f;
  }
  if (input->isKeyDown(aie::INPUT_KEY_D))
  {
    // [ ] D: rotate clockwise around ball
    m_cueRotationAngle += 0.1f;
  }
}

void PhysicsApp::drawScoreBoard()
{
  // [ ] ScoreBoard: Text object that iterates when ball sunk
  // [ ] ScoreBoard: Text object that iterates when white ball is hit}
  
  m_scoreBoardScore = "Score: " + std::to_string(m_score);

  m_2dRenderer->drawText(m_font, m_scoreBoardScore.c_str(), 875, 695);
}

// TODO Notes from Open Room Discussion
/* W brings it back towards the ball (fine control)
 * S increases distance between cue and ball
 * Spacebar: max velocity stored (maxV), a maxDist, actualDist... MaxV *
 * actualDist/maxDist

 * A/D rotate around ball

 * Cue has max velocity that can be reached

 * Move cue off table until all balls velocity = ~0, move cue back to the
 * white ball. Rotating around ball should be something like cos angle, sin
 * angle (unit circle maths)

 * If sink white ball, wait for ball velocity = 0, set it up at starting
 * position with cue.

 * Scoreboard, text object that iterates when ball sunk (or removes a point
 * for white ball).

 * When cue off table, disable input?

 * Boolean to check if within window boundaries. Move to off screen?
 * x maxBoundary 0 + 1/2 width
 * x minBoundary 0 - 1/2 width
 * y maxBoundary 0 + 1/2 height
 * y minBoundary 0 - 1/2 height
 */

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
