#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>



#include "ShaderProgram.h"
//#include "ObjMesh.h"
//#include "error.h"


#include "Model.h"
#include "Car.h"
float scaleFactor = 1.0f;


//#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include "error.h"

#define SCALE_FACTOR 2.0f
#define SHININESS_STEP 1.0f

int width, height;


float lightOffsetY = 0.0f;
//glm::vec3 eyePosition(4, 3, 5);
//glm::vec3 eyePosition(40, 30, 30);


glm::vec3 eyePosition(0, 10, 10);
bool animateLight = true;
bool rotateObject = true;
//float scaleFactor = 1.0f;


float lastX = std::numeric_limits<float>::infinity();
float lastY = std::numeric_limits<float>::infinity();
float shininess = 25.0f;


//Model* Car;

// Global Variables and data containers
//std::vector<Model*> Models;
std::vector<Model*> Models;
glm::mat4 projection;


int lastTime = 0;
int nowTime = 0;
int dt = 0;
static void update(void) {
	//int milliseconds = 50;
    nowTime = glutGet(GLUT_ELAPSED_TIME);

    // rotate the shape about the y-axis so that we can see the shading
    //if (rotateObject) {
    //  float degrees = (float)milliseconds / 80.0f;
    //  angle = degrees;
    //}

    // move the light position over time along the x-axis, so we can see how it affects the shading
    if (animateLight) {
      float t = nowTime / 2500.0f;
      lightOffsetY = sinf(t) * 40.0f;
    }

	for (auto const& model : Models)
	{
		model->Update( nowTime , nowTime-lastTime);
	}


    glutPostRedisplay();

	lastTime = nowTime;
}

static void render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// turn on depth buffering
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_MULTISAMPLE);

	// projection matrix - perspective projection
	// FOV:           45°
	// Aspect ratio:  4:3 ratio
	// Z range:       between 0.1 and 100.0
	float aspectRatio = (float)width / (float)height;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.001f, 1000.0f);

	// projection matrix - orthographic (non-perspective) projection
	// Note:  These are in world coordinates
	// xMin:          -10
	// xMax:          +10
	// yMin:          -10
	// yMax:          +10
	// zMin:           0
	// zMax:          +100
	//glm::mat4 projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);


	//glm::vec4 test = glm::vec4(0, 5, 5, 1);

	//glm::vec3 lookpos = car->modelm*glm::vec4(0,0,-5,1);
	//eyePosition = car->modelm*test;

	Model* car = Models[0];
	glm::vec3 lookpos = glm::vec3(0,0,0);

	// view matrix - orient everything around our preferred view
	glm::mat4 view = glm::lookAt(
		eyePosition,
		lookpos,    // where to look
		glm::vec3(0,1,0)     // up
	);

	glm::vec3 sunPos = glm::vec4(50.0f, 50.0f, -20.0f, 1.0f);
	glm::vec3 test2 = glm::vec3(0, 40, 0);
	RenderData render = {
		eyePosition,
		glm::vec3(0,40,0),
		Vector3{0.8,0.2,0.2},
		shininess
	};

	//Car->modelMeshes[0]->Render( model, view , projection);
	//Car->Render(view,projection);

	//glm::mat4 test = glm::mat4(1);
	//test = glm::scale(test, glm::vec3(2, 2, 2));
	//Model* carr = Models[0];

	for (auto const& model : Models)
	{
		model->Render(view, projection,render);
	}


	//carr->modelMeshes[0]->Render(test, view, projection,render);


	

	// make the draw buffer to display buffer (i.e. display what we have drawn)
	glutSwapBuffers();
}

static void reshape(int w, int h) {
    glViewport(0, 0, w, h);

    width = w;
    height = h;
}

static void drag(int x, int y) {
  if (!isinf(lastX) && !isinf(lastY)) {
    float dx = lastX - (float)x;
    float dy = lastY - (float)y;
    float distance = sqrt(dx * dx + dy * dy);

    if (dy > 0.0f) {
      scaleFactor = SCALE_FACTOR / distance;
    } else {
      scaleFactor = distance / SCALE_FACTOR;
    }
	//Model::scaleFactor = scaleFactor;
  } else {
    lastX = (float)x;
    lastY = (float)y;
  }
}

static void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
      lastX = std::numeric_limits<float>::infinity();
      lastY = std::numeric_limits<float>::infinity();
    }
}

//static void keyboard(unsigned char key, int x, int y) {
//   //std::cout << "Key pressed: " << key << std::endl;
//   Model* car = Models[0];
//   car->Keyboard(key, x, y);
//
//   if (key == 'l') {
//      animateLight = !animateLight;
//   } else if (key == 'r') {
//      rotateObject = !rotateObject;
//   } else if (key == 'w') {
//      shininess += SHININESS_STEP;
//   } else if (key == 's') {
//      shininess -= SHININESS_STEP;
//   }
//   else if (key == 27) {
//	   exit(0);
//   }
//}
//
//static void keyboardUp(unsigned char key, int x, int y) {
//	//std::cout << "Key Released: " << key << std::endl;
//	Model* car = Models[0];
//	car->KeyboardUp(key, x, y);
//}

static void __stdcall openGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {

	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source) {
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

void InitializeOGL(int argc, char** argv)
{
	glutInit(&argc, argv);
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(1100, 140);
	glutInitWindowSize(800, 600);
	glutCreateWindow("CSCI 3090u Shading in OpenGL");

	glutIdleFunc(&update);
	glutDisplayFunc(&render);
	glutReshapeFunc(&reshape);
	glutMotionFunc(&drag);
	glutMouseFunc(&mouse);
	//glutKeyboardFunc(&keyboard);
	//glutKeyboardUpFunc(&keyboardUp);

	glewInit();
	if (!GLEW_VERSION_2_0) {
		std::cerr << "OpenGL 2.0 not available" << std::endl;
		exit(1);
	}

	std::cout << "GLEW Version:   " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "GPU Vendor:     " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GPU Model:      " << glGetString(GL_RENDERER) << std::endl;
}

int main(int argc, char** argv) {
	InitializeOGL(argc, argv);


	ShaderProgram program;
	// program.loadShaders("shaders/gouraud_vertex.glsl", "shaders/gouraud_fragment.glsl");
	program.loadShaders("shaders/phong_vertex.glsl", "shaders/phong_fragment.glsl");
	//program.loadShaders("shaders/phong_vertex.glsl", "shaders/fragment.glsl");
	GLuint programId = program.getProgramId();
	printf("SHADER PROGRAM: %d\n",programId);




	//Model Car();
	//Car = new Model("meshes/Low-Poly-Racing-Car.obj",programId);
	Model* Car = new class Car("meshes/car.obj",programId);
	Car->position += glm::vec3(0, 0, -2.0f);
	Car->scale = glm::vec3(1.0, 1.0, 1.0);
	Car->LoadGeometry();
	Models.push_back(Car);

	Model* Ground = new Model("meshes/cube.obj", programId);
	Ground->scale = glm::vec3(500.0f, 0.1f, 500.0f);
	Ground->LoadGeometry();
	Models.push_back(Ground);

	//Model* ref= new Model("meshes/cube.obj", programId);
	//ref->position.x = -2.0f;
	//ref->LoadGeometry();
	//Models.push_back(ref);
	
	glutMainLoop();

    return 0;
}

void updateProjectionMatrix(int width, int height) {
	// projection matrix - perspective projection
	// FOV:           45°
	// Aspect ratio:  4:3 ratio
	// Z range:       between 0.1 and 100.0
	float aspectRatio = (float)width / (float)height;
	projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 1000.0f);

	// projection matrix - orthographic (non-perspective) projection
	// Note:  These are in world coordinates
	// xMin:          -10
	// xMax:          +10
	// yMin:          -10
	// yMax:          +10
	// zMin:           0
	// zMax:          +100
	//glm::mat4 projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
}

void reshape(GLFWwindow *window, int w, int h) {
	width = w;
	height = h;

	glViewport(0, 0, width, height);

	updateProjectionMatrix(w, h);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	printf("key pressed");
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}


void dragg(GLFWwindow* window, double xpos, double ypos) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	int stateLeft = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int stateRight = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	bool areScaling = stateRight == GLFW_PRESS;
	bool mouseDown = (stateLeft == GLFW_PRESS) || (stateRight == GLFW_PRESS);

	if (mouseDown) {
		int x = (int)xpos;
		int y = (int)ypos;
		if (!std::isinf(lastX) && !std::isinf(lastY)) {
			if (areScaling) {
				float dx = lastX - (float)x;
				float dy = lastY - (float)y;

				// scale in/out (right button drag)
				float distance = sqrt(dx * dx + dy * dy);
				float scaleChange = distance * SCALE_FACTOR;
				if ((dx + dy) > 0.0f) {
					scaleChange *= -1.0f;
				}
				scaleFactor += scaleChange;

				lastX = x;
				lastY = y;
			}
			else {
				//// trackball rotation (left button drag)
				//glm::vec3 a = getTrackballVector(lastX, lastY, width, height);
				//glm::vec3 b = getTrackballVector(x, y, width, height);

				//float rotateSpeed = 1.0f;
				//float angle = ROTATION_FACTOR * -(float)acos(glm::dot(a, b) / glm::length(a) / glm::length(b));

				//if (!std::isnan(angle)) {
				//	glm::vec3 axis = glm::normalize(glm::cross(a, b));

				//	if (std::isnan(axis.x) || std::isnan(axis.y) || std::isnan(axis.z)) {
				//		return;
				//	}

				//	angle *= rotateSpeed;
				//	glm::quat quaternion = glm::angleAxis(-angle, axis);

				//	//glm::mat4 rotationMatrix = glm::mat4_cast(quaternion);
				//	eyePosition = eyePosition * quaternion;

				//	lastX = (float)x;
				//	lastY = (float)y;
				//}
			}
		}
		else {
			lastX = (float)x;
			lastY = (float)y;
		}
	}
}

void mouseg(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_RELEASE) {
		lastX = std::numeric_limits<float>::infinity();
		lastY = std::numeric_limits<float>::infinity();
	}
}

void onError(int error, const char* description) {
	std::cerr << "Error: " << description << std::endl;
}

//int main(int argc, char** argv) {
//
//	// set a function to receive GLFW errors
//	glfwSetErrorCallback(onError);
//
//	// initialize GLFW (windowing abstraction library)
//	if (!glfwInit()) {
//		// initialization failed
//		std::cerr << "GLFW Error:  Unable to initialize GLFW" << std::endl;
//		return -1;
//	}
//
//	width = 900;
//	height = 800;
//
//	// create a window and an OpenGL context
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
//
//	glfwWindowHint(GLFW_SAMPLES, 4);
//
//	GLFWwindow* window = glfwCreateWindow(width, height, "CSCI 3090U Base OpenGL Project", NULL, NULL);
//	if (!window) {
//		// window or OpenGL context creation failed
//		std::cerr << "GLFW Error:  Unable to create window" << std::endl;
//		return -1;
//	}
//
//	glfwSetMouseButtonCallback(window, mouseg);
//	glfwSetCursorPosCallback(window, dragg);
//	glfwSetKeyCallback(window, keyboard);
//	glfwSetFramebufferSizeCallback(window, reshape);
//	updateProjectionMatrix(width, height);
//	glfwSwapInterval(1);
//
//	glfwMakeContextCurrent(window);
//
//	// initialize GLEW (OpenGL extension loading library)
//	glewInit();
//	if (!GLEW_VERSION_2_0) {
//		std::cerr << "OpenGL 2.0 not available" << std::endl;
//		return 1;
//	}
//	std::cout << "GLEW Version:   " << glewGetString(GLEW_VERSION) << std::endl;
//	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
//	std::cout << "GLSL Version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
//	std::cout << "GPU Vendor:     " << glGetString(GL_VENDOR) << std::endl;
//	std::cout << "GPU Model:      " << glGetString(GL_RENDERER) << std::endl;
//
//	// setup the error handling
//	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
//	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
//		glEnable(GL_DEBUG_OUTPUT);
//		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//		glDebugMessageCallback(openGlDebugCallback, nullptr);
//		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
//	}
//
//	ShaderProgram program;
//	// program.loadShaders("shaders/gouraud_vertex.glsl", "shaders/gouraud_fragment.glsl");
//	program.loadShaders("shaders/phong_vertex.glsl", "shaders/phong_fragment.glsl");
//	GLuint programId = program.getProgramId();
//
//
//
//
//	//Model Car();
//	//Car = new Model("meshes/Low-Poly-Racing-Car.obj",programId);
//	Model* Car = new class Car("meshes/cube.obj",programId);
//	Car->position += glm::vec3(0, 0, -25.0f);
//	Car->LoadGeometry();
//	Models.push_back(Car);
//
//	//Model* Ground = new Model("meshes/cube.obj", programId);
//	//Ground->scale = glm::vec3(500.0f, 0.1f, 500.0f);
//	//Ground->LoadGeometry();
//	//Models.push_back(Ground);
//
//	//Model* ref = new Model("meshes/cube.obj", programId);
//	//ref->position.z = -5.0f;
//	//ref->LoadGeometry();
//	//Models.push_back(ref);
//
//
//	while (!glfwWindowShouldClose(window)) {
//		// perform updates (e.g. physics)
//		update();
//
//		// re-draw
//		render();
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	glfwDestroyWindow(window);
//	glfwTerminate();
//
//	return 0;
//}
