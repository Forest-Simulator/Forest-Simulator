#pragma once

#include <map>
#include <stack>

#include "opengl.hpp"
#include "cgra_math.hpp"

namespace tree {

	const cgra::vec3 up = cgra::vec3(0, 1, 0);
	const cgra::vec3 left = cgra::vec3(1, 0, 0);
	const cgra::vec3 towards = cgra::vec3(0, 0, 1);

	struct TreeState {
		float angle;
		float length;
		int colourIndex;
		cgra::vec3 heading = cgra::vec3(0, 0, 1);
		cgra::vec3 position;
	};

	// Forward declare Tree so that pointers to 
	// member-methods can be constructed
	class Tree;

	typedef void (Tree::*RenderFunction)(void);

	class Tree {
	private:
		
		std::vector<std::string> strings;

		TreeState state;
		std::stack<TreeState> stateStack;
		std::map<char, RenderFunction> functionMap;
		std::vector<cgra::vec3> branchVertices;
		std::vector<cgra::vec3> leafVertices;

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
		void pushState();
		void popState();
		void turnAround();
		void increaseColourIndex();
		void decreaseColourIndex();
		void increaseLineWidth();
		void decreaseLineWidth();

		void headingBegin();
		void headingEnd();
		cgra::vec3 getCrossProductOfZAxisAndHeading();
		float getAngleBetweenZAxisAndHeading();
		void createFromString();
		void createDisplayList();
	public:
		Tree();
		Tree(cgra::vec3, std::vector<std::string>, float, float);
		void render();
		std::vector<cgra::vec3> getBranchVertices();
	};
}