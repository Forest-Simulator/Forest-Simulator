#pragma once

#include <string>
#include <vector>

namespace lsys {

	struct RuleContext {
		char left;
		char right;
	};

	class Rule {
	private:
		char character;
		RuleContext context;

	public:
		char transform;
		
		Rule(char, char);
		Rule(char, char, RuleContext);
		bool matches(char, char, char);
	};


	class LSystem {
	private:
		std::vector<std::string> strings;
		std::vector<Rule> rules;
		std::string currentString = "";
		std::string nextString = "";
	public:
		LSystem(std::string, std::vector<Rule>);
		std::vector<std::string> generate();
	};
}