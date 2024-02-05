#include "RigidBody.h"

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass):
    PhysicsObject(shapeID)
{
    // Unsure default values until tested
    m_shapeID = shapeID;
    m_position = position;
    m_velocity = velocity;
    m_orientation = orientation;
    m_mass = mass;
}

RigidBody::~RigidBody()
{
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void RigidBody::applyForce(glm::vec2 force)
{
    //const float mass = getMass();
    //glm::vec2 acceleration = force/mass;
}

void RigidBody::applyForceToActor(RigidBody* actor2, glm::vec2 force)
{
}


