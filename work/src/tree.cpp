#include <iostream>
#include <string>
#include <vector>

#include "opengl.hpp"
#include "cgra_math.hpp"
#include "cgra_geometry.hpp"
#include "tree.hpp"

using namespace cgra;
using namespace std;
using namespace tree;

Tree::Tree(vector<string> s, float a, float l) {
	strings = s;
	angle = a;
	length = l;
	heading = vec3(0, 0, 1);
	up = vec3(0, 1, 0);
	left = vec3(1, 0, 0);

	functionMap = {
		{'F', &Tree::drawForward},
		{'G', &Tree::drawForward},
		{'[', &Tree::pushMatrix},
		{']', &Tree::popMatrix},
		{'+', &Tree::turnLeft},
		{'-', &Tree::turnRight},
		{'^', &Tree::pitchUp},
		{'&', &Tree::pitchDown},
		{'\\', &Tree::rollLeft},
		{'/', &Tree::rollRight}
	};
}

void Tree::drawForward() {
	float radius = 0.03;
	
	float d = dot(vec3(0, 0, 1), heading);
	float a = degrees(acos(d));

	vec3 c = cross(vec3(0, 0, 1), heading);

	glRotatef(a, c.x, c.y, c.z);

	cgraCylinder(radius, radius, length);

	vec3 move = heading * length;
	glTranslatef(move.x, move.y, move.z);

	glRotatef(-a, c.x, c.y, c.z);
}

void Tree::turnLeft() {
	glRotatef(-angle, up.x, up.y, up.z);
}

void Tree::turnRight() {
	glRotatef(angle, up.x, up.y, up.z);
}

void Tree::pitchUp() {
	glRotatef(-angle, left.x, left.y, left.z);
}

void Tree::pitchDown() {
	glRotatef(angle, left.x, left.y, left.z);
}

void Tree::rollLeft() {
	glRotatef(-angle, heading.x, heading.y, heading.z);
}

void Tree::rollRight() {
	glRotatef(angle, heading.x, heading.y, heading.z);
}

void Tree::pushMatrix() {
	glPushMatrix();
}

void Tree::popMatrix() {
	glPopMatrix();
}

void tMaterial() {
	GLfloat mat_specular[] = { 0.51, 0.32, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.51, 0.32, 0.0, 1.0 };
	GLfloat mat_shininess[] = { 30.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void redMaterial() {
	GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat mat_shininess[] = { 30.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void blueMaterial() {
	GLfloat mat_specular[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 30.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void greenMaterial() {
	GLfloat mat_specular[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat mat_shininess[] = { 30.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void drawAxis() {
	float radius = 0.02;
	float axisLength = 10.0;

	// Y Axis
	glPushMatrix();
		redMaterial();
		glColor3f(1.0, 0.0, 0.0);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		cgraCylinder(radius, radius, axisLength);
	glPopMatrix();

	// X Axis
	glPushMatrix();
		blueMaterial();
		glColor3f(0.0, 0.0, 1.0);
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		cgraCylinder(radius, radius, axisLength);
	glPopMatrix();

	// Z Axis
	glPushMatrix();
		greenMaterial();
		glColor3f(0.0, 0.0, 1.0);
		cgraCylinder(radius, radius, axisLength);
	glPopMatrix();

	tMaterial();
}

void Tree::render() {
	string s = strings.back();
	drawAxis();

	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	for(int i = 0; i < int(s.size()); i++) {
		char c = s.at(i);

		// Get the corresponding function for character
		// c and call it on this object
		RenderFunction func = functionMap.at(c);
		(this->*func)();
	}
	glPopMatrix();
}