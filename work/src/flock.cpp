#pragma once

#include "cgra_math.hpp"
#include "opengl.hpp"
#include "flock.hpp"

using namespace std;
using namespace cgra;

Flock::Flock(int size){
	leader = boid(vec3(0, 0, 0));
	for (int i = 0; i < size - 1; ++i){
		boid b = boid(vec3(rand() % 10, rand() % 10, rand() % 10))
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
		boids[i].destination = boids[i].parent.position;
	}
}

void Flock::steer(Boid b){
	vec3 v = vec3(0, 0, 0);
	v += align(b);
	v += separate(b);

	if(length(v) > max_speed){
		v = v*(max_speed/length(v));
	}


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
		if(boids[i] != b){
			float distance = distance(b.position, boids[i].position);

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

void arrange(Boid node, Boid b){
	if(node.emptyLeft){
		node.left = b;
		node.emptyLeft = b;
		b.parent = node;
		return;
	}
	else if(node.emptyRight){
		node.right = b;
		node.emptyRight = false;
		b.parent = node;
		return;
	}
	else{
		if(rand()%2){
			arrange(node.left, b);
		}
		else{
			arrange(node.right, b);
		}
	}
}
