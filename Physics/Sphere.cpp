#include "Sphere.h"
#include "Gizmos.h"

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius,
			   glm::vec4 colour)
	: RigidBody(SPHERE, position, velocity, 0, mass) {
  m_radius = radius;
  m_colour = colour;
}

<<<<<<< HEAD
Sphere::~Sphere() {}

void Sphere::draw() {
  aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour, nullptr);
=======
void Sphere::draw()
{
    aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour, nullptr);
>>>>>>> parent of 326e774 (Re-added bootstrap)
}
