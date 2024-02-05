#pragma once
#include <vector>
#include <glm/vec2.hpp>

class PhysicsObject;

class PhysicsScene
{
public:
    PhysicsScene();
    ~PhysicsScene();

    void addActor(PhysicsObject* actor);
    void removeActor(PhysicsObject* actor);

    // Update physical simulation
    void update(float dt); 

    // Drawing physical objects once per frame
    void draw(); 

    void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
    glm::vec2 getGravity() const { return m_gravity; }

    void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
    float getTimeStep() const { return m_timeStep; }

protected:
    glm::vec2 m_gravity;
    float m_timeStep;

    // Vector collection that stores all the physical objects in the scene,
    // along with functions to add and remove objects to and from this list.
    std::vector<PhysicsObject*> m_actors; 
};
