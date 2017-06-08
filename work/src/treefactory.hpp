#pragma once

#include <string>
#include <vector>

#include "lsystem.hpp"
#include "tree.hpp"

namespace tree {
	struct TreeGenerator {
		std::string name;
		std::string axiom;
		std::vector<float> startLength;
		float branchAngle;
		float probability;
		float generations;

		lsys::LSystem lsystem;
		std::vector<lsys::Rule> rules;
		TreeGenerator();
		Tree* generate();
	};

	class TreeFactory {
	private:
		std::vector<TreeGenerator> generators;
		void readFile(std::string);
	public:
		TreeFactory(std::string);
		Tree* generate();
	};
}