#pragma once

#include <iostream>
#include <vector>

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

class Heightmap {
private:
	int size;
	int seed;
	float initial = -100.0f;
	float lower = 0.0;
	float upper = 10.0;
	std::vector<std::vector<float>> heightmap;

	void constructHelper();
	void generateCorners(int, int);
	void diamondStep(Point, int);
	void calculateSquareCenter(Point, int);
	void calculateDiamondCenter(Point, int);

	float getAverageOfSquare(Point, int);
	float getAverageOfDiamond(Point, int);
	float averageOfList(std::vector<Point>);
	Point getSquareCenter(Point, int);
	bool isValid(Point);
	float randomValue();
	
public:
	Heightmap();
	Heightmap(float);
	Heightmap(float, float);
	~Heightmap();


	float getAt(Point);
	void setAt(Point, float);
	void printAt(Point);
	void generateHeightmap();
	void printHeightmap();
	
};