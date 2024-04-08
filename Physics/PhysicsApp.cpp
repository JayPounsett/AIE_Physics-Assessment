#include "PhysicsApp.h"

#include <glm/ext.hpp>

#include "Box.h"
#include "Font.h"
#include "Gizmos.h"
#include "Input.h"
#include "Plane.h"
#include "Sphere.h"
#include "Texture.h"

PhysicsApp::PhysicsApp() {}

PhysicsApp::~PhysicsApp() {}

bool PhysicsApp::startup() {
  // increase the 2D line count to maximize the number of objects we can draw
  aie::Gizmos::create(255U, 255U, 65535U, 65535U);

  m_2dRenderer = new aie::Renderer2D();

  m_font = new aie::Font("./font/consolas.ttf", 32);

  m_physicsScene = new PhysicsScene();
  m_physicsScene->setTimeStep(0.01f);

  // setupContinuousDemo(glm::vec2(-40, 0), 45, 40, 10);
  // projectilePhysicsNumerical();
  // ballsOnPlane();
  // newtonsCradle();
  cubesOnPlane();


  return true;
}

void PhysicsApp::shutdown() {
  delete m_font;
  delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime) {
  // input example
  aie::Input* input = aie::Input::getInstance();

  aie::Gizmos::clear();

  m_physicsScene->update(deltaTime);
  m_physicsScene->draw();

  // exit the application
  if (input->isKeyDown(aie::INPUT_KEY_ESCAPE)) quit();
}

void PhysicsApp::draw() {
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

void PhysicsApp::newtonsCradle() {
  m_physicsScene->setGravity(glm::vec2(0));

  m_physicsScene->addActor(new Plane(glm::vec2(1, 0), -50));
  m_physicsScene->addActor(new Plane(glm::vec2(-1, 0), -50));

  // basic Newton's Cradle - 5 balls of equal mass
  m_physicsScene->addActor(new Sphere(
    glm::vec2(-30, 20), glm::vec2(10, 0), 1.0f, 5, 0, glm::vec4(1, 0, 0, 1)));
  m_physicsScene->addActor(new Sphere(
    glm::vec2(0, 20), glm::vec2(0, 0), 1.0f, 5, 0, glm::vec4(1, 0, 0, 1)));
  m_physicsScene->addActor(new Sphere(
    glm::vec2(10, 20), glm::vec2(0, 0), 1.0f, 5, 0, glm::vec4(1, 0, 0, 1)));
  m_physicsScene->addActor(new Sphere(
    glm::vec2(20, 20), glm::vec2(0, 0), 1.0f, 5, 0, glm::vec4(1, 0, 0, 1)));
  m_physicsScene->addActor(new Sphere(
    glm::vec2(30, 20), glm::vec2(0, 0), 1.0f, 5, 0, glm::vec4(1, 0, 0, 1)));
}

void PhysicsApp::projectilePhysicsNumerical() {
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
    new Sphere(startPosition, velocity, mass, radius, 0, colourRed));
}

void PhysicsApp::ballsOnPlane() {
  m_physicsScene->setGravity(glm::vec2(0, -9.82f));

  Plane* plane = new Plane(glm::vec2(0, 1), -30);

  Sphere* ball1 = new Sphere(
    glm::vec2(-20, 0), glm::vec2(0), 4.0f, 4, 0, glm::vec4(1, 0, 0, 1));
  Sphere* ball2 = new Sphere(
    glm::vec2(10, 0), glm::vec2(0), 4.0f, 4, 0, glm::vec4(0, 1, 0, 1));

  m_physicsScene->addActor(plane);
  m_physicsScene->addActor(ball1);
  m_physicsScene->addActor(ball2);
}

void PhysicsApp::cubesOnPlane() {
  m_physicsScene->setGravity(glm::vec2(0, -9.82f));

  Plane* plane = new Plane(glm::vec2(0, 1), -30);

  Box* box1 = new Box(
    glm::vec2(0), glm::vec2(3), glm::vec2(0), 4.0f, 30, glm::vec4(1, 1, 0, 1));

  Box* box2 = new Box(
    glm::vec2(0, 10),
    glm::vec2(3),
    glm::vec2(0),
    4.0f,
    60,
    glm::vec4(1, 0, 1, 1));

  m_physicsScene->addActor(plane);
  m_physicsScene->addActor(box1);
  m_physicsScene->addActor(box2);
}

void PhysicsApp::setupContinuousDemo(
  glm::vec2 startPosition, float inclination, float speed, float gravity) {
  float t = 0;
  float tStep = 0.5f;
  float radius = 1.0f;
  int segments = 12;
  glm::vec4 colourYellow{1, 1, 0, 1};

  // Gravity needs to be negative to pull objects downwards
  glm::vec2 acceleration = {0, -gravity};

  // Calculate velocity (as given speed but not speed over time)
  // cos and sin use radians and degrees
  float velocityX = speed * glm::cos(glm::radians(inclination));
  float velocityY = speed * glm::sin(glm::radians(inclination));
  glm::vec2 velocity{velocityX, velocityY};
  glm::vec2 changePosition;

  while (t <= 5) {
    changePosition = {
      startPosition.x + velocity.x * t,
      startPosition.y + velocity.y * t + 0.5f * acceleration.y * t * t};
    // draw projectile
    aie::Gizmos::add2DCircle(
      startPosition + changePosition, radius, segments, colourYellow);
    t += tStep;
  }
}
