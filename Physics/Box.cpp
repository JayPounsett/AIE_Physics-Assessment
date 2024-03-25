#include "Box.h"
#include <Gizmos.h>

Box::~Box() {}

void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
    Rigidbody::fixedUpdate(gravity, timeStep);

    // store the local axes
    float cs = cosf(m_orientation);
    float sn = sinf(m_orientation);
    m_localX = glm::normalize(glm::vec2(cs, sn));
    m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void Box::draw() 
{
    // if only using rotation
    // glm::mat4 transform = glm::rotate(m_rotation, glm::vec3(0, 0, 1));
    // aie::Gizmos::add2DAABBFilled(getCenter(), m_extents, m_colour, &transform);

    // draw using local axes
    glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
    glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
    glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
    glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;

    aie::Gizmos::add2DTri(p1,p2,p4,m_colour);
    aie::Gizmos::add2DTri(p1,p4,p3,m_colour);
}
