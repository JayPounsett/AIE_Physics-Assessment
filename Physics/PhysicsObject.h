#pragma once
#include <glm/vec2.hpp>

enum ShapeType {
    PLANE = 0,
    SPHERE,
    BOX
};

class PhysicsObject
{
protected:
    PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID){}

public:
    virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
    virtual void draw() = 0;
    virtual void resetPosition(){}

protected:
    ShapeType m_shapeID;
};