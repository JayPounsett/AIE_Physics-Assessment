#include "PhysicsApp.h"

#include <iostream>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

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

  playerInput(deltaTime);
  updateScore();

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
  drawCueAimLine();

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

#pragma region Setup Pool Table
void PhysicsApp::setupPoolTableGame()
{
  aie::Application::setBackgroundColour(0.0f, 0.25f, 0.0f);
  m_physicsScene->setGravity(glm::vec2(0));

  // Setup Table Edges
  m_tableEdgeLeft = new Plane(glm::vec2(1, 0), -95, 0.4f);
  m_tableEdgeRight = new Plane(glm::vec2(-1, 0), -95, 0.4f);
  m_tableEdgeTop = new Plane(glm::vec2(0, -1), -51, 0.4f);
  m_tableEdgeBottom = new Plane(glm::vec2(0, 1), -51, 0.4f);

  // Setup Pockets as kinematic
  m_pocketTopMid->setKinematic(true);
  m_pocketTopRight->setKinematic(true);
  m_pocketBottomLeft->setKinematic(true);
  m_pocketBottomMid->setKinematic(true);
  m_pocketBottomRight->setKinematic(true);

  // Setup White Ball
  m_whiteBall = new Sphere(
    m_whiteBallStartPos,
    m_zeroVelocity,
    m_ballMass,
    m_ballRadius,
    m_ballElasticity,
    m_colourWhite);

  // Setup Cue Position with Offset
  m_cuePosition = glm::vec2(
    m_whiteBall->getPosition().x + m_cueExtents.x + m_ballRadius,
    m_whiteBall->getPosition().y);


  // Setup Cue Tip position for use with rotations as a box position is its
  // center point
  // getCueTipPosition();

  // Setup Cue
  m_cue = new Box(
    m_cuePosition,
    m_cueExtents,
    m_zeroVelocity,
    m_cueMass,
    180.0f,
    m_cueElasticity,
    m_colourBrown);

  // Setup Coloured Balls
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

  // Add balls & pockets to their vectors for collision checks
  // Add balls to balls vector
  m_balls.push_back(m_whiteBall);
  m_balls.push_back(m_redBall);
  m_balls.push_back(m_greenBall);
  m_balls.push_back(m_blueBall);
  m_balls.push_back(m_yellowBall);
  m_balls.push_back(m_cyanBall);
  m_balls.push_back(m_magentaBall);
  // Add pockets to the pockets vector
  m_pockets.push_back(m_pocketTopLeft);
  m_pockets.push_back(m_pocketTopMid);
  m_pockets.push_back(m_pocketTopRight);
  m_pockets.push_back(m_pocketBottomLeft);
  m_pockets.push_back(m_pocketBottomMid);
  m_pockets.push_back(m_pocketBottomRight);

  // Cue
  // m_cue->isKinematic(true);

  // White Ball
  // m_whiteBall->setKinematic(true);

  // Add objects to scene
  // Table
  m_physicsScene->addActor(m_tableEdgeLeft);
  m_physicsScene->addActor(m_tableEdgeRight);
  m_physicsScene->addActor(m_tableEdgeTop);
  m_physicsScene->addActor(m_tableEdgeBottom);
  // Pockets
  m_physicsScene->addActor(m_pocketTopLeft);
  m_physicsScene->addActor(m_pocketTopMid);
  m_physicsScene->addActor(m_pocketTopRight);
  m_physicsScene->addActor(m_pocketBottomLeft);
  m_physicsScene->addActor(m_pocketBottomMid);
  m_physicsScene->addActor(m_pocketBottomRight);
  // Balls
  m_physicsScene->addActor(m_whiteBall);
  m_physicsScene->addActor(m_redBall);
  m_physicsScene->addActor(m_greenBall);
  m_physicsScene->addActor(m_blueBall);
  m_physicsScene->addActor(m_yellowBall);
  m_physicsScene->addActor(m_cyanBall);
  m_physicsScene->addActor(m_magentaBall);
  // Cue
  m_physicsScene->addActor(m_cue);
}
#pragma endregion Setup Pool Table

#pragma region Scoreboard
void PhysicsApp::drawScoreBoard()
{
  // [X] ScoreBoard: Text object that iterates when ball sunk

  m_scoreBoardScore = "Score: " + std::to_string(m_score);

  m_2dRenderer->drawText(m_font, m_scoreBoardScore.c_str(), 875, 692);
}

void PhysicsApp::updateScore()
{
  // TODO: Increase score when a coloured ball sinks or decreases when white
  // sinks
}
#pragma endregion Scoreboard

#pragma region Cue Aiming Line
void PhysicsApp::drawCueAimLine()
{
  // TODO: Draw line between cue and ball

  // This is called in draw() but nothing in update(), may be an issue
  // m_2dRenderer->drawLine(
  //  m_cuePosition.x,
  //  m_cuePosition.y,
  //  m_cuePosition.x + m_cue->getVelocity().x, // also tried
  //  m_cueActualDistance m_cuePosition.y + m_cue->getVelocity().y, // also
  //  tried m_cueActualDistance 1.0f);

  // Set Origin to the cue's tip
  // DrawLine() has the origin as the bottom left while objects are using the
  // middle of the window

  m_cueTipPosition = getCueTipPosition();
  glm::vec2 cueAimLine =
    glm::vec2(getCueTipPosition().x - 100, getCueTipPosition().y);

  // This is (54, 0) when origin is windowWidth/2, windowHeight/2 as the
  // renderer's origin is bottom left.

  aie::Gizmos::add2DLine(m_cueTipPosition, cueAimLine, m_colourWhite);

  // m_2dRenderer->drawLine(
  //   m_cueTipPosition.x +
  //     getWindowWidth() /
  //       2, // This is assuming 0,0 is bottom left, so need to adjust
  //   m_cueTipPosition.y + getWindowHeight() / 2,
  //   m_cueTipPosition.x + getWindowWidth() / 2 -
  //     500, // also tried m_cueActualDistance
  //   m_cueTipPosition.y + getWindowHeight() / 2, // also tried
  //   m_cueActualDistance 1.0f);
}
#pragma endregion Cue Aiming Line

#pragma region Player Input
void PhysicsApp::playerInput(float deltaTime)
{ // TODO Setup Player Input
  // Not currently functioning for W/S/A/D/Space

  if (input->isKeyDown(aie::INPUT_KEY_W))
  {
    // [ ] W: brings it back towards the ball (fine control)

    //// Check to see if distance between cue and white ball is close to zero
    // if (m_cueActualDistance <= 0.01f) m_cueActualDistance = 0.0f;

    //// Decrease distance between white ball and cue
    // m_cueActualDistance -=
    //   1.0f * deltaTime; // Should (float) actualDistance be a vector2 or a
    //                     // length of a vector?
  }

  if (input->isKeyDown(aie::INPUT_KEY_S))
  {
    // [ ] S: increases distance between cue and ball

    //// Check to see if max distance has been reached
    // if (m_cueActualDistance > m_cueMaxDistance) // Should (float)
    // actualDistance
    //                                             // be a vector2 or a length
    //                                             of a
    //                                             // vector?
    //   m_cueActualDistance = m_cueMaxDistance;

    //// Increase distance between white ball and cue
    // m_cueActualDistance += 1.0f * deltaTime;
  }

  if (input->isKeyDown(aie::INPUT_KEY_SPACE))
  {
    // [ ] Spacebar: max velocity is stored(maxV), maxDist, actualDist
    // [ ] Math: MaxV* actualDist / maxDist

    // Once space bar is pressed, send the cue towards the white ball with
    // velocity
  }

  if (input->isKeyDown(aie::INPUT_KEY_A))
  {
    // [ ] A: rotate clockwise around ball
    m_cueAngle = m_cue->getOrientation();
    m_cueAngle += 1.0f;

    m_cue->setOrientation(m_cueAngle);
    m_cue->setPosition(m_cue->getFacing() * deltaTime);
    rotateCueAroundWhiteBall();
  }
  if (input->isKeyDown(aie::INPUT_KEY_D))
  {
    // [ ] D: rotate counter-clockwise around ball
    m_cueAngle = m_cue->getOrientation();
    m_cueAngle -= 1.0f;

    m_cue->setOrientation(m_cueAngle);
    m_cue->setPosition(m_cue->getFacing() * deltaTime);
    rotateCueAroundWhiteBall();
  }

  // Debug scoreboard by pressing Q
  if (input->isKeyDown(aie::INPUT_KEY_Q))
  {
    m_score += 1;
  }
}
#pragma endregion Player Input

void PhysicsApp::rotateCueAroundWhiteBall()
{
  // Calculate angle between cue and white ball
  float angle = m_cue->getOrientation();

  // glm::vec2 cueFacingVector = m_cue->getFacing(); // Returns (-1, 0)
  //// Calculate the distance vector from the cue to the white ball
  // auto normalizedVector =
  //  glm::normalize(m_whiteBall->getPosition() - m_cue->getPosition());
  //// ((50,0), (74, 0)) = (-24,0) = (-1, 0)

  // float angleInDegrees = glm::dot(cueFacingVector, normalizedVector);

  // m_cue->setOrientation(angleInDegrees);


  //// Calculate the distance on the x-axis between cue and white ball
  // auto distance =
  //   glm::distance(m_cue->getPosition(), m_whiteBall->getPosition());
  //// float angle = m_cue->getOrientation();
  // auto angleVector = m_cue->getFacing();

  //// Dot product the cue & white ball position to get the cos(angle)
  // glm::vec2 normaliseWhiteBallPosition =
  //   glm::normalize(m_whiteBall->getPosition());
  // auto dotProductResult =
  //   glm::dot(glm::vec2(distance, 0), -normaliseWhiteBallPosition);

  //// Convert orientation to degrees and set cue's orientation
  // float angleInRadians = glm::acos(dotProductResult);
  // m_cue->setOrientation(glm::degrees(angleInRadians));
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
