#include "PhysicsScene.h"

#include "PhysicsObject.h"

PhysicsScene::PhysicsScene()
{
    setGravity(glm::vec2(0, 0));
    setTimeStep(0.01f);
}

void PhysicsScene::update(float dt)
{
    // update physics at a fixed time step

    static float accumulatedTime = 0.0f;
    accumulatedTime += dt;

    while(accumulatedTime >= m_timeStep)
    {
        for(auto pActor : m_actors)
        {
            pActor->fixedUpdate(m_gravity, m_timeStep);
        }

        accumulatedTime -= m_timeStep;
    }
}

void PhysicsScene::draw()
{
    for(auto pActor : m_actors)
    {
        pActor->draw();
    }
}
