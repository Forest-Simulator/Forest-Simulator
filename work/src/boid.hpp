#include "cgra_math.hpp"
#include "opengl.hpp"

using namespace std;
using namespace cgra;


struct triangle{
	vec3 vertices[3];

	cgra::vec3 normal;
};

class Boid{
private:
	vector<triangle> triangles;

	GLuint m_displayList = 0;

	void addTriangles();
	void createDisplayList();
	void process(triangle tri);

public:
	cgra::vec3 position;
	cgra::vec3 velocity;
	cgra::vec3 destination;

	Boid* left = nullptr;
	Boid* right = nullptr;
	Boid* parent = nullptr;

	float minimum_separation = 0.1f;

	Boid(cgra::vec3 position);
	void render();
};
