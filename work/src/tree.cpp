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
	heading = vec3(0.0, 1.0, 0.0);
}

void Tree::drawForward() {
	float radius = 0.08;
	cgraCylinder(radius, radius, length);
	// glTranslatef();
}

void Tree::rotateLeft() {
	glRotatef(-angle, 0.0, 1.0, 0.0);
}

void Tree::rotateRight() {
	glRotatef(angle, 0.0, 1.0, 0.0);
}

void Tree::render() {
	glRotatef(-90, 1.0, 0.0, 0.0);
	string s = strings.back();
	for(int i = 0; i < int(s.size()); i++) {
		char c = s.at(i);
		if(c == 'F') {
			drawForward();
		} else if(c == 'G') {
			drawForward();
		} else if(c == '+') {
			rotateLeft();
		} else if(c == '-') {
			rotateRight();
		} else if(c == '[') {
			glPushMatrix();
		} else if(c == ']') {
			glPopMatrix();
		}
	}
}