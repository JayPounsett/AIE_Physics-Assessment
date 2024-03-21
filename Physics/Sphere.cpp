#include "Sphere.h"

#include "Gizmos.h"
#include "PhysicsObject.h"

Sphere::~Sphere() {}

void Sphere::draw()
{
  aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour, nullptr);
}
