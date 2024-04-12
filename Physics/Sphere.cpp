#include "Sphere.h"

#include "Gizmos.h"
#include "PhysicsObject.h"

Sphere::~Sphere() {}

void Sphere::draw() {
  glm::vec2 end =
      glm::vec2(std::cos(m_orientation), std::sin(m_orientation)) * m_radius;

  aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour);
  aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1, 1, 1, 1));
}
