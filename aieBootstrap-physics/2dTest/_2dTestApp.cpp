#include "_2dTestApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <glm/ext.hpp>
#include <Gizmos.h>
#include <glm/glm.hpp>


_2dTestApp::_2dTestApp() {

}

_2dTestApp::~_2dTestApp() {

}

bool _2dTestApp::startup() {
	
	//increase the 2d line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(vec2(0, -9.8f));
	m_physicsScene->setTimeStep(0.01f);

	Timer = 0;
	
	//Sphere* ball;
	//ball = new Sphere(vec2(-14, 0), vec2(25,0), 300.0f, 4, vec4(1, 1, 1, 1));
	//
	//Sphere* ball2;
	//ball2 = new Sphere(vec2(14, 0), vec2(-25, 0), 300.0f, 4, vec4(0.3f, 0.3f, 0.3f, 1));
	//
	//m_physicsScene->addActor(ball);
	//m_physicsScene->addActor(ball2);

	////ball->applyForceToActor(ball2, vec2(2, 0));
	//ball->applyForce(vec2(30, 0));
	//ball2->applyForce(vec2(30, 0));

	
	rocket = new Sphere(vec2(0, -25), vec2(0, 0), 100.0f, 8, vec4(0, 1, 1, 1));
	m_physicsScene->addActor(rocket);
	

	return true;
}

void _2dTestApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void _2dTestApp::update(float deltaTime) {

	// input example
	Input* input = Input::getInstance();

	Gizmos::clear();
	
	Timer += deltaTime;
	
	if (Timer > 0.01f)
	{
		if (input->isKeyDown(INPUT_KEY_SPACE) && rocket->getMass() > 0.0f)
		{
			Sphere* gas;
			gas = new Sphere((rocket->getPosition() - vec2(0, 12.0f)), vec2(0, 0.0f), 1.0f, 3, vec4(0.38f, 0.12f, 0.5f, 1));
			m_physicsScene->addActor(gas);
			rocket->setMass(rocket->getMass() - gas->getMass());

			vec2 mouse(input->getMouseX(), input->getMouseY());

			float mag = sqrt((mouse.x * mouse.x) + (mouse.y * mouse.y));

			mouse.x = mouse.x / mag;
			mouse.y = mouse.y / mag;



			gas->applyForceToActor(rocket, vec2(0, 200.0f));
		
		}
		if(input->isKeyUp(INPUT_KEY_SPACE))
			rocket->setMass(rocket->getMass() + 1.0f);

		Timer = 0;
	}

	/*if (input->isKeyDown(INPUT_KEY_LEFT))
		rocket->applyForce(vec2(-100.f, 0));

	if (input->isKeyDown(INPUT_KEY_RIGHT))
		rocket->applyForce(vec2(100.f, 0));
*/
	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void _2dTestApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);
	cout << rocket->getMass() << endl;

	// done drawing sprites
	m_2dRenderer->end();
}