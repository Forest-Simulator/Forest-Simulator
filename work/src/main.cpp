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
#include <cstdlib>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "cgra_math.hpp"
#include "simple_shader.hpp"
#include "opengl.hpp"
#include "heightmap.hpp"
#include "lsystem.hpp"
#include "tree.hpp"
#include "treefactory.hpp"

#include "flock.hpp"

using namespace std;
using namespace cgra;

// Window
//
GLFWwindow* g_window;


// Projection values
// 
float g_fovy = 20.0;
float g_znear = 0.1;
float g_zfar = 1000.0;


// Mouse controlled Camera values
//
bool g_leftMouseDown = false;
vec2 g_mousePosition;
float g_pitch = 0;
float g_yaw = 0;
float g_zoom = 1.0;

// Shader code
//
bool g_useShader = false;
GLuint g_shader = 0;

// Command line argument defaults
//
int mapSize = 4;
string treeFile = "../work/res/trees/trees.txt";

// Base Heightmap to be rendered upon
//
hmap::Heightmap* heightmap;
tree::TreeFactory* treeFactory;
vector<tree::Tree*> trees;

//Flock of birds
//
Flock* flock;
Boid* boid;


// Mouse Button callback
// Called for mouse movement event on since the last glfwPollEvents
//
void cursorPosCallback(GLFWwindow* win, double xpos, double ypos) {
	// cout << "Mouse Movement Callback :: xpos=" << xpos << "ypos=" << ypos << endl;
	if (g_leftMouseDown) {
		g_yaw -= g_mousePosition.x - xpos;
		g_pitch -= g_mousePosition.y - ypos;
	}
	g_mousePosition = vec2(xpos, ypos);
}


// Mouse Button callback
// Called for mouse button event on since the last glfwPollEvents
//
void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
	// cout << "Mouse Button Callback :: button=" << button << "action=" << action << "mods=" << mods << endl;
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		g_leftMouseDown = (action == GLFW_PRESS);
}


// Scroll callback
// Called for scroll event on since the last glfwPollEvents
//
void scrollCallback(GLFWwindow *win, double xoffset, double yoffset) {
	// cout << "Scroll Callback :: xoffset=" << xoffset << "yoffset=" << yoffset << endl;
	g_zoom -= yoffset * g_zoom * 0.2;
}


// Keyboard callback
// Called for every key event on since the last glfwPollEvents
//
void keyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) {
	// cout << "Key Callback :: key=" << key << "scancode=" << scancode
	// 	<< "action=" << action << "mods=" << mods << endl;
}


// Character callback
// Called for every character input event on since the last glfwPollEvents
//
void charCallback(GLFWwindow *win, unsigned int c) {
	// cout << "Char Callback :: c=" << char(c) << endl;
	// Not needed for this assignment, but useful to have later on
}


// Sets up where the camera is in the scene
// 
void setupCamera(int width, int height) {
	// Set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(g_fovy, width / float(height), g_znear, g_zfar);

	// Set up the view part of the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -50 * g_zoom);
	glRotatef(g_pitch, 1, 0, 0);
	glRotatef(g_yaw, 0, 1, 0);
}

void initAmbientLight() {
	float noLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	
	// Remove diffuse and specular components for the 
	// ambient light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ambient);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glEnable(GL_LIGHT0);
}

void initLight() {
	initAmbientLight();
}

void initShader() {
	// To create a shader program we use a helper function
	// We pass it an array of the types of shaders we want to compile
	// and the corrosponding locations for the files of each stage
	g_shader = makeShaderProgramFromFile(
		{
			GL_VERTEX_SHADER,
			GL_FRAGMENT_SHADER
		},
		{
			"../work/res/shaders/vertex_shader.vert",
			"../work/res/shaders/fragment_shader.frag"
		}
	);
}

void initHeightmap() {
	heightmap = new hmap::Heightmap(mapSize);
	heightmap->generateHeightmap();
}

void initTrees() {
	treeFactory = new tree::TreeFactory(treeFile);

	int incr = 8;
	float halfIncr = incr / 2;
	int halfSize = (heightmap->getSize() - incr) / 2;
	int y = halfSize;
	
	for(int x = -halfSize; x < halfSize; x += incr) {
		// Randomly offset the trees
		float offsetX = math::random(-halfIncr, halfIncr);
		float offsetY = math::random(-halfIncr, halfIncr);
		trees.push_back(treeFactory->generate(vec3(x+offsetX, -y+offsetY, 0)));
		if(x+incr == halfSize && y > -halfSize) {
			y -= incr;
			x = -halfSize;
		}
	}

	// Trees have been generated, so release the treeFactory object
	// from memory and set its pointer to null
	delete treeFactory;
	treeFactory = nullptr;
}

void initFlock(){
	flock = new Flock(200);
	boid = new Boid(vec3(1, 1, 1));
}

void groundMaterial() {
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void boidMaterial() {
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat mat_shininess[] = { 0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}


void renderObjects(int width, int height) {
	// Render a single square as our geometry
	// You would normally render your geometry here
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 

		glPushMatrix();
			groundMaterial();
			heightmap->render();
		glPopMatrix();

		glPushMatrix();
			for(tree::Tree* t : trees) {
				t->render();
			}
		glPopMatrix();

		boidMaterial();
		flock->update();
		//boid->render();


	glPopMatrix();

	glFlush();
}

// Draw function
//
void render(int width, int height) {

	// Light blue background
	glClearColor(0.49f, 0.65f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Enable flags for normal rendering
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	setupCamera(width, height);

	// Texture setup
	//

	// Enable Drawing texures
	glEnable(GL_TEXTURE_2D);

	// Bind shader
	glUseProgram(g_shader);

	glUniform1i(glGetUniformLocation(g_shader, "texture0"), 0);
	
	renderObjects(width, height);

	// Unbind shader
	glUseProgram(0);

	// Disable flags for cleanup (optional)
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
}


// Forward decleration for cleanliness (Ignore)
void APIENTRY debugCallbackARB(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, GLvoid*);


//Main program
// 
int main(int argc, char **argv) {

	// Initialize the GLFW library
	if (!glfwInit()) {
		cerr << "Error: Could not initialize GLFW" << endl;
		abort(); // Unrecoverable error
	}


	if(argc >= 2) {
		int tempSize = *argv[1] - '0';
		if(0 > tempSize || tempSize > 5) {
			cerr << "Error: Map Size " << tempSize << " is out of bounds (0 - 5)" << endl;
			abort();
		}
		mapSize = tempSize;
	}

	if(argc == 3) {
		treeFile = argv[2];
	}

	// Get the version for GLFW for later
	int glfwMajor, glfwMinor, glfwRevision;
	glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);

	// Create a windowed mode window and its OpenGL context
	g_window = glfwCreateWindow(640, 480, "Forest Simulator", nullptr, nullptr);
	if (!g_window) {
		cerr << "Error: Could not create GLFW window" << endl;
		abort(); // Unrecoverable error
	}

	// Make the g_window's context is current.
	// If we have multiple windows we will need to switch contexts
	glfwMakeContextCurrent(g_window);

	// Initialize GLEW
	// must be done after making a GL context current (glfwMakeContextCurrent in this case)
	glewExperimental = GL_TRUE; // required for full GLEW functionality for OpenGL 3.0+
	GLenum err = glewInit();
	if (GLEW_OK != err) { // Problem: glewInit failed, something is seriously wrong.
		cerr << "Error: " << glewGetErrorString(err) << endl;
		abort(); // Unrecoverable error
	}



	// Print out our OpenGL verisions
	cout << "Using OpenGL " << glGetString(GL_VERSION) << endl;
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
	cout << "Using GLFW " << glfwMajor << "." << glfwMinor << "." << glfwRevision << endl;



	// Attach input callbacks to g_window
	glfwSetCursorPosCallback(g_window, cursorPosCallback);
	glfwSetMouseButtonCallback(g_window, mouseButtonCallback);
	glfwSetScrollCallback(g_window, scrollCallback);
	glfwSetKeyCallback(g_window, keyCallback);
	glfwSetCharCallback(g_window, charCallback);



	// Enable GL_ARB_debug_output if available. Not nessesary, just helpful
	if (glfwExtensionSupported("GL_ARB_debug_output")) {
		// This allows the error location to be determined from a stacktrace
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		// Set the up callback
		glDebugMessageCallbackARB(debugCallbackARB, nullptr);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
		cout << "GL_ARB_debug_output callback installed" << endl;
	}
	else {
		cout << "GL_ARB_debug_output not available. No worries." << endl;
	}


	// Initialize Geometry/Material/Lights
	initFlock();
	initHeightmap();
	initShader();
	initTrees();
	initLight();

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(g_window)) {

		// Make sure we draw to the WHOLE window
		int width, height;
		glfwGetFramebufferSize(g_window, &width, &height);

		// Main Render
		render(width, height);

		// Swap front and back buffers
		glfwSwapBuffers(g_window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
}






//-------------------------------------------------------------
// Fancy debug stuff
//-------------------------------------------------------------

// function to translate source to string
string getStringForSource(GLenum source) {

	switch (source) {
	case GL_DEBUG_SOURCE_API:
		return("API");
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return("Window System");
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return("Shader Compiler");
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return("Third Party");
	case GL_DEBUG_SOURCE_APPLICATION:
		return("Application");
	case GL_DEBUG_SOURCE_OTHER:
		return("Other");
	default:
		return("n/a");
	}
}

// function to translate severity to string
string getStringForSeverity(GLenum severity) {

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		return("HIGH!");
	case GL_DEBUG_SEVERITY_MEDIUM:
		return("Medium");
	case GL_DEBUG_SEVERITY_LOW:
		return("Low");
	default:
		return("n/a");
	}
}

// function to translate type to string
string getStringForType(GLenum type) {
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		return("Error");
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return("Deprecated Behaviour");
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return("Undefined Behaviour");
	case GL_DEBUG_TYPE_PORTABILITY:
		return("Portability Issue");
	case GL_DEBUG_TYPE_PERFORMANCE:
		return("Performance Issue");
	case GL_DEBUG_TYPE_OTHER:
		return("Other");
	default:
		return("n/a");
	}
}

// actually define the function
void APIENTRY debugCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message, GLvoid*) {
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) return;

	cerr << endl; // extra space

	cerr << "Type: " <<
		getStringForType(type) << "; Source: " <<
		getStringForSource(source) << "; ID: " << id << "; Severity: " <<
		getStringForSeverity(severity) << endl;

	cerr << message << endl;

	if (type == GL_DEBUG_TYPE_ERROR_ARB) throw runtime_error("");
}