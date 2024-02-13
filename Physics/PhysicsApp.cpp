#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/ext.hpp>

PhysicsApp::PhysicsApp() {

}

PhysicsApp::~PhysicsApp() {

}

bool PhysicsApp::startup() {
	// increase the 2D line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();
	
	m_font = new aie::Font("./font/consolas.ttf", 32);

	//Initialise the physics scene
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0,0));
	m_physicsScene->setTimeStep(0.01f);

#pragma region Disabled Test Actors
	#pragma region Demonstrating Newton's Third Law
	//Sphere* ball1 = new Sphere(vec2(-4, 0), vec2(0, 0), 4.0f, 4, vec4(1, 0, 0, 1));
	//Sphere* ball2 = new Sphere(vec2(4, 0), vec2(0, 0), 4.0f, 4, vec4(0, 1, 0, 1));

	//m_physicsScene->addActor(ball1);
	//m_physicsScene->addActor(ball2);

	//ball1->applyForceToActor(ball2, vec2(-2, 0));
	#pragma endregion
	#pragma region Simulate Collision
	//Sphere* ball1 = new Sphere(glm::vec2(-20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	//Sphere* ball2 = new Sphere(glm::vec2(10, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	//m_physicsScene->addActor(ball1);
	//m_physicsScene->addActor(ball2);

	//ball1->applyForce(glm::vec2(30, 0));
	//ball2->applyForce(glm::vec2(-15, 0));
	#pragma endregion
	#pragma region Simulating a Rocket Motor
	/*float m = 20.0f;
	Sphere* rocketMotor = new Sphere(glm::vec2(0,0), glm::vec2(0), m, 3, glm::vec4(1, 0, 0, 1));
	Sphere* exhaustGas = new Sphere(glm::vec2(0,-4), glm::vec2(0), m, 0.1, glm::vec4(0, 1, 0, 1));
	m_physicsScene->addActor(rocketMotor);
	m_physicsScene->addActor(exhaustGas);
	
	while (m > 0) {
		exhaustGas->applyForceToActor(rocketMotor, glm::vec2(0, 0.1));
		m -= 0.1;
	}*/
	#pragma endregion
#pragma endregion

	return true;
}

void PhysicsApp::shutdown() {
	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime) {
	// input example
	aie::Input* input = aie::Input::getInstance();

	//aie::Gizmos::clear();
	
	m_physicsScene->update(deltaTime);
	m_physicsScene->draw();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// demonstrate animation
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}