#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>
#include <random>

#include "cgra_math.hpp"
#include "heightmap.hpp"

using namespace std;
using namespace cgra;

Heightmap::Heightmap() {
	// Default the size to 5 if one is not provided
	size = 5;
	constructHelper();
}

Heightmap::Heightmap(float mapSize) {
	size = mapSize;
	constructHelper();
}

Heightmap::Heightmap(float mapSize, float mapSeed) {
	size = mapSize;
	seed = mapSeed;
	constructHelper();
}

Heightmap::~Heightmap() {  }

void Heightmap::constructHelper() {
	vector<float> vals (size, initial);
	heightmap.assign(size, vals);
}

void Heightmap::generateHeightmap() {
	int start = 0;
	int end = size - 1;

	generateCorners(start, end);

	int distance = (end - start);
	Point squareStart = Point(start, start);

	while(distance > 0) {
		cout << distance << endl;
		// Diamond step
		for(int x = 0; x < end; x += distance) {
			for(int y = 0; y < end; y += distance) {
				squareStart = Point(x, y);
				// if(distance == 4) {
					calculateSquareCenter(squareStart, distance);
				// }
			}
		}

		// Square step
		for(int x = 0; x < end; x += distance) {
			for(int y = 0; y < end; y += distance) {
				squareStart = Point(x, y);
				// if(distance == 4) {
					diamondStep(squareStart, distance);
				// }
				

			}
		}

		distance = distance / 2;
	}
}

void Heightmap::generateCorners(int start, int end) {
	setAt(Point(start, start), randomValue());
    setAt(Point(start, end), randomValue());
    setAt(Point(end, start), randomValue());
    setAt(Point(end, end), randomValue());
}

void Heightmap::calculateSquareCenter(Point start, int distance) {
	Point centerOfSquare = getSquareCenter(start, distance);
	float averageOfSquare = getAverageOfSquare(start, distance);
	
	setAt(centerOfSquare, averageOfSquare);
}

void Heightmap::calculateDiamondCenter(Point diamondCenter, int distance) {
	float averageOfDiamond = getAverageOfDiamond(diamondCenter, distance);
	setAt(diamondCenter, averageOfDiamond);
}

void Heightmap::diamondStep(Point square, int distance) {
	int halfDistance = (distance / 2);
	Point centerOfSquare = getSquareCenter(square, distance);

	vector<Point> diamondCenters = {
		Point(centerOfSquare.x-halfDistance, centerOfSquare.y),
		Point(centerOfSquare.x+halfDistance, centerOfSquare.y),
		Point(centerOfSquare.x, centerOfSquare.y-halfDistance),
		Point(centerOfSquare.x, centerOfSquare.y+halfDistance),
	};

	if(distance > 0) {
		for(Point p : diamondCenters) {
			calculateDiamondCenter(p, halfDistance);
		}
	}
}

float Heightmap::getAverageOfSquare(Point start, int distance) {
	vector<Point> values = {
		Point(start.x, start.y),
		Point(start.x, start.y+distance),
		Point(start.x+distance, start.y),
		Point(start.x+distance, start.y+distance),
	};

	return averageOfList(values);
}

float Heightmap::getAverageOfDiamond(Point start, int distance) {
	vector<Point> values = {
		Point(start.x+distance, start.y),
		Point(start.x-distance, start.y),
		Point(start.x, start.y+distance),
		Point(start.x, start.y-distance),
	};
	
	return averageOfList(values);
}

bool Heightmap::isValid(Point point) {
    return (point.x >= 0 && point.y >= 0) && (point.x < size && point.y < size);
}

float Heightmap::averageOfList(vector<Point> list) {
	float count = 0.0;
	float total = 0.0;
	for(Point p : list) {
		if(isValid(p)) {
			total += getAt(p);
			count += 1.0;
		}
	}

	if(count > 0) {
		return total / count;
	}
	return initial;
}

Point Heightmap::getSquareCenter(Point squareStart, int distance) {
	int halfDistance = (distance/2);
	Point centerOfSquare = Point(
		squareStart.x + halfDistance,
		squareStart.y + halfDistance
	);
	return centerOfSquare;
}

void Heightmap::printHeightmap() {
	for(vector<float> v : heightmap) {
		for(float f : v) {
			cout << f << ", ";
		}
		cout << endl;
	}
}

float Heightmap::getAt(Point point) {
	return heightmap[point.y][point.x];
}

void Heightmap::setAt(Point point, float z) {
	if(heightmap[point.y][point.x] == initial) {
		heightmap[point.y][point.x] = z;
	}
}

void Heightmap::printAt(Point point) {
	cout << heightmap[point.y][point.x] << endl;
}

float Heightmap::randomValue() {
	return math::random<float>(lower, upper);
}