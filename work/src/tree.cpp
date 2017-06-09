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

	functionMap = {
		{'F', &Tree::drawBranch},
		// {'f', &Tree::moveForward},
		{'S', &Tree::drawLeaf},
		{'[', &Tree::pushState},
		{']', &Tree::popState},
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

	createDisplayList();
}

void Tree::createDisplayList() {
	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);

	string s = strings.back();

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
	
	glEndList();
}

void Tree::render() {
	glPushMatrix();
		glCallList(displayList);
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

mat4 makeRotationMatrix(vec3 rotationAxis, float angle) {
	// Ensure rotation axis is normalized
	vec3 u = normalize(rotationAxis);

	// Convert angle from degrees to radians
	float a = radians(angle);

	// Create the rotation matrix based on axis and angle.
	// See: https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
	mat4 rotationMatrix = mat4(
		vec4(cos(a) + (pow(u.x, 2) * (1 - cos(a))), u.x * u.y * (1 - cos(a)) - u.z * sin(a), u.x * u.y * (1 - cos(a)) + u.y * sin(a), 0.0f),
		vec4(u.y * u.x * (1 - cos(a)) + u.z * sin(a), cos(a) + pow(u.y, 2) * (1 - cos(a)), u.y * u.x * (1 - cos(a)) - u.x * sin(a), 0.0f),
		vec4(u.z * u.x * (1 - cos(a)) - u.y * sin(a), u.z * u.y * (1 - cos(a)) + u.x * sin(a), cos(a) + pow(u.z, 2) * (1 - cos(a)), 0.0f),
		vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);

	return rotationMatrix;
}

vec3 rotate(vec3 start, vec3 rotationAxis, float angle) {
	mat4 rotationMatrix = makeRotationMatrix(rotationAxis, angle);

	vec4 newStart = vec4(start.x, start.y, start.z, 1);

	vec3 ns = vec3(
		dot(newStart, rotationMatrix[0]),
		dot(newStart, rotationMatrix[1]),
		dot(newStart, rotationMatrix[2])
	);

	return ns;
}

void Tree::drawBranch() {
	tMaterial();

	vec3 posStart = currentState.position;
	vec3 move = currentState.heading * currentState.length;
	vec3 posEnd = translate(posStart, move);
	currentState.position = posEnd;


	glBegin(GL_LINES);

	glVertex3f(posStart.x, posStart.y, posStart.z);
	glVertex3f(posEnd.x, posEnd.y, posEnd.z);

	glEnd();
}

void Tree::drawLeaf() {
	float size = 0.2;
	vec3 pos = currentState.position;
	// vec3 rot = rotate(pos, currentState.heading, angle);

	// glPushMatrix();
	greenMaterial();
	glBegin(GL_POLYGON);
		glVertex3f(pos.x - size, pos.y + size, pos.z);
		glVertex3f(pos.x + size, pos.y + size, pos.z);
		glVertex3f(pos.x + size, pos.y - size, pos.z);
		glVertex3f(pos.x - size, pos.y - size, pos.z);

		// glVertex3f(rot.x - size, rot.y + size, rot.z);
		// glVertex3f(rot.x + size, rot.y + size, rot.z);
		// glVertex3f(rot.x + size, rot.y - size, rot.z);
		// glVertex3f(rot.x - size, rot.y - size, rot.z);
	glEnd();
	// glPopMatrix();
}

void Tree::moveForward() {
	vec3 posStart = currentState.position;
	vec3 move = currentState.heading * currentState.length;
	vec3 posEnd = translate(posStart, move);
	currentState.position = posEnd;
}

void Tree::turnLeft() {
	vec3 rotEnd = rotate(currentState.heading, up, angle);
	currentState.heading = rotEnd;
}

void Tree::turnRight() {
	vec3 rotEnd = rotate(currentState.heading, up, -angle);
	currentState.heading = rotEnd;
}

void Tree::pitchUp() {
	vec3 rotEnd = rotate(currentState.heading, left, angle);
	currentState.heading = rotEnd;
}

void Tree::pitchDown() {
	vec3 rotEnd = rotate(currentState.heading, left, -angle);
	currentState.heading = rotEnd;
}

void Tree::rollLeft() {
	// This is not correct. Want to rotate around heading rather
	// than z axis, but this is difficult.
	vec3 rotEnd = rotate(currentState.heading, towards, angle);
	currentState.heading = rotEnd;
}

void Tree::rollRight() {
	// This is not correct. Want to rotate around heading rather
	// than z axis, but this is difficult.
	vec3 rotEnd = rotate(currentState.heading, towards, -angle);
	currentState.heading = rotEnd;
}

void Tree::turnAround() {
	// Flips 180 degrees
	vec3 rotStart = currentState.heading;
	vec3 rotEnd = rotate(rotStart, up, 180.0);
	currentState.heading = rotEnd;
}

void Tree::pushState() {
	stateStack.push(currentState);
}

void Tree::popState() {
	currentState = stateStack.top();
	stateStack.pop();
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