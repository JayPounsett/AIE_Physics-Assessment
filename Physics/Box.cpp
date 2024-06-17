#include "Box.h"

#include <Gizmos.h>

Box::~Box() {}

// void Box::fixedUpdate(glm::vec2 gravity, float timeStep) {
//   Rigidbody::fixedUpdate(gravity, timeStep);
//
//   // store the local axes
//   float cs = cosf(m_orientation);
//   float sn = sinf(m_orientation);
//   m_localX = glm::normalize(glm::vec2(cs, sn));
//   m_localY = glm::normalize(glm::vec2(-sn, cs));
// }

bool Box::checkBoxCorners(
  const Box& box,
  glm::vec2& contact,
  int& numContacts,
  float& pen,
  glm::vec2& edgeNormal)
{
  float minX{}, maxX{}, minY{}, maxY{};
  float boxW = box.getExtents().x * 2.0f;
  float boxH = box.getExtents().y * 2.0f;
  int numLocalContacts = 0;
  glm::vec2 localContact(0, 0);
  bool first = true;

  // loop over all corners of the other box
  for (float x = -box.getExtents().x; x < boxW; x += boxW)
  {
    for (float y = -box.getExtents().y; y < boxH; y += boxH)
    {
      // get the position in world space
      glm::vec2 p = box.getPosition() + x * box.m_localX + y * box.m_localY;

      // get the position in our box's space
      glm::vec2 p0(
        glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY));

      // update the extents in each cardinal direction in our box's space
      // (ie extents along the separating axes)
      if (first || p0.x < minX) minX = p0.x;
      if (first || p0.x > maxX) maxX = p0.x;
      if (first || p0.y < minY) minY = p0.y;
      if (first || p0.y > maxY) maxY = p0.y;

      // if this corner is inside the box, add it to the list of contact points
      if (
        p0.x >= -m_extents.x && p0.x <= m_extents.x && p0.y >= -m_extents.y &&
        p0.y <= m_extents.y)
      {
        numLocalContacts++;
        localContact += p0;
      }
      first = false;
    }
  }
  // if we lie entirely on one side of the box along one axis, we've found a
  // separating axis, and we can exit
  if (
    maxX <= -m_extents.x || minX >= m_extents.x || maxY <= -m_extents.y ||
    minY >= m_extents.y)
    return false;

  if (numLocalContacts == 0) return false;

  bool result = false;
  contact +=
    m_position + (localContact.x * m_localX + localContact.y * m_localY) /
                   (float)numLocalContacts;
  numContacts++;

  // find the minimum penetration vector as a penetration amount and normal
  float pen0 = m_extents.x - minX;
  if (pen0 > 0 && (pen0 < pen || pen == 0))
  {
    edgeNormal = m_localX;
    pen = pen0;
    result = true;
  }

  pen0 = maxX + m_extents.x;
  if (pen0 > 0 && (pen0 < pen || pen == 0))
  {
    edgeNormal = -m_localX;
    pen = pen0;
    result = true;
  }

  pen0 = m_extents.y - minY;
  if (pen0 > 0 && (pen0 < pen || pen == 0))
  {
    edgeNormal = m_localY;
    pen = pen0;
    result = true;
  }

  pen0 = maxY + m_extents.y;
  if (pen0 > 0 && (pen0 < pen || pen == 0))
  {
    edgeNormal = -m_localY;
    pen = pen0;
    result = true;
  }
  return result;
}

void Box::draw()
{
  // draw using local axes
  glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
  glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
  glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
  glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;

  aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
  aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}

//#pragma region Cue Aiming Line
//void Box::drawCueAimLine()
//{
//  // TODO: Draw cue aiming line
//  // TODO: Line not updating as cue is moved
//  // TODO: Add in distance variable for when cue is moved forward/back to show
//  // how far it may move
//
//  // Store cue tip position
//  glm::vec2 cueTipPosition = glm::vec2(
//    this->getPosition().x - this->getExtents().x, this->getPosition().y);
//
//  // Vector for the line's end point
//  glm::vec2 cueAimLineEnd = glm::vec2(cueTipPosition.x - 100, cueTipPosition.y);
//
//  aie::Gizmos::add2DLine(cueTipPosition, cueAimLineEnd, glm::vec4(1, 1, 1, 1));
//}
glm::vec2 Box::getFacing()
{
  float radians = this->getOrientationRadians();
  // Starting value of 3.1459274 (cue is pointing to the left)

  float xValue = 0.0f;
  float yValue = 0.0f;

  xValue = glm::cos(radians); 
  yValue = glm::sin(-radians); 

  glm::vec2 normVector = glm::normalize(glm::vec2(xValue, yValue));

  return normVector;
  }
#pragma endregion Cue Aiming Line
