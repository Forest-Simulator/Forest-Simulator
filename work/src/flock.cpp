#include <cmath>
#include <cstdlib>

#include "cgra_math.hpp"
#include "flock.hpp"
//#include "boid.hpp"

using namespace std;
using namespace cgra;

Flock::Flock(int size){
	for (int i = 0; i < size - 1; ++i){
		Boid b = Boid(vec3(rand() % 10, rand() % 10, rand() % 10));
		boids.push_back(b);
		arrange(leader, b);
	}
} 

void Flock::update(){
	steer(leader);
	leader.destination = destination;
	int s = boids.size();
	for(int i = 0; i < s; ++i){
		steer(boids[i]);
		boids[i].destination = boids[i].parent->position;
	}
}

void Flock::steer(Boid b){
	vec3 v = vec3(0, 0, 0);
	v += align(b);
	v += separate(b);

	b.velocity += v;
	if(length(v) > max_speed){
		b.velocity = b.velocity*(max_speed/length(v));
	}

	b.position += b.velocity;
}

vec3 Flock::align(Boid b){
	vec3 alignment = b.destination - b.position;
	return normalize(alignment);
}

vec3 Flock::separate(Boid b){
	int neighbours;
	vec3 velocity = vec3(0, 0, 0);

	int s = boids.size();
	for(int i = 0; i < s; ++i){
		if(&boids[i] != &b){
			float distance = length(b.position - boids[i].position);

			if(distance < minimum_separation){
				vec3 force = b.position - boids[i].position;
				velocity += normalize(force);

				++neighbours;
			}
		}
	}

	if(neighbours > 0){
		return velocity/neighbours;
	}
	return velocity;
}

void Flock::setDestination(vec3 dest){
	destination = dest;
}

void Flock::arrange(Boid node, Boid b){
	if(node.emptyLeft){
		node.left = &b;
		node.emptyLeft = false;
		b.parent = &node;
		return;
	}
	else if(node.emptyRight){
		node.right = &b;
		node.emptyRight = false;
		b.parent = &node;
		return;
	}
	else{
		if(rand()%2){
			arrange(*node.left, b);
		}
		else{
			arrange(*node.right, b);
		}
	}
}
