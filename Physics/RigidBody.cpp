#include "RigidBody.h"
#include <glm/glm.hpp>

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass):
	PhysicsObject(shapeID)
{
	m_shapeID = shapeID;
	m_position = position;
	m_velocity = velocity;
	m_orientation = orientation;
	m_mass = mass;
}

RigidBody::~RigidBody(){}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	applyForce(gravity * m_mass * timeStep);
}

void RigidBody::applyForce(glm::vec2 force)
{
	//In the applyForce() function you will need to calculate acceleration and add
	//it to the m_velocity member variable.You should be able to write this as one
	//line of code.

	m_velocity += force / m_mass;
}

void RigidBody::applyForceToActor(RigidBody* actor2, glm::vec2 force)
{
	//In the applyForceToActor() function you must first apply the input force to 
	//the input actor(by calling the applyForce() function of that actor), and then
	//apply the opposite force to the current actor(by calling applyForce() on this
	//using the negative force).

	actor2->applyForce(force); // Input actor & input force
	this->applyForce(-force); // Current actor & negative input force
}
