#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "cgra_math.hpp"
#include "lsystem.hpp"
#include "tree.hpp"
#include "treefactory.hpp"

using namespace cgra;
using namespace lsys;
using namespace std;
using namespace tree;

TreeGenerator::TreeGenerator() {}

Tree* TreeGenerator::generate() {
	vector<string> s;
	for(int i = 0; i < generations; i++) {
		s = lsystem.generate();
	}
	
	float sl = math::random(startLength[0], startLength[1]);

	return new Tree(s, branchAngle, sl);
}

TreeFactory::TreeFactory(string filename) {
	readFile(filename);
}

void TreeFactory::readFile(string filename) {
	ifstream objFile(filename);

	if(!objFile.is_open()) {
		cerr << "Error reading " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	TreeGenerator t;

	char match;
	string transform;
	char rightcontext = 'Z';
	char leftcontext = 'Z';
	float rulechance = 0.0;

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
			t = TreeGenerator();
		} else if(key == "name") {
			t.name = values[0];
		} else if(key == "axiom") {
			t.axiom = values[0];
		} else if(key == "startLength") {
			float lower = stof(values[0]);
			float upper = stof(values[1]);
			t.startLength.push_back(lower);
			t.startLength.push_back(upper);
		} else if(key == "branchAngle") {
			t.branchAngle = stof(values[0]);
		} else if(key == "probability") {
			t.probability = stof(values[0]);
		} else if(key == "generations") {
			t.generations = stof(values[0]);
		} else if(key == "rulestart") {
			// rules.clear();
		} else if(key == "match") {
			match = values[0][0];
		} else if(key == "transform") {
			transform = values[0];
		} else if(key == "rulechance") {
			rulechance = stof(values[0]);
		} else if(key == "rightcontext") {
			rightcontext = values[0][0];
		} else if(key == "leftcontext") {
			leftcontext = values[0][0];
		} else if(key == "ruleend") {
			Rule r;
			if(rulechance != 0.0) {
				r = Rule(match, transform, rulechance);
			} else if(rightcontext != 'Z' || leftcontext != 'Z') {
				RuleContext rc;
				rc.right = rightcontext;
				rc.left = leftcontext;
				r = Rule(match, transform, rc);
			} else {
				r = Rule(match, transform);
			}
			t.rules.push_back(r);
		} else if(key == "end") {
			t.lsystem = LSystem(t.axiom, t.rules);
			generators.push_back(t);
		}
	}
}

Tree* TreeFactory::generate() {
	
	int rand = math::random(0, int(generators.size()));
	TreeGenerator tg = generators[rand];
	
	return tg.generate();
}
