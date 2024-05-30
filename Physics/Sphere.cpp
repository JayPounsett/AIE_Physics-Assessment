#include "Sphere.h"

#include "Gizmos.h"

Sphere::~Sphere() {}

void Sphere::draw()
{
  glm::vec2 end =
    glm::vec2(std::cos(m_orientationRadians), std::sin(m_orientationRadians)) * m_radius;

  aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour);
  aie::Gizmos::add2DLine(
    m_position, m_position + end, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}
