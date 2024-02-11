#pragma once
#include "PhysicsObject.h"
#include <glm/glm.hpp>

class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType shapeID, glm::vec3 position,
		glm::vec2 velocity, float orientation,
		float mass);
	~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	void applyForce(glm::vec2 force);
	void applyForceToActor(RigidBody* actor2, glm::vec2 force);

	glm::vec2 getPosition() { return m_position; }
	float getOrientation() { return m_orientation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation; //2D so we only need a single float to represent orientation
};

