#pragma once
#include <glm/ext.hpp>
#include <Gizmos.h>

using namespace glm;
using namespace aie;

class setupContinuousDemo
{
public:
	setupContinuousDemo();
	~setupContinuousDemo();

	void setUpDemo(vec2 startPos, float inclination, float speed, float gravity);

protected:
	float x;
	float y;
};

