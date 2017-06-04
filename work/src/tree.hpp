#include "cgra_math.hpp"

namespace tree {

	class Tree {
	private:
		float angle;
		float length;
		std::vector<std::string> strings;
		cgra::vec3 heading;
		

		void drawForward();
		void rotateLeft();
		void rotateRight();
	public:
		Tree(std::vector<std::string>, float, float);
		void render();
	};

}