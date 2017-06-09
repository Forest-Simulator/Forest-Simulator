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

Tree::Tree() {
	
}

Tree::Tree(vector<string> s, float a, float l) {
	strings = s;
	angle = a;
	currentState.length = l;
	heading = vec3(0, 0, 1);
	up = vec3(0, 1, 0);
	left = vec3(1, 0, 0);

	functionMap = {
		{'F', &Tree::drawBranch},
		// {'f', &Tree::moveForward},
		{'S', &Tree::drawLeaf},
		{'[', &Tree::pushMatrix},
		{']', &Tree::popMatrix},
		{'+', &Tree::turnLeft},
		{'-', &Tree::turnRight},
		{'^', &Tree::pitchUp},
		{'&', &Tree::pitchDown},
		{'\\', &Tree::rollLeft},
		{'/', &Tree::rollRight},
		{'|', &Tree::turnAround},
		{',', &Tree::increaseColourIndex},
		{';', &Tree::decreaseColourIndex},
		{'#', &Tree::increaseLineWidth},
		{'!', &Tree::decreaseLineWidth}
	};
}

void Tree::createDisplayList() {
	/*displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);
	// glBegin(GL_TRIANGLES);

	string s = strings.back();

	// glRotatef(-90, 1.0, 0.0, 0.0);
	for(int i = 0; i < int(s.size()); i++) {
		char c = s.at(i);
		// Get the corresponding function for character
		// c and call it on this object
		RenderFunction func = functionMap.at(c);
		(this->*func)();
	}
	
	// glEnd();
	glEndList();*/
}

void Tree::render() {
	// Ideally want to do this here:
	// glPushMatrix();
		// glRotatef(-90, 1.0, 0.0, 0.0);
		// glCallList(displayList);
	// glPopMatrix();

	string s = strings.back();

	// cout << s << endl;

	// glLineWidth(10.0);
	// glBegin(GL_LINES);
	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	for(int i = 0; i < int(s.size()); i++) {
		char c = s.at(i);

		// Get the corresponding function for character
		// c and call it on this object
		if(functionMap.find(c) != functionMap.end()) {
			RenderFunction func = functionMap.at(c);
			(this->*func)();
		}
	}
	glPopMatrix();
}

vec3 translate(vec3 start, vec3 translation) {
	vec4 newStart = vec4(start.x, start.y, start.z, 1);
	mat4 translationMatrix = mat4(
		vec4(1, 0, 0, translation.x),
		vec4(0, 1, 0, translation.y),
		vec4(0, 0, 1, translation.z),
		vec4(0, 0, 0, 1)
	);

	vec3 ns = vec3(
		dot(newStart, translationMatrix[0]),
		dot(newStart, translationMatrix[1]),
		dot(newStart, translationMatrix[2])
	);

	return ns;
}

vec3 rotate(vec3 start, vec3 rotation, float angle) {
	vec4 newStart = vec4(start.x, start.y, start.z, 1);
	mat4 rotationMatrix = mat4(
		vec4(cos(angle), 0, sin(angle), 0),
		vec4(0, 1, 0, 0),
		vec4(-sin(angle), 0, cos(angle), 0),
		vec4(0, 0, 0, 1)
	);

	vec3 ns = vec3(
		dot(newStart, rotationMatrix[0]),
		dot(newStart, rotationMatrix[1]),
		dot(newStart, rotationMatrix[2])
	);

	// cout << start << ", " << ns << endl;

	return ns;
}

void Tree::drawBranch() {
	headingBegin();

	float radius = 0.05;

	tMaterial();

	// vec3 posStart = currentState.position;

	vec3 move = heading * currentState.length;

	// vec3 posEnd = translate(posStart, move);
	// currentState.position = posEnd;


	// glBegin(GL_LINES);

	// glVertex3f(posStart.x, posStart.y, posStart.z);
	// glVertex3f(posEnd.x, posEnd.y, posEnd.z);

	// glEnd();
	
	cgraCylinder(radius, radius, currentState.length, 2, 1);
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

void Tree::moveForward() {
	// headingBegin();

	vec3 move = heading * currentState.length;
	glTranslatef(move.x, move.y, move.z);

	// headingEnd();
}

void Tree::turnLeft() {
	vec3 rotStart = currentState.heading;
	vec3 rotEnd = rotate(rotStart, up, angle);
	currentState.heading = rotEnd;

	// glRotatef(angle, up.x, up.y, up.z);
}

void Tree::turnRight() {
	vec3 rotStart = currentState.heading;
	vec3 rotEnd = rotate(rotStart, up, -angle);
	currentState.heading = rotEnd;

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
	stateStack.push(currentState);
}

void Tree::popMatrix() {
	currentState = stateStack.top();
	stateStack.pop();
	glPopMatrix();
}

void Tree::increaseColourIndex() {
	int colourMapSize = 5;
	int newColourIndex = currentState.colourIndex + 1;
	if(newColourIndex < colourMapSize) currentState.colourIndex = newColourIndex;
}

void Tree::decreaseColourIndex() {
	int newColourIndex = currentState.colourIndex - 1;
	if(newColourIndex >= 0) currentState.colourIndex = newColourIndex;
}

void Tree::increaseLineWidth() {
	float newLength = currentState.length / 0.8;
	if(newLength < 4.0) currentState.length = newLength;
}

void Tree::decreaseLineWidth() {
	float newLength = currentState.length * 0.8;
	if(newLength > 0.2) currentState.length = newLength;
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