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

Tree::Tree() {
	
}

Tree::Tree(vector<string> s, float a, float l) {
	strings = s;
	angle = a;
	length = l;
	heading = vec3(0, 0, 1);
	up = vec3(0, 1, 0);
	left = vec3(1, 0, 0);

	functionMap = {
		{'F', &Tree::drawBranch},
		{'f', &Tree::moveForward},
		{'S', &Tree::drawLeaf},
		{'[', &Tree::pushMatrix},
		{']', &Tree::popMatrix},
		{'+', &Tree::turnLeft},
		{'-', &Tree::turnRight},
		{'^', &Tree::pitchUp},
		{'&', &Tree::pitchDown},
		{'\\', &Tree::rollLeft},
		{'/', &Tree::rollRight},
		{'|', &Tree::turnAround}
	};
}

void Tree::render() {
	string s = strings.back();
	// drawAxis();

	glMatrixMode(GL_MODELVIEW);

	// glLineWidth(10.0);
	// glBegin(GL_LINES);
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
	glEnd();
}

void Tree::moveForward() {
	headingBegin();

	vec3 move = heading * length;
	glTranslatef(move.x, move.y, move.z);

	headingEnd();
}

void Tree::turnLeft() {
	glRotatef(angle, up.x, up.y, up.z);
}

void Tree::turnRight() {
	glRotatef(-angle, up.x, up.y, up.z);
}

void Tree::pitchUp() {
	glRotatef(angle, left.x, left.y, left.z);
}

void Tree::pitchDown() {
	glRotatef(-angle, left.x, left.y, left.z);
}

void Tree::rollLeft() {
	glRotatef(angle, heading.x, heading.y, heading.z);
}

void Tree::rollRight() {
	glRotatef(-angle, heading.x, heading.y, heading.z);
}

void Tree::turnAround() {
	glRotatef(180.0, up.x, up.y, up.z);
}

void Tree::pushMatrix() {
	
	glPushMatrix();
	lengthStack.push(length);
	float newLength = length * 0.8;
	if(newLength > 0.2) length = newLength;
	
}

void Tree::popMatrix() {
	length = lengthStack.top();
	lengthStack.pop();
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

void Tree::drawBranch() {
	headingBegin();

	float radius = 0.05;

	tMaterial();
	cgraCylinder(radius, radius, length, 2, 1);

	vec3 move = heading * length;
	glTranslatef(move.x, move.y, move.z);

	headingEnd();
}

void Tree::drawLeaf() {
	headingBegin();

	float size = 0.2;

	glPushMatrix();
	greenMaterial();
	glBegin(GL_POLYGON);
		glVertex3f(-size, size, 0);
		glVertex3f(size, size, 0);
		glVertex3f(size, -size, 0);
		glVertex3f(-size, -size, 0);

	glEnd();
	glPopMatrix();

	headingEnd();
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

void Tree::headingBegin() {
	float a = getAngleBetweenZAxisAndHeading();
	vec3 c = getCrossProductOfZAxisAndHeading();

	glRotatef(a, c.x, c.y, c.z);
}

void Tree::headingEnd() {
	float a = getAngleBetweenZAxisAndHeading();
	vec3 c = getCrossProductOfZAxisAndHeading();

	glRotatef(-a, c.x, c.y, c.z);
}

vec3 Tree::getCrossProductOfZAxisAndHeading() {
	return cross(vec3(0, 0, 1), heading);
}

float Tree::getAngleBetweenZAxisAndHeading() {
	float d = dot(vec3(0, 0, 1), heading);
	return degrees(acos(d));
}