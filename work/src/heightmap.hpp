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
	
	// Random upper and lower bounds
	float lower = -5.0;
	float upper = 5.0;

	// Initial needs to be at least double the lower 
	// or upper bounds to work correctly
	float initial = (lower * 3);

	// Random decay rate
	float randomDecayRate = 1.0;

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

	void render();
	void generateHeightmap();
	void printHeightmap();

	float getAt(Point);
	void setAt(Point, float);
	void printAt(Point);
};