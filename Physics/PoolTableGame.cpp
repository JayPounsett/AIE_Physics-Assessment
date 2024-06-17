#include "PoolTableGame.h"

#include <iostream>

#include "Application.h"
#include "PhysicsScene.h"

PoolTableGame::~PoolTableGame() {}

void PoolTableGame::update(float deltaTime, aie::Input* input)
{
  this->gameInput(deltaTime, input, m_cue, m_whiteBall);
  this->updateScore();

  // If white ball sunk, move it off the table
  if (m_sunkWhiteBall)
  {
    // Move white ball off the table
    m_whiteBall->setPosition(glm::vec2(-200, -200));
    m_whiteBall->setVelocity(m_zeroVelocity);
    m_whiteBall->setAngularVelocity(0.0f);
    m_whiteBall->setOrientation(0);
  }

  // Move the cue off the table until the balls finish moving
  if (hasBallVelocity() && isCueOnTable)
  {
    m_cue->setVelocity(glm::vec2(0, 0));
    m_cue->setAngularVelocity(0);
    m_cue->setPosition(m_cueOffTablePosition);
    isCueOnTable = false;
  }

  #pragma region Checks to move the white ball and/or cue back to the table
  // Move the white ball back to the table
  if (!hasBallVelocity() && m_sunkWhiteBall)
  {
    m_whiteBall->setPosition(m_whiteBallStartPos);
    m_whiteBall->setVelocity(m_zeroVelocity);

    m_sunkWhiteBall = false;
  }

  // When the balls finish moving, bring the cue back
  else if (!hasBallVelocity() && !isCueOnTable && !m_sunkWhiteBall)
  {
    glm::vec2 whiteBallPos = m_whiteBall->getPosition();

    m_cuePosition = glm::vec2(
      m_whiteBall->getPosition().x + m_cueOffset.x,
      m_whiteBall->getPosition().y);

    m_cue->setPosition(m_cuePosition);
    m_cue->setOrientation(-180);

    isCueOnTable = true;
    wasSpacePressed = false;
  }
  #pragma endregion Checks to move the white ball and/or cue back to the table
}

void PoolTableGame::draw(aie::Renderer2D* renderer, aie::Font* font)
{
  // output 'some text, uses the last used m_colour
  renderer->drawText(font, "Press ESC to quit", 180, 5);

  this->drawScoreBoard(renderer, font);
}

#pragma region Setup Pool Table
void PoolTableGame::setupPoolTableGame(PhysicsScene* physicsScene)
{
#pragma region Setup Table Edges
  m_tableEdgeLeft = new Plane(glm::vec3(1, 0, 0), -95, 0.4f);
  m_tableEdgeRight = new Plane(glm::vec3(-1, 0, 0), -95, 0.4f);
  m_tableEdgeTop = new Plane(glm::vec3(0, -1, 0), -51, 0.4f);
  m_tableEdgeBottom = new Plane(glm::vec3(0, 1, 0), -51, 0.4f);

  // Add table edges into the scene
  physicsScene->addActor(m_tableEdgeLeft);
  physicsScene->addActor(m_tableEdgeRight);
  physicsScene->addActor(m_tableEdgeTop);
  physicsScene->addActor(m_tableEdgeBottom);
#pragma endregion Setup Table Edges

#pragma region Setup Pockets
  m_pocketTopLeft = new Sphere(
    m_pocketTopLeftPosition,
    m_zeroVelocity,
    m_pocketMass,
    m_pocketRadius,
    m_pocketElasticity,
    m_colourBlack);
  m_pocketTopMid = new Sphere(
    m_pocketTopMidPosition,
    m_zeroVelocity,
    m_pocketMass,
    m_pocketRadius,
    m_pocketElasticity,
    m_colourBlack);
  m_pocketTopRight = new Sphere(
    m_pocketTopRightPosition,
    m_zeroVelocity,
    m_pocketMass,
    m_pocketRadius,
    m_pocketElasticity,
    m_colourBlack);
  m_pocketBottomLeft = new Sphere(
    m_pocketBottomLeftPosition,
    m_zeroVelocity,
    m_pocketMass,
    m_pocketRadius,
    m_pocketElasticity,
    m_colourBlack);
  m_pocketBottomMid = new Sphere(
    m_pocketBottomMidPosition,
    m_zeroVelocity,
    m_pocketMass,
    m_pocketRadius,
    m_pocketElasticity,
    m_colourBlack);
  m_pocketBottomRight = new Sphere(
    m_pocketBottomRightPosition,
    m_zeroVelocity,
    m_pocketMass,
    m_pocketRadius,
    m_pocketElasticity,
    m_colourBlack);

  // Setup Pockets as kinematic
  m_pocketTopLeft->setKinematic(true);
  m_pocketTopMid->setKinematic(true);
  m_pocketTopRight->setKinematic(true);
  m_pocketBottomLeft->setKinematic(true);
  m_pocketBottomMid->setKinematic(true);
  m_pocketBottomRight->setKinematic(true);

  // Add pockets to the pockets vector
  m_pockets.push_back(m_pocketTopLeft);
  m_pockets.push_back(m_pocketTopMid);
  m_pockets.push_back(m_pocketTopRight);
  m_pockets.push_back(m_pocketBottomLeft);
  m_pockets.push_back(m_pocketBottomMid);
  m_pockets.push_back(m_pocketBottomRight);

  // Add pockets into the scene
  physicsScene->addActor(m_pocketTopLeft);
  physicsScene->addActor(m_pocketTopMid);
  physicsScene->addActor(m_pocketTopRight);
  physicsScene->addActor(m_pocketBottomLeft);
  physicsScene->addActor(m_pocketBottomMid);
  physicsScene->addActor(m_pocketBottomRight);
#pragma endregion Setup Pockets

#pragma region Setup White Ball
  // Setup white ball
  m_whiteBall = new Sphere(
    m_whiteBallStartPos,
    m_zeroVelocity,
    m_ballMass,
    m_ballRadius,
    m_ballElasticity,
    m_colourWhite);

  // Add white ball to scene
  m_balls.push_back(m_whiteBall);
  physicsScene->addActor(m_whiteBall);
#pragma endregion Setup White Ball

#pragma region Setup Cue
  // Offset of cue from the white ball
  m_cueOffset = glm::vec2(m_cueExtents.x + m_ballRadius, 0.0f);

  // Setup cue position
  m_cuePosition = glm::vec2(
    m_whiteBall->getPosition().x + m_cueOffset.x, m_whiteBall->getPosition().y);

  // Setup cue
  m_cue = new Box(
    m_cuePosition,
    m_cueExtents,
    m_zeroVelocity,
    m_cueMass,
    180.f,
    m_cueElasticity,
    m_colourBrown);

  // Add cue to scene
  physicsScene->addActor(m_cue);
  isCueOnTable = true;
#pragma endregion Setup Cue

#pragma region Setup Coloured Balls
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

  // Add coloured balls into the balls vector
  m_balls.push_back(m_redBall);
  m_balls.push_back(m_greenBall);
  m_balls.push_back(m_blueBall);
  m_balls.push_back(m_yellowBall);
  m_balls.push_back(m_cyanBall);
  m_balls.push_back(m_magentaBall);

  // Add coloured balls to scene
  physicsScene->addActor(m_redBall);
  physicsScene->addActor(m_greenBall);
  physicsScene->addActor(m_blueBall);
  physicsScene->addActor(m_yellowBall);
  physicsScene->addActor(m_cyanBall);
  physicsScene->addActor(m_magentaBall);
#pragma endregion Setup Coloured Balls
}
#pragma endregion Setup Pool Table

float PoolTableGame::getAngleBetweenVectors(glm::vec2 v1, glm::vec2 v2)
{
  glm::vec2 v1Norm = glm::normalize(v1);
  glm::vec2 v2Norm = glm::normalize(v2);
  float dot = glm::dot(v1Norm, v2Norm);
  float angle = glm::acos(dot);
  return angle;
}

bool PoolTableGame::hasBallVelocity()
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

void PoolTableGame::drawScoreBoard(aie::Renderer2D* renderer, aie::Font* font)
{
  // [ ] ScoreBoard: Text object that iterates when ball sunk

  m_scoreBoardScore = "Score: " + std::to_string(m_score);

  renderer->drawText(font, m_scoreBoardScore.c_str(), 875, 692);
}

void PoolTableGame::updateScore()
{
  for (auto it = m_balls.begin(); it != m_balls.end();)
  {
    Sphere* ball = *it;

    for (auto pocket : m_pockets)
    {
      if (m_poolTableScene->sphere2Sphere(ball, pocket) == true)
      {
        if (ball->getColour() != m_colourWhite)
        {
          // Remove the ball from the scene
          m_poolTableScene->removeActor(ball);

          // Erase the ball from the vector
          it = m_balls.erase(it);

          // Increase the score
          m_score += 1;

          // Exit the inner loop as the ball has been removed
          break;
        }

        if (ball->getColour() == m_colourWhite)
        {
          m_sunkWhiteBall = true;
          m_score -= 1;
        }
      }
    }

    // Move to the next ball
    if (it != m_balls.end())
    {
      ++it;
    }
  }
  // for (auto ball_ptr = m_balls.begin(); ball_ptr != m_balls.end();)
  //{
  //   auto ball = *ball_ptr; // Dereference the pointer to get the Sphere
  //   object

  //  for (auto pocket_ptr = m_pockets.begin(); pocket_ptr != m_pockets.end();)
  //  {
  //    auto pocket = *pocket_ptr;

  //    if (
  //      m_poolTableScene->sphere2Sphere(ball, pocket) &&
  //      ball->getColour() != m_colourWhite)
  //    {
  //      ball_ptr = m_balls.erase(ball_ptr); // Remove the ball from the vector
  //      m_poolTableScene->removeActor(ball);

  //      m_score += 1;
  //      break;
  //    }

  //    else if (
  //      m_poolTableScene->sphere2Sphere(ball, pocket) &&
  //      ball->getColour() == m_colourWhite)
  //    {
  //      m_sunkWhiteBall = true;
  //      break;
  //    }
  //    else
  //    {
  //      ++pocket_ptr;
  //    }
  //  }
  //  ++ball_ptr;
  //}

  // if (m_sunkWhiteBall)
  //{
  //   //m_whiteBall->setVelocity(glm::vec2(0, 0));
  //   m_whiteBall->setPosition(m_whiteBallStartPos);
  //   m_sunkWhiteBall = false;

  //  m_score -= 1;
  //}

  // if (m_score < 0)
  //{
  //   m_score = 0;
  // }
}

void PoolTableGame::gameInput(
  const float& deltaTime, aie::Input* input, Box* box, Sphere* sphere)
{
  // Get the current position of the box and sphere
  glm::vec2 boxPosition = box->getPosition();
  glm::vec2 spherePosition = sphere->getPosition();

  // Calculate the direction from box to sphere
  glm::vec2 direction = glm::normalize(spherePosition - boxPosition);

  // Calculate the perpendicular direction for strafing
  glm::vec2 rotationDirection = glm::vec2(-direction.y, direction.x);

  // Calculate the up vector for the lookAt matrix
  glm::vec2 up = glm::vec2(0, 1);

  // Calculate the lookAt matrix
  glm::mat4 lookAtMatrix = glm::lookAt(
    glm::vec3(boxPosition.x, boxPosition.y, 0),
    glm::vec3(spherePosition.x, spherePosition.y, 0),
    glm::vec3(0, 0, 1));

  // Extract the rotation matrix from the lookAt matrix
  m_cueStickRotationMatrix = glm::mat3(lookAtMatrix);

  // Calculate the new orientation based on the rotation matrix
  float newOrientation = glm::degrees(
    atan2(m_cueStickRotationMatrix[1][0], m_cueStickRotationMatrix[0][0]));

  // Update the orientation of the box
  box->setOrientation(newOrientation + 90.0f);

  // Define movement speed for strafing
  float strafeSpeed = 100.0f * deltaTime;

  // Move left when pressing A
  if (input->isKeyDown(aie::INPUT_KEY_A))
  {
    box->setPosition(box->getPosition() - rotationDirection * strafeSpeed);
  }

  // Move right when pressing D
  if (input->isKeyDown(aie::INPUT_KEY_D))
  {
    box->setPosition(box->getPosition() + rotationDirection * strafeSpeed);
  }

  // Move toward sphere when pressing W
  if (input->isKeyDown(aie::INPUT_KEY_W))
  {
    // Check if the cue is at the ball, stop input if it is
    if (
      m_cue->getPosition().x <=
      (m_whiteBall->getPosition().x + m_ballRadius + m_cueExtents.x))
    {
      return;
    }
    box->setPosition(box->getPosition() + direction * strafeSpeed);
  }

  // Move away from sphere when pressing S
  if (input->isKeyDown(aie::INPUT_KEY_S))
  {
    // Check if the cue is at it's max distance, stop input if it is
    if (
      m_cue->getPosition().x >= (m_whiteBall->getPosition().x + m_ballRadius) +
                                  m_cueExtents.x + m_cueMaxDistance)
    {
      return;
    }
    box->setPosition(box->getPosition() - direction * strafeSpeed);
  }

  if (input->isKeyDown(aie::INPUT_KEY_SPACE))
  {
    if (wasSpacePressed) return;

    wasSpacePressed = true;

    glm::vec2 cuePosition = m_cue->getPosition();
    glm::vec2 whiteBallPos = m_whiteBall->getPosition();

    float distance = glm::length(whiteBallPos - cuePosition);
    float maxDistance = 100.0f; // Define your maximum distance here

    // Adjust the velocity based on the distance
    float velocityAdjustment = 1.0f - (distance / maxDistance);
    glm::vec2 cueVelocity =
      glm::normalize(glm::vec2(whiteBallPos - cuePosition)) *
      (m_cueStrength * velocityAdjustment);

    m_cue->setVelocity(cueVelocity);
  }

  // Debug scoreboard by pressing Q
  if (input->isKeyDown(aie::INPUT_KEY_Q))
  {
    m_score += 1;
  }
}
