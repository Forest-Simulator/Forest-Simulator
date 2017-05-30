//---------------------------------------------------------------------------
//    ____                 __    _____            __     __          
//   / __/__  _______ ___ / /_  / __(_)_ _  __ __/ /__ _/ /____  ____
//  / _// _ \/ __/ -_|_-</ __/ _\ \/ /  ' \/ // / / _ `/ __/ _ \/ __/
// /_/  \___/_/  \__/___/\__/ /___/_/_/_/_/\_,_/_/\_,_/\__/\___/_/   
//                                                                  
// A COMP308 project by Lawrence Buck and Sam Costigan.
//
//----------------------------------------------------------------------------

#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "heightmap.hpp"

using namespace std;

int main(int argc, char **argv) {
	Heightmap hm = Heightmap(9);
	Point p;
	p.x = 0;
	p.y = 4;
	// hm.setAt(p, 5);
	
	// hm.printAt(p);
	hm.generateHeightmap();
	hm.printHeightmap();
}