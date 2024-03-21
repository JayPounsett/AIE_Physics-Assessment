#include "Box.h"

#include "Gizmos.h"
#include "PhysicsObject.h"

Box::~Box() {}

void Box::draw()
{
  aie::Gizmos::add2DAABBFilled(
    getPosition(), getExtents(), m_colour, &getRotation());
}
