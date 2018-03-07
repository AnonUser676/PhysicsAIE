#include "_2dTestApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <glm/ext.hpp>
#include <Gizmos.h>
#include <Math.h>
#include <gl_core_4_4.h>
#include <imgui.h>

//using namespace ImGui;

_2dTestApp::_2dTestApp() {
	
}

_2dTestApp::~_2dTestApp() {

}

bool _2dTestApp::startup() {
	
	//increase the 2d line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 6553500U);

	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(vec2(0.0f, -10.0f));
	m_physicsScene->setTimeStep(0.005f);

	Timer = 0;
	
	wall = new Plane(vec2 (0,1),-30);
	wall2 = new Plane(vec2(1, 1), -40);
	wall3 = new Plane(vec2(1, 1), -30);
	wall4 = new Plane(vec2(-1, 1), -30);
	wall5 = new Plane(vec2(0, 1), -30);
	objectBox1 = new Box(vec2(10, 30), 5, 15, vec2(0, 0), 10.0f, 0.01f, 0.1f, 0.99f, vec4(0, 1, 0, 1));
	objectBox2 = new Box(vec2(10, -40), 5, 6, vec2(0, 5.0f), 10.0f, 0.01f, 0.1f, 0.99f, vec4(0, 1, 0, 1));
	m_physicsScene->addActor(objectBox1);
	m_physicsScene->addActor(objectBox2);
	m_physicsScene->addActor(wall);
	//m_physicsScene->addActor(wall2);
	m_physicsScene->addActor(wall3);
	m_physicsScene->addActor(wall4);
	//m_physicsScene->addActor(wall5);
	
	gravity = m_physicsScene->getGravity().y;

	SpherePosX = 0;
	SpherePosY = 0;
	SphereVelX = 0;
	SphereVelY = 0;
	SphereMass = 1;
	Radius = 1;
	SphereLinearDrag = 0.01f;
	SphereElasticity = 0.01;
	sphereStatic = false;
	SphereColor = vec4(0,0,0,1);
	circleCounter = 0;

	AABBPosX = 0.0f;
	AABBPosY = 0.0f;
	AABBVelX = 0.0f;
	AABBVelY = 0.0f;
	AABBMass = 1.0f;
	AABBLength = 1.0f;
	AABBHeight = 1.0f;
	AABBLinearDrag = 0.01f;
	AABBElasticity = 0.01f;
	AABBStatic = false;
	AABBColor = vec4(0, 0, 0, 1);

	aabbCounter = 0;

	return true;
}

void _2dTestApp::shutdown() 
{
	delete m_font;
	delete m_2dRenderer;
	delete wall;
	delete wall2;
	delete wall3;
	delete wall4;
	delete wall5;
	delete objectBox1;
	delete objectBox2;
	Gizmos::destroy();

	if (circle.size() > 0)
	{
		while(circle.size() > 0)
		{
			m_physicsScene->removeActor(circle[circle.size()]);
			delete circle[circle.size()];
		}
	}

	if (aabb.size() > 0)
	{
		for (int i = 0; i < aabb.size(); i--)
		{
			//m_physicsScene->removeActor(aabb[i]);
			//delete aabb[i];
		}
	}

}

void _2dTestApp::update(float deltaTime) {

	
	// input example
	Input* input = Input::getInstance();

	imgui();

	Gizmos::clear();
	
	Timer += deltaTime;
	
	if (Timer > 0.1f)
		Timer = 0;

	cout << m_physicsScene->actorCounter() << endl;
	
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
	
	ImGui::Begin("Physics Manipulator");

	ImGui::SliderFloat("Gravity", &gravity, -400.0f, 10.0f);
	if (ImGui::Button("Set Gravity"))
	{
		m_physicsScene->setGravity(vec2(0, gravity));
	}
	ImGui::Separator();

	ImGui::Spacing();
	
	if (ImGui::CollapsingHeader("Sphere", 11.0f))
	{
		ImGui::SliderFloat("Sphere Position X", &SpherePosX, -600.0f, 600.0f);
		ImGui::SliderFloat("Sphere Position Y", &SpherePosY, -600.0f, 600.0f);
		ImGui::InputInt("Sphere Velocity x", &SphereVelX);
		ImGui::InputInt("Sphere Velocity y", &SphereVelY);
		ImGui::SliderFloat("Sphere Mass", &SphereMass, 0.01f, 10000.0f);
		ImGui::SliderFloat("Sphere Radius", &Radius, 0.1f, 1000.0f);
		ImGui::InputFloat("Sphere Linear Drag", &SphereLinearDrag, 0.01f);
		ImGui::InputFloat("Sphere Elasticity", &SphereElasticity);
		ImGui::Checkbox("is Sphere Static", &sphereStatic);

		if (ImGui::Button("Create Sphere"))
		{
			circle.push_back(new Sphere(vec2(SpherePosX, SpherePosY), vec2(SphereVelX, SphereVelY), SphereMass, Radius, SphereLinearDrag, 0.3, SphereElasticity, vec4(rand() % 100 * 0.01f, rand() % 100 * 0.01f, rand() % 100 * 0.01f, 1)));
			m_physicsScene->addActor(circle[circleCounter]);
			circle[circleCounter]->setKinematic(sphereStatic);
			circleCounter = circle.size();
		}

		if (ImGui::Button("Destroy Sphere"))
		{
			if (circleCounter > 0)
			{
				m_physicsScene->removeActor(circle[circleCounter - 1]);
				delete circle[circleCounter - 1];
				circleCounter--;
			}
		}
	}
	ImGui::Separator();
	ImGui::Separator();

	ImGui::Spacing();

	if (ImGui::CollapsingHeader("AABB Box", 11.0f))
	{
		ImGui::SliderFloat("AABB Position X", &AABBPosX, -600.0f, 600.0f);
		ImGui::SliderFloat("AABB Position Y", &AABBPosY, -600.0f, 600.0f);
		ImGui::InputInt("AABB Velocity x", &AABBVelX);
		ImGui::InputInt("AABB Velocity y", &AABBVelY);
		ImGui::SliderFloat("AABB Mass", &AABBMass, 0.01f, 10000.0f);
		ImGui::SliderFloat("AABB Length", &AABBLength, 0.1f, 1000.0f);
		ImGui::SliderFloat("AABB Height", &AABBHeight, 0.1f, 1000.0f);
		ImGui::InputFloat("AABB Linear Drag", &AABBLinearDrag, 0.01f);
		ImGui::InputFloat("AABB Elasticity", &AABBElasticity);
		ImGui::Checkbox("is AABB Static", &AABBStatic);

		if (ImGui::Button("Create AABB"))
		{
			aabb.push_back(new AABB(vec2(AABBPosX, AABBPosY), AABBLength, AABBHeight, vec2(AABBVelX, AABBVelY), AABBMass, AABBLinearDrag, 0.3, AABBElasticity, vec4(rand() % 100 * 0.01f, rand() % 100 * 0.01f, rand() % 100 * 0.01f, 1)));
			m_physicsScene->addActor(aabb[aabbCounter]);
			aabb[aabbCounter]->setKinematic(AABBStatic);
			aabbCounter = aabb.size();
		}

		if (ImGui::Button("Destroy AABB"))
		{
			if (aabbCounter > 0)
			{
				m_physicsScene->removeActor(aabb[aabbCounter - 1]);
				delete aabb[aabbCounter - 1];
				aabbCounter--;
			}
		}
	}
	ImGui::Separator();

	ImGui::End();
}