#pragma once

#include "cgra_math.hpp"
#include "opengl.hpp"

using namespace std;
using namespace cgra;

class Boid{

private:
	cgra::vec3 position;
	cgra::vec3 velocity;
	cgra::vec3 destination;

	GLuint m_displayListPoly = 0; // DisplayList for Polygon

	void pyramid();

public:
	Boid(cgra::vec3 position);
	void render();
};
