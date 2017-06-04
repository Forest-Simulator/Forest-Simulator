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
	// glPushMatrix();
		float d = dot(vec3(0,0,1), heading);
		float a = degrees(acos(d));

		vec3 c = cross(vec3(0,0,1), heading);

		glRotatef(a, c.x, c.y, c.z);

		cgraCylinder(radius, radius, length);
	// glPopMatrix();

	vec3 move = heading * length;
	cout << move << endl;

	// glTranslatef(move.x, move.y, move.z);
}

void Tree::rotateLeft() {
	glRotatef(-angle, 0.0, 1.0, 0.0);
}

void Tree::rotateRight() {
	glRotatef(angle, 0.0, 1.0, 0.0);
}

void Tree::render() {
	// glRotatef(-90, 1.0, 0.0, 0.0);
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