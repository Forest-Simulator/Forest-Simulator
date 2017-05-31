#include <cmath>
#include <cstdlib>

#include "cgra_math.hpp"
#include "opengl.hpp"
#include "boid.hpp"

using namespace std;
using namespace cgra;

Boid::Boid(vec3 pos){
	position = pos;
	pyramid();
}

void Boid::pyramid()
{
	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);

	// Create a new list
	cout << "Creating Poly Geometry" << endl;
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE);

	glBegin(GL_TRIANGLES);

	glVertex3f( 0.0f, 1.f, 0.0f );
	glVertex3f( -1.0f, -1.0f, 1.0f );
	glVertex3f( 1.0f, -1.0f, 1.0f);

	glVertex3f( 0.0f, 1.0f, 0.0f);
	glVertex3f( -1.0f, -1.0f, 1.0f);
	glVertex3f( 0.0f, -1.0f, -1.0f);

	glVertex3f( 0.0f, 1.0f, 0.0f);
	glVertex3f( 0.0f, -1.0f, -1.0f);
	glVertex3f( 1.0f, -1.0f, 1.0f);


	glVertex3f( -1.0f, -1.0f, 1.0f);
	glVertex3f( 0.0f, -1.0f, -1.0f);
	glVertex3f( 1.0f, -1.0f, 1.0f);

	glEnd();

	glEndList();
	cout << "Finished creating Poly Geometry" << endl;

}

void Boid::render(){
	glTranslatef(position.x, position.y, position.z);
	glColor3f(1.0, 0.0, 0.0);

	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCallList(m_displayListPoly);
}