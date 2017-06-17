#
# CGRA Project
# If you add new source files, you must list them here
#

# TODO list your header files (.hpp) here
SET(headers
	"cgra_math.hpp"	
	"cgra_geometry.hpp"
	"opengl.hpp"
	"triangle.hpp"
	"heightmap.hpp"
	"lsystem.hpp"
	"tree.hpp"
	"treefactory.hpp"
	"boid.hpp"
	"flock.hpp"
	"oct_tree.hpp"
)


# TODO list your source files (.cpp) here
SET(sources
	"main.cpp"
	"heightmap.cpp"
	"lsystem.cpp"
	"tree.cpp"
	"treefactory.cpp"
	"boid.cpp"
	"flock.cpp"
	"oct_tree.cpp"
)

# Add executable target and link libraries
# You do not need to touch this
add_executable(${CGRA_PROJECT} ${headers} ${sources})
target_link_libraries(${CGRA_PROJECT} PRIVATE glew glfw ${GLFW_LIBRARIES})
target_link_libraries(${CGRA_PROJECT} PRIVATE stb)