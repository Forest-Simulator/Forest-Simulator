#include "cgra_math.hpp"
#include "opengl.hpp"
#include "boid.hpp"

using namespace std;
using namespace cgra;

class Flock{
private:
	Boid leader = Boid(vec3(0, 0, 0));
	vec3 destination = vec3(5, 5, 5);
	vector<Boid> boids;

	float minimum_separation = 1;
	float max_speed = 0.2f;

	void steer(Boid *b);
	float lengthVector(cgra::vec3 v);
	cgra::vec3 normalizeVector(cgra::vec3 v);
	cgra::vec3 separate(Boid *b);
	cgra::vec3 align(Boid *b);
	
	void arrange(Boid *node, Boid *b);
public:
	Flock(int size);
	void setDestination(vec3 dest);
	void update();
};
