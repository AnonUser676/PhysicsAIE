#include "_2dTestApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <glm/ext.hpp>
#include <Gizmos.h>
#include <Math.h>
#include <gl_core_4_4.h>
#include <imgui.h>


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
	m_physicsScene->setGravity(vec2(0, -10));
	m_physicsScene->setTimeStep(0.005f);

	m_ContinousDemo = new setupContinuousDemo();

	m_ContinousDemo->setUpDemo(vec2(-30, 0), 45.0f, 35.0f, -10.0f);
	
	float radius = 1.0f;
	float speed = 35.0f;
	vec2 startPos(-30, 0);
	float inclination = 45.0f;
		
	Sphere* projectile;
	projectile = new Sphere(startPos, inclination, speed, 10.0f, radius, vec4(1, 0, 0, 1));

	m_physicsScene->addActor(projectile);


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

	
	/*rocket = new Sphere(vec2(0, -25), vec2(0, 0), 100.0f, 8, vec4(0, 1, 1, 1));
	m_physicsScene->addActor(rocket);
	*/

	return true;
}

void _2dTestApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void _2dTestApp::update(float deltaTime) {

	imgui();
	// input example
	Input* input = Input::getInstance();

	//Gizmos::clear();
	
	Timer += deltaTime;
	
	if (Timer > 0.1f)
	{
		/*if (input->isKeyDown(INPUT_KEY_SPACE) && rocket->getMass() > 0.0f)
		{
			Sphere* gas;
			gas = new Sphere((rocket->getPosition() - vec2(0, 12.0f)), vec2(0, 0.0f), 1.0f, 3, vec4(0.38f, 0.12f, 0.5f, 1));
			m_physicsScene->addActor(gas);
			rocket->setMass(rocket->getMass() - gas->getMass());

			gas->applyForceToActor(rocket, vec2(0, 100.0f));
				
			
		}*/
		Timer = 0;
	}

	/*if (input->isKeyDown(INPUT_KEY_LEFT))
		rocket->applyForce(vec2(-100.f, 0));

	if (input->isKeyDown(INPUT_KEY_RIGHT))
		rocket->applyForce(vec2(100.f, 0));*/

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
	Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
		
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void _2dTestApp::imgui()
{
	vec4 m_clearColor(1, 1, 1, 1);

	ImGui::Begin("Color");
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, 1);
	ImGui::ColorEdit3("clear color", value_ptr(m_clearColor));

	ImGui::End();
}