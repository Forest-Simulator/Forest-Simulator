```
---------------------------------------------------------------------
    ____                 __    _____            __     __          
   / __/__  _______ ___ / /_  / __(_)_ _  __ __/ /__ _/ /____  ____
  / _// _ \/ __/ -_|_-</ __/ _\ \/ /  ' \/ // / / _ `/ __/ _ \/ __/
 /_/  \___/_/  \__/___/\__/ /___/_/_/_/_/\_,_/_/\_,_/\__/\___/_/   
                                                                  
---------------------------------------------------------------------
```

A COMP308 project by Lawrence Buck and Sam Costigan.

## Building the Project

To build the project, run the following commands from the main project directory:
```
mkdir build
cd build
cmake ../work
make
```

## Running the Project

Once built, you can then run the project from the `build` directory with:
```
./bin/fs
```

You can specify the size of the terrain and the type of trees to use as command line arguments:
```
./bin/fs <size> <trees>
```

The default used when called without arguments is:

```
./bin/fs 4 ../work/res/trees/trees.txt
```

The size must be a number between 0-6. The trees must be a path to a file relative to the `build` 
directory. This file must follow a specific format.

## Trees File

The trees are generated using [Lindenmayer Systems](https://en.wikipedia.org/wiki/L-system). This is a 
parallel string rewriting system used to model biological processes. A Lindenmayer System (L-system) is 
composed of an axiom, which is the starting string, and a series of rules which describe how characters 
in the system should be rewritten. The trees file defines L-systems in the following format:
```
start
name		Example
axiom		A
startLength	1.0 2.0
branchAngle	25.0
generations	2
rulestart
	match		A
	transform	AB
ruleend
rulestart
	match		B
	transform	BC
ruleend
end
```
The output of such an L-system would be:

* No generations: A
* 1st generation: AB
* 2nd generation: ABBC

The resulting string is then used to draw a tree in the following way. Specific characters are mapped to 
functions which carry out certain actions in 3D space. The function mappings are as follows:
```
F: draw a branch and place a vertex
G: draw a branch
f: move forward without drawing anything and place a vertex
g: move forward without drawing anything
[: push the current state, creating a branch
]: pop the current state, returning to position and rotation before the branch
{: begin a polygon which includes all vertices placed within
}: end the polygon at the last placed vertex
+: turn left by branchAngle
-: turn right by branchAngle
^: pitch up by branchAngle
&: pitch down by branchAngle
\: roll left by branchAngle
/: roll right by branchAngle
|: turn around 180 degrees along the y axis
’: increase the current colour index
;: decrease the current colour index
#: increase the current branch length
!: decrease the current branch length
.: place a vertex within a polygon
```
Many different kinds of plants can be constructed using such an L-system. Multiple rules can also be defined 
in the same file. The generator will choose randomly from the list of trees.

## Flocking and Oct-tree

Flocking algorithm spawns 200 boids and sets destination at 15, 15, 15. The destination changes
randomly whenever the leader reaches it.

rulestart
    Press s to toggle visual representation of the oct tree.
    Press d to toggle pausing the simulation.
    Press a while paused to step the simulation.
    Press o to toggle the usage of the octree vs simple O(n^2) collision detection.
ruleend

In addition to the arguments above, you can add two extra arguments.

Argument 3 is an int, and sets the number of boids. this is 200 by default
Argument 4 is a bool, (type true or false) and sets whether the oct tree is in intial use at runtime or not.