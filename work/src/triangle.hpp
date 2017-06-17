#pragma once

#include <vector>

#include "cgra_math.hpp"

struct Vertex {
	int point;
	int normal;
	Vertex() {};
	Vertex(int p) {
		point = p;
		normal = p;
	};
};

struct Triangle {
	std::vector<Vertex> vertices2;
	std::vector<int> vertices;
	std::vector<int> normals;
	Triangle() {};
	Triangle(int v1, int v2, int v3) {
		vertices2.push_back(Vertex(v1));
		vertices2.push_back(Vertex(v2));
		vertices2.push_back(Vertex(v3));


		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
	};
};