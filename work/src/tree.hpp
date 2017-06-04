#include <map>

#include "cgra_math.hpp"

namespace tree {

	// Forward declare Tree so pointer methods
	// can be constructed
	class Tree;

	typedef void (Tree::*RenderFunction)(void);

	class Tree {
	private:
		float angle;
		float length;
		std::vector<std::string> strings;
		std::map<char, RenderFunction> functionMap;
		cgra::vec3 heading;
		cgra::vec3 up;
		cgra::vec3 left;

		void drawForward();
		void turnLeft();
		void turnRight();
		void pitchUp();
		void pitchDown();
		void rollLeft();
		void rollRight();
		void pushMatrix();
		void popMatrix();
	public:
		Tree(std::vector<std::string>, float, float);
		void render();
	};
}