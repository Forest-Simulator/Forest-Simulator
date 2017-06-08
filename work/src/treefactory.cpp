#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "lsystem.hpp"
#include "tree.hpp"
#include "treefactory.hpp"

using namespace lsys;
using namespace std;
using namespace tree;

TreeFactory::TreeFactory(string filename) {
	readFile(filename);
	
}

void TreeFactory::readFile(string filename) {
	ifstream objFile(filename);

	if(!objFile.is_open()) {
		cerr << "Error reading " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	// Values to save
	string name;
	string axiom;
	vector<float> startLength;
	float branchAngle;
	float probability;

	char match;
	string transform;

	Tree t;
	LSystem l;
	vector<Rule> rules;

	while(objFile.good()) {
		string line;
		std::getline(objFile, line);
		istringstream objLine(line);

		// Pulled from the file
		string key;
		vector<string> values;

		objLine >> key;

		while(objLine.good()) {
			string v;
			objLine >> v;
			values.push_back(v);
		}

		if(key == "start") {
			// cout << "start of tree" << endl;
		} else if(key == "name") {
			name = values[0];
		} else if(key == "axiom") {
			axiom = values[0];
		} else if(key == "startLength") {
			float lower = stof(values[0]);
			float upper = stof(values[1]);
			startLength.push_back(lower);
			startLength.push_back(upper);
		} else if(key == "branchAngle") {
			branchAngle = stof(values[0]);
		} else if(key == "probability") {
			probability = stof(values[0]);
		} else if(key == "rulestart") {
			rules.clear();
			// cout << "new rule match" << endl;
			cout << "start of rule" << endl;
		} else if(key == "match") {
			match = values[0][0];
		} else if(key == "transform") {
			transform = values[0];
		} else if(key == "ruleend") {
			Rule r = Rule(match, transform);
			rules.push_back(r);
		} else if(key == "end") {
			l = LSystem(axiom, rules);
			cout << "end of tree definition" << endl;
		}
	}

	cout << "name: " << name << endl;
	cout << "startLength: " << startLength[0] << " " << startLength[1] << endl;
	cout << "branchAngle: " << branchAngle << endl;
	cout << "probability: " << probability << endl;
	cout << "rules: " << rules.size() << endl;
}