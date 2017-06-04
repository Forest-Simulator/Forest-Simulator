#pragma once

#include <iostream>
#include <vector>

#include "cgra_math.hpp"

namespace hmap {

	struct Point {
		int x;
		int y;
		Point() {};
		Point(int x_val, int y_val) {
			x = x_val;
			y = y_val;
		};
		void print() {
			std::cout << "Point(" << x << ", " << y << ")" << std::endl;
		}
	};


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

	class Heightmap {
	private:
		int size;
		int seed;
		
		// Random upper and lower bounds
		float lower = -1.0;
		float upper = 1.0;

		// Initial needs to be at least double the lower 
		// or upper bounds to work correctly
		float initial = (lower * 3);

		// Random decay rate
		float randomDecayRate = 0.1;

		std::vector<std::vector<float>> heightmap;

		std::vector<cgra::vec3> vertices;
		std::vector<cgra::vec3> normals;
		std::vector<Triangle> triangles;

		void constructHelper();
		void generateCorners(int, int);
		void calculateDiamondCenters(Point, int);
		void calculateSquareCenter(Point, int);
		void calculateDiamondCenter(Point, int);
		void makeList();

		float getAverageOfSquare(Point, int);
		float getAverageOfDiamond(Point, int);
		float averageOfList(std::vector<Point>);
		Point getSquareCenter(Point, int);
		bool isValid(Point);
		float randomValue();
		
	public:
		Heightmap();
		Heightmap(int);
		Heightmap(int, float);
		~Heightmap();

		void render();
		void generateHeightmap();
		void printHeightmap();

		float getAt(Point);
		void setAt(Point, float);
		void printAt(Point);

		std::vector<cgra::vec3> getVertices();
	};
}