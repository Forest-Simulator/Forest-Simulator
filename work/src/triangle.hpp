#pragma once

#include <vector>

struct Triangle {
	std::vector<int> vertices;
	std::vector<int> normals;
	Triangle() {};
	Triangle(int v1, int v2, int v3) {
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
	};
};