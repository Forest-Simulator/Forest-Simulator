#include <queue>
#include <cstdint>

#include "cgra_math.hpp"
#include "boid.hpp"

using namespace std;
using namespace cgra;

struct boundingBox{
	cgra::vec3 pos;		//measured from bottom left closest corner
	cgra::vec3 size;	//goes to top right furthest corner
};

class OctTree{
private:
	queue<Boid*> pending_insertion;

	boundingBox bounding_box;

	vector<Boid*> objects;

	int MIN_SIZE = 2;
	int MAX_LIFESPAN = 8;
	int currentLife = -1;

	OctTree *parent = nullptr;
	OctTree *children [8];

	uint8_t activeNodes = 0;

	bool treeReady = false;
	bool treeBuilt = false;

	bool hasChildren = false;

	void buildTree(int level);
	void insert(Boid *boid);
	void addOctant(cgra::vec3 pos, cgra::vec3 size, boundingBox *octant);
	void update();
	bool contains(Boid *boid, boundingBox box);
	OctTree* createNode(boundingBox region, vector<Boid*> obs);
public:
	OctTree();
	OctTree(boundingBox box);
	OctTree(boundingBox box, vector<Boid*> obj);
	OctTree(cgra::vec3 pos, cgra::vec3 size, vector<Boid*> obj);
	void updateTree();
	void renderTree(int level);
};
