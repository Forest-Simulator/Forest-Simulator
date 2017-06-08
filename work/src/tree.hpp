#pragma once

#include <map>
#include <stack>

#include "opengl.hpp"
#include "cgra_math.hpp"

namespace tree {

	// Forward declare Tree so that pointers to 
	// member-methods can be constructed
	class Tree;

	typedef void (Tree::*RenderFunction)(void);

	class Tree {
	private:
		float angle;
		float length;
		std::vector<std::string> strings;
		std::stack<float> lengthStack;
		std::map<char, RenderFunction> functionMap;
		cgra::vec3 heading;
		cgra::vec3 up;
		cgra::vec3 left;

		GLuint displayList = 0;

		void drawBranch();
		void drawLeaf();
		void moveForward();
		void turnLeft();
		void turnRight();
		void pitchUp();
		void pitchDown();
		void rollLeft();
		void rollRight();
		void pushMatrix();
		void popMatrix();
		void turnAround();

		void headingBegin();
		void headingEnd();
		cgra::vec3 getCrossProductOfZAxisAndHeading();
		float getAngleBetweenZAxisAndHeading();
		void createDisplayList();
	public:
		Tree();
		Tree(std::vector<std::string>, float, float);
		void render();
	};
}