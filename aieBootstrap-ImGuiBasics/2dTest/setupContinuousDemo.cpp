#include "setupContinuousDemo.h"

setupContinuousDemo::setupContinuousDemo() {};


setupContinuousDemo::~setupContinuousDemo()
{
}

void setupContinuousDemo::setUpDemo(vec2 startPos, float inclination, float speed, float gravity)
{
	float t = 0;
	float tStep = 0.5f;
	float radius = 1.0f;
	int segments = 12;
	vec4 colour = vec4(1, 1, 0, 1);

	while (t <= 5)
	{
		vec2 inc(cosf(inclination), sinf(inclination));
		
		x = startPos.x + (inc.x * speed * t);
		y = startPos.y + (inc.y * speed * t) + ((gravity * t*t) / 2.0f);

		Gizmos::add2DCircle(vec2(x,y), radius, segments, colour);
		t += tStep;
	}
}
