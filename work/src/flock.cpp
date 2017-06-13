#include <cmath>
#include <cstdlib>

#include "cgra_math.hpp"
#include "flock.hpp"

using namespace std;
using namespace cgra;

Flock::Flock(int size){
	for (int i = 0; i < size - 1; ++i){
		Boid b = Boid(vec3(rand() % 10, rand() % 10, rand() % 10));
		boids.push_back(b);
		arrange(&leader, &b);
	}
} 

void Flock::update(){
	leader.destination = destination;
	steer(&leader);
	leader.render();

	int s = boids.size();
	for(int i = 0; i < s; ++i){
		boids[i].destination = boids[i].parent->position;
		steer(&boids[i]);
		boids[i].render();
	}
}

void Flock::steer(Boid *b){
	vec3 v = vec3(0, 0, 0);
	v += align(b);
	v += separate(b);

	b->velocity += v;

	float length = lengthVector(b->velocity);
	if(length > max_speed){
		b->velocity = b->velocity*(max_speed/length);
	}

	b->position += b->velocity;
}

float Flock::lengthVector(vec3 v){
	vec3 u = v;
	float n = (u.x * u.x + u.y * u.y + u.z * u.z);
	if(n != 0){
		float l = sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
		return l;
	}
	return 0;
}

vec3 Flock::normalizeVector(vec3 v){
	vec3 u = v;
	float l = lengthVector(u);
	if(l != 0){
		if(u.x != 0){ u.x /= l; }
		if(u.y != 0){ u.y /= l; }
		if(u.z != 0){ u.z /= l; }
		return u;
	}
	return vec3(0, 0, 0);
}

vec3 Flock::align(Boid *b){
	vec3 a = b->destination - b->position;
	return normalizeVector(a);
}

vec3 Flock::separate(Boid *b){
	int neighbours = 0;
	vec3 velocity = vec3(0, 0, 0);

	int s = boids.size();
	for(int i = 0; i < s-1; ++i){
		if(&boids[i] != b){
			float distance = lengthVector(b->position - boids[i].position);
			if(distance < minimum_separation){
				vec3 force = b->position - boids[i].position;
				velocity += normalizeVector(force);

				++neighbours;
			}
		}
	}

	

	if(neighbours > 0){
		if(velocity.x != 0){velocity.x /= neighbours;}
		if(velocity.y != 0){velocity.y /= neighbours;}
		if(velocity.z != 0){velocity.z /= neighbours;}
		return velocity;
	}
	return velocity;
}

void Flock::setDestination(vec3 dest){
	destination = dest;
}

void Flock::arrange(Boid *node, Boid *b){
	if(node->left != nullptr){
		node->left = b;
		b->parent = node;
		return;
	}
	else if(node->right != nullptr){
		node->right = b;
		b->parent = node;
		return;
	}
	else{
		if(rand()%2){
			arrange(node->left, b);
		}
		else{
			arrange(node->right, b);
		}
	}
}
