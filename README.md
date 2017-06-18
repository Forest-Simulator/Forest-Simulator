```
//---------------------------------------------------------------------------
//    ____                 __    _____            __     __          
//   / __/__  _______ ___ / /_  / __(_)_ _  __ __/ /__ _/ /____  ____
//  / _// _ \/ __/ -_|_-</ __/ _\ \/ /  ' \/ // / / _ `/ __/ _ \/ __/
// /_/  \___/_/  \__/___/\__/ /___/_/_/_/_/\_,_/_/\_,_/\__/\___/_/   
//                                                                  
//----------------------------------------------------------------------------
```

A COMP308 project by Lawrence Buck and Sam Costigan.

# Building the Project

To build the project, run the following commands from the main project directory:
```
mkdir build
cd build
cmake ../work
make
```

# Running the Project

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

The size must be a number between 0-5. The trees must be a path to a file relative to the `build` 
directory. This file must follow a specific format.

# Trees File

The trees are generated using [Lindenmayer Systems](https://en.wikipedia.org/wiki/L-system). This is a
string rewriting system used to model biological processes. The basics
 The trees file uses the following format:

```
start
name		Example
axiom		F
startLength	1.0 2.0
branchAngle	25.0
probability	0.2
generations	2
rulestart
	match		F
	transform	FF-[/+F-F-FS]+[\\-F+F+FS]
ruleend
end
```