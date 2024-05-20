#include "PhysicsScene.h"

#include "Box.h"
#include "PhysicsObject.h"
#include "Plane.h"
#include "Sphere.h"

PhysicsScene::PhysicsScene() : m_gravity(glm::vec2(0, 0)), m_timeStep(0.01f) {}

PhysicsScene::~PhysicsScene() {
  for (auto pActor : m_actors) {
    delete pActor;
  }
}

void PhysicsScene::addActor(PhysicsObject* actor) { m_actors.push_back(actor); }

void PhysicsScene::removeActor(PhysicsObject* actor) {
  auto it = std::find(m_actors.begin(), m_actors.end(), actor);
  if (it != m_actors.end()) {
    m_actors.erase(it);
  }
}

// function pointer array for doing our collisions
typedef bool (*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] = {
  PhysicsScene::plane2Plane,
  PhysicsScene::plane2Sphere,
  PhysicsScene::plane2Box,
  PhysicsScene::sphere2Plane,
  PhysicsScene::sphere2Sphere,
  PhysicsScene::sphere2Box,
  PhysicsScene::box2Plane,
  PhysicsScene::box2Sphere,
  PhysicsScene::box2Box,
};

void PhysicsScene::update(float dt) {
  // update physics at a fixed time step
  static float accumulatedTime = 0.0f;
  accumulatedTime += dt;

  while (accumulatedTime >= m_timeStep) {
    for (auto pActor : m_actors) {
      pActor->fixedUpdate(m_gravity, m_timeStep);
    }

    accumulatedTime -= m_timeStep;

    checkForCollision();
  }
}

void PhysicsScene::draw() {
  for (auto pActor : m_actors) {
    pActor->draw();
  }
}

void PhysicsScene::checkForCollision() { 
  int actorCount = m_actors.size();

  // Need to check for collisions against all objects except this one
  for (int outer = 0; outer < actorCount - 1; outer++) {
    for (int inner = outer + 1; inner < actorCount; inner++) {
      PhysicsObject* object1 = m_actors[outer];
      PhysicsObject* object2 = m_actors[inner];
      int shapeId1 = object1->getShapeID();
      int shapeId2 = object2->getShapeID();

      // this check will ensure we don't include any joints in the collision
      // checks
      if (shapeId1 < 0 || shapeId2 < 0) {
        continue;
      }

      // using function pointers
      int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
      fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
      if (collisionFunctionPtr != nullptr) {
        // did a collision occur?
        collisionFunctionPtr(object1, object2);
      }
    }
  }
}

void PhysicsScene::ApplyContactForces(
  Rigidbody* body1, Rigidbody* body2, glm::vec2 norm, float pen) {
  float body2Mass = body2 ? body2->getMass() : INT_MAX;
  float body1Factor = body2Mass / (body1->getMass() + body2Mass);

  body1->setPosition(body1->getPosition() - body1Factor * norm * pen);
  if (body2)
    body2->setPosition(body2->getPosition() + (1 - body1Factor) * norm * pen);
}

bool PhysicsScene::plane2Plane(PhysicsObject*, PhysicsObject*) { return false; }

bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2) {
  // reverse the order of arguments, as obj1 is the plane and obj2 is the sphere
  return sphere2Plane(obj2, obj1);
}

bool PhysicsScene::plane2Box(PhysicsObject* obj1, PhysicsObject* obj2) {
  // Disabled to allow Cue to move over the pool table boundaries in simulation
    
  //Plane* plane = dynamic_cast<Plane*>(obj1);
  //Box* box = dynamic_cast<Box*>(obj2);

  //// if we are successful, then test for collision
  //if (box != nullptr && plane != nullptr) {
  //  int numContacts = 0;
  //  glm::vec2 contact(0, 0);
  //  float contactV = 0;

  //  // get a representative point on the plane
  //  glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();

  //  // check all four corners to see if we've hit the plane
  //  for (float x = -box->getExtents().x; x < box->getWidth();
  //       x += box->getWidth()) {
  //    for (float y = -box->getExtents().y; y < box->getHeight();
  //         y += box->getHeight()) {
  //      // get the position of the corner in world space
  //      glm::vec2 p =
  //        box->getPosition() + x * box->getLocalX() + y * box->getLocalY();
  //      float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());

  //      // this is the total velocity of the point in world space
  //      glm::vec2 displacement = x * box->getLocalX() + y * box->getLocalY();
  //      glm::vec2 pointVelocity =
  //        box->getVelocity() + box->getAngularVelocity() *
  //                               glm::vec2(-displacement.y, displacement.x);

  //      // and this is the component of the point velocity into the plane
  //      float velocityIntoPlane = glm::dot(pointVelocity, plane->getNormal());

  //      // and moving further in, we need to resolve the collision
  //      if (distFromPlane < 0 && velocityIntoPlane <= 0) {
  //        numContacts++;
  //        contact += p;
  //        contactV += velocityIntoPlane;
  //      }
  //    }
  //  }
  //  // we've had a hit - typically only two corners can contact
  //  if (numContacts > 0) {
  //    plane->resolveCollision(box, contact / (float)numContacts);
  //    return true;
  //  }
  //}
  return false;
}

bool PhysicsScene::sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2) {
  Sphere* sphere = dynamic_cast<Sphere*>(obj1);
  Plane* plane = dynamic_cast<Plane*>(obj2);
  // if we are successful then test for collision
  if (sphere != nullptr && plane != nullptr) {
    glm::vec2 collisionNormal = plane->getNormal();
    glm::vec2 contact =
      sphere->getPosition() + (collisionNormal * -sphere->getRadius());

    float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) -
                          plane->getDistance();

    float intersection = sphere->getRadius() - sphereToPlane;
    float velocityOutOfPlane =
      glm::dot(sphere->getVelocity(), plane->getNormal());
    if (intersection > 0 && velocityOutOfPlane < 0) {
      plane->resolveCollision(sphere, contact);
      return true;
    }
  }
  return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2) {
  // try to cast objects to sphere and sphere
  Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
  Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

  if (sphere1 != nullptr && sphere2 != nullptr) {
    // This will adjust sphere2's position very slightly up the Y axis if both
    // sphere's inhabit exactly the same coordinates.
    //
    // This is not perfect in the long run and should instead move around the
    // outside of the sphere for where the position should be.
    if (sphere1->getPosition() - sphere2->getPosition() == glm::vec2(0)) {
      sphere2->setPosition(sphere2->getPosition() + glm::vec2(0, 0.1f));
    }

    float dist = glm::length(sphere1->getPosition() - sphere2->getPosition());

    float penetration = sphere1->getRadius() + sphere2->getRadius() - dist;
    if (penetration > 0) {
      sphere1->resolveCollision(
        sphere2,
        (sphere1->getPosition() + sphere2->getPosition()) * 0.5f,
        nullptr,
        penetration);

      return true;
    }
  }
  return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2) {
  return box2Sphere(obj2, obj1);
}

bool PhysicsScene::box2Plane(PhysicsObject* obj1, PhysicsObject* obj2) {
  //return plane2Box(obj2, obj1);   // Disabled to allow Cue to move over the pool table boundaries in simulation
  return false;                 // If line above enabled, remove this line
}

bool PhysicsScene::box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2) {
  Box* box = dynamic_cast<Box*>(obj1);
  Sphere* sphere = dynamic_cast<Sphere*>(obj2);

  if (box != nullptr && sphere != nullptr) {
    // transform the circle into the box's coordinate space
    glm::vec2 circlePosWorld = sphere->getPosition() - box->getPosition();
    glm::vec2 circlePosBox = glm::vec2(
      glm::dot(circlePosWorld, box->getLocalX()),
      glm::dot(circlePosWorld, box->getLocalY()));

    // find the closest point to the circle centre on the box by clamping the
    // coordinates in box-space to the box's extents
    glm::vec2 closestPointOnBoxBox = circlePosBox;
    glm::vec2 extents = box->getExtents();
    if (closestPointOnBoxBox.x < -extents.x)
      closestPointOnBoxBox.x = -extents.x;
    if (closestPointOnBoxBox.x > extents.x) closestPointOnBoxBox.x = extents.x;
    if (closestPointOnBoxBox.y < -extents.y)
      closestPointOnBoxBox.y = -extents.y;
    if (closestPointOnBoxBox.y > extents.y) closestPointOnBoxBox.y = extents.y;

    // and convert back into world coordinates
    glm::vec2 closestPointOnBoxWorld =
      box->getPosition() + closestPointOnBoxBox.x * box->getLocalX() +
      closestPointOnBoxBox.y * box->getLocalY();

    // Checking if box and sphere at in EXACTLY the same coordinate,
    // Amend sphere to be on top of box by adding extents.y to its position
    if (sphere->getPosition() - closestPointOnBoxWorld == glm::vec2(0)) {
      sphere->setPosition(sphere->getPosition() + glm::vec2(0, extents.y));
    }

    glm::vec2 circleToBox = sphere->getPosition() - closestPointOnBoxWorld;

    float penetration = sphere->getRadius() - glm::length(circleToBox);

    if (penetration > 0) {
      glm::vec2 direction = glm::normalize(circleToBox);

      glm::vec2 contact = closestPointOnBoxWorld;
      box->resolveCollision(sphere, contact, &direction, penetration);
    }
  }
  return false;
}

bool PhysicsScene::box2Box(PhysicsObject* obj1, PhysicsObject* obj2) {
  Box* box1 = dynamic_cast<Box*>(obj1);
  Box* box2 = dynamic_cast<Box*>(obj2);
  if (box1 != nullptr && box2 != nullptr) {
    glm::vec2 boxPos = box2->getPosition() - box1->getPosition();
    glm::vec2 norm(0, 0);
    glm::vec2 contact(0, 0);
    float pen = 0.0f;
    int numContacts = 0;

    if (boxPos == glm::vec2(0)) {
      box2->setPosition(
        box2->getPosition() + glm::vec2(0, box1->getExtents().y));
    }

    box1->checkBoxCorners(*box2, contact, numContacts, pen, norm);
    if (box2->checkBoxCorners(*box1, contact, numContacts, pen, norm)) {
      norm = -norm;
    }
    if (pen > 0) {
      box1->resolveCollision(box2, contact / float(numContacts), &norm, pen);
    }
    return true;
  }
  return false;
}
