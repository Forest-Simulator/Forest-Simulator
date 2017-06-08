#pragma once

#include <string>
#include <vector>

namespace lsys {

	struct RuleContext {
		char left = 'Z';
		char right = 'Z';
	};

	class Rule {
	private:
		char character;
		RuleContext context;
		char invalid = 'Z';
		float chance = 0.0;

		bool isValid(char);
	public:
		std::string transform;

		Rule();
		Rule(char, std::string);
		Rule(char, std::string, float);
		Rule(char, std::string, RuleContext);
		~Rule();
		bool matches(char, char, char);
		void print();
	};


	class LSystem {
	private:
		std::vector<std::string> strings;
		std::vector<Rule> rules;
		std::string currentString = "";
		std::string nextString = "";
	public:
		LSystem();
		LSystem(std::string, std::vector<Rule>);
		~LSystem();
		std::vector<std::string> generate();
	};
}