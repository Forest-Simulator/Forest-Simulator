#include "cgra_math.hpp"

namespace tree {

	class Tree {
	private:
		float angle;
		float length;
		std::vector<std::string> strings;
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
	public:
		Tree(std::vector<std::string>, float, float);
		void render();
	};

}