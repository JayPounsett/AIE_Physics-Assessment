#include "PoolTableGame.h"

#include "Application.h"
#include "PhysicsScene.h"

PoolTableGame::~PoolTableGame() {}

void PoolTableGame::update(float deltaTime, aie::Input* input)
{
  //getCueMatrix();
  getCue()->drawCueAimLine();
  //this->playerInput(deltaTime, input);
  this->updateInput(deltaTime, input);
  this->updateScore();
 
  if (hasBallVelocity() && isCueOnTable)
  {
    m_cue->setVelocity(glm::vec2(0, 0));
    m_cue->setPosition(m_cueOffTablePosition);
    isCueOnTable = false;
  }
  
  if (!hasBallVelocity() && !isCueOnTable)
  {
    glm::vec2 whiteBallPos = m_whiteBall->getPosition();

    m_cue->setPosition(glm::vec2(
      whiteBallPos.x + m_ballRadius + m_cueExtents.x, whiteBallPos.y));

    isCueOnTable = true;
  }
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
  physicsScene->addActor(m_whiteBall);
#pragma endregion Setup White Ball

#pragma region Setup Cue
  // Offset of cue from the white ball
  m_cueOffset = glm::vec2(m_cueExtents.x + m_ballRadius, 0.0f);
  
  // Setup cue position
  m_cuePosition = glm::vec2(
    m_whiteBall->getPosition().x + m_cueOffset.x,
    m_whiteBall->getPosition().y);

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

  // Add balls into the balls vector
  m_balls.push_back(m_whiteBall);
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

#pragma region Scoreboard
void PoolTableGame::drawScoreBoard(aie::Renderer2D* renderer, aie::Font* font)
{
  // [ ] ScoreBoard: Text object that iterates when ball sunk

  m_scoreBoardScore = "Score: " + std::to_string(m_score);

  renderer->drawText(font, m_scoreBoardScore.c_str(), 875, 692);
}

void PoolTableGame::updateScore()
{
  // TODO: Increase score when a coloured ball sinks or decreases when white
  // sinks
}
#pragma endregion Scoreboard

#pragma region Player Input
//void PoolTableGame::playerInput(float deltaTime, aie::Input* input)
//{ // TODO Setup Player Input
//  // Not currently functioning for W/S/A/D/Space
//
//  if (input->isKeyDown(aie::INPUT_KEY_W))
//  {
//    // [ ] W: brings it back towards the ball (fine control)
//
//    //// Check to see if distance between cue and white ball is close to zero
//    // if (m_cueActualDistance <= 0.01f) m_cueActualDistance = 0.0f;
//
//    //// Decrease distance between white ball and cue
//    // m_cueActualDistance -=
//    //   1.0f * deltaTime; // Should (float) actualDistance be a vector2 or a
//    //                     // length of a vector?
//  }
//
//  if (input->isKeyDown(aie::INPUT_KEY_S))
//  {
//    // [ ] S: increases distance between cue and ball
//
//    //// Check to see if max distance has been reached
//    // if (m_cueActualDistance > m_cueMaxDistance) // Should (float)
//    // actualDistance
//    //                                             // be a vector2 or a length
//    //                                             of a
//    //                                             // vector?
//    //   m_cueActualDistance = m_cueMaxDistance;
//
//    //// Increase distance between white ball and cue
//    // m_cueActualDistance += 1.0f * deltaTime;
//  }
//
//  if (input->isKeyDown(aie::INPUT_KEY_SPACE))
//  {
//    // [ ] Spacebar: max velocity is stored(maxV), maxDist, actualDist
//    // [ ] Math: MaxV* actualDist / maxDist
//
//    // Once space bar is pressed, send the cue towards the white ball with
//    // velocity
//  }
//
//  if (input->isKeyDown(aie::INPUT_KEY_A))
//  {
//    // TODO: A - rotate clockwise around ball
//    m_cueAngle = glm::degrees(m_cue->getOrientationRadians());
//    m_cueAngle += 1.0f;
//
//    m_cue->setOrientation(m_cueAngle);
//    //m_cue->setPosition(m_cue->getFacing() * deltaTime);
//    setFacing();
//
//    // TODO: Cue is rotating around it's origin. I need to
//    // m_cue->setPosition(m_cue->getFacing() * deltaTime);
//    // rotateCue();
//  }
//  if (input->isKeyDown(aie::INPUT_KEY_D))
//  {
//    // [ ] D: rotate counter-clockwise around ball
//    m_cueAngle = glm::degrees(m_cue->getOrientationRadians());
//    m_cueAngle -= 1.0f;
//
//    m_cue->setOrientation(m_cueAngle);
//    //m_cue->setPosition(m_cue->getFacing() * deltaTime);
//    // rotateCue();
//  }
//
//  // Debug scoreboard by pressing Q
//  if (input->isKeyDown(aie::INPUT_KEY_Q))
//  {
//    m_score += 1;
//  }
//}
#pragma endregion Player Input

//void PoolTableGame::rotateCue()
//{
//  // Calculate angle between cue and white ball
//  float angle = m_cue->getOrientationRadians();
//
//  // Using a camera as an example:
//  // Moving the cue left/right would be camera pitch.
//  // 
//  // mat4 cueMatrix
//  // vec3 position, vec3 front, vec3 right, don't need up (set to 0)
//  // float speed
//  // float sensitivity (if use mouse)
//  // 
//  // glm::mat4 getCueMatrix(){ this->cueMatrix = this->position, this->position + this->front, this->right, 0.0f); }
//}
