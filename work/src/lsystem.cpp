#include <iostream>
#include <string>
#include <vector>

#include "lsystem.hpp"

using namespace lsys;
using namespace std;

Rule::Rule(char c, std::string t) {
	RuleContext rc = RuleContext();
	Rule(c, t, rc);
}

Rule::Rule(char c, std::string t, RuleContext rc) {
	character = c;
	transform = t;
	context = rc;
}

bool Rule::matches(char characterMatch, char leftMatch, char rightMatch) {
	bool charMatches = (characterMatch == character);
	bool leftMatches = (leftMatch == context.left);
	bool rightMatches = (rightMatch == context.right);

	cout << character << endl;
	cout << "left: " << context.left << ", right: " << context.right << endl;

	return charMatches;
}

LSystem::LSystem(string axiom, vector<Rule> r) {
	strings.push_back(axiom);
	rules = r;
	currentString = axiom;
}

vector<string> LSystem::generate() {
	for(int i = 0; i < int(currentString.size()); i++) {
		char c = currentString.at(i);
		char left;
		char right;

		if(i-1 > 0)
			left = currentString.at(i-1);
		if(i+1 < int(currentString.size()))
		 	right = currentString.at(i+1);

		bool ruleMatch = false;

		for(Rule r : rules) {
			if(r.matches(c, left, right)) {
				ruleMatch = true;
				nextString += r.transform;
				break;
			}
		}

		if(ruleMatch) {
			nextString += c;
		}
	}

	currentString = nextString;
	nextString = "";

	strings.push_back(currentString);

	return strings;
}