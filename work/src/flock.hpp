#include "cgra_math.hpp"
#include "opengl.hpp"
#include "boid.hpp"

using namespace std;
using namespace cgra;

class Flock{
private:
	Boid leader = Boid(vec3(0, 0, 0));
	vector<Boid> boids;
	vec3 destination;

	float minimum_separation = 2;
	float max_speed = 0.2f;

	void steer(Boid b);
	cgra::vec3 separate(Boid b);
	cgra::vec3 align(Boid b);
	
	void arrange(Boid node, Boid b);
public:
	Flock(int size);
	void setDestination(vec3 dest);
	void update();
};
