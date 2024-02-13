﻿#include "PhysicsScene.h"
#include "PhysicsObject.h"

PhysicsScene::PhysicsScene() : m_gravity(glm::vec2(0, 0)), m_timeStep(0.01f)
{
}

PhysicsScene::~PhysicsScene()
{
    for(auto pActor : m_actors)
    {
        delete pActor;
    }
}

void PhysicsScene::addActor(PhysicsObject* actor)
{
    m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
    auto it = std::find(m_actors.begin(), m_actors.end(), actor);
    if (it != m_actors.end()) { m_actors.erase(it); }
}

void PhysicsScene::update(float dt)
{
    static float accumulatedTime = 0.0f;
    accumulatedTime += dt;

    while(accumulatedTime >= m_timeStep)
    {
        for(auto pActor : m_actors)
        {
            pActor->fixedUpdate(m_gravity, m_timeStep);
        }

        accumulatedTime -= m_timeStep;

        // check for collisions (ideally you'd want to have some sort of
        // scene management in place
        int actorCount = m_actors.size();
        
        //Need to check for collisions against all objects except this one
        for(int outer = 0; outer < actorCount - 1; outer++)
        {
            for (int inner = outer + 1; inner < actorCount; inner++)
            {
                PhysicsObject* object1 = m_actors[outer];
                PhysicsObject* object2 = m_actors[inner];
        
                //for now we can assume both shapes are spheres,
                //since that is all we've implemented for now
                sphere2Sphere(object1, object2);
            }
        }
    }
}

void PhysicsScene::draw()
{
    for (auto pActor : m_actors) {
        pActor->draw();
    }
}

bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
    //try to cast objects to sphere and sphere
    const auto sphere1 = dynamic_cast<Sphere*>(obj1);
    const auto sphere2 = dynamic_cast<Sphere*>(obj2);

    if(sphere1 != nullptr && sphere2 != nullptr)
    {
        auto dist = glm::distance(sphere1->getPosition(), sphere2->getPosition());
        auto sum_radii = sphere1->getRadius() + sphere2->getRadius();
    
        //If the distance is less than the sum of both radii, a collision has occurred
        if(dist <= sum_radii)
        {
            sphere1->applyForce(-sphere1->getVelocity());
            sphere2->applyForce(-sphere2->getVelocity());
            return true;
        }       
    }
    return false;
}