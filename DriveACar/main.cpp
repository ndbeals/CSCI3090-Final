#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cmath>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>



#include "ShaderProgram.h"
//#include "ObjMesh.h"
#include "error.h"

#include "Model.h"
#include "Car.h"
#include "Track.h"




float scaleFactor = 1.0f;
#define SCALE_FACTOR 2.0f
#define SHININESS_STEP 1.0f

int width, height;


float lightOffsetY = 0.0f;
//glm::vec3 eyePosition(4, 3, 5);
//glm::vec3 eyePosition(40, 30, 30);


glm::vec3 eyePosition(10, 10, 10);
bool animateLight = true;



float lastX = std::numeric_limits<float>::infinity();
float lastY = std::numeric_limits<float>::infinity();
float shininess = 25.0f;


//Model* Car;

// Global Variables and data containers
//std::vector<Model*> Models;
std::vector<Model*> Models;
glm::mat4 projection;


double lastTime = 0;
double nowTime = 0;
int dt = 0;
static void update(void) {
	nowTime = glfwGetTime();

    // move the light position over time along the x-axis, so we can see how it affects the shading
    if (animateLight) {
      float t = nowTime / 2500.0f;
      lightOffsetY = sinf(t) * 40.0f;
    }

	for (auto const& model : Models)
	{
		model->Update( nowTime , nowTime-lastTime);
	}
	lastTime = nowTime;
}

static void render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// turn on depth buffering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);


	Model* car = Models[0];
	glm::vec4 test = glm::vec4(1.0f, 6.0f, 8.0f, 1);

	//glm::vec3 lookpos = car->modelm*glm::vec4(0,0,-5,1);
	eyePosition = car->modelm*test;

	//Model* car = Models[0];
	glm::vec3 lookpos = car->modelm*glm::vec4(0,0,-5,1);

	// view matrix - orient everything around our preferred view
	glm::mat4 view = glm::lookAt(
		eyePosition,
		lookpos,    // where to look
		glm::vec3(0,1,0)     // up
	);

	glm::vec4 sunPos = view*glm::vec4(0.0f, 50.0f, 0.0f, 1.0f);
	glm::vec3 test2 = glm::vec3(0, 40, 0);
	RenderData render = {
		eyePosition,
		glm::vec3(sunPos),
		Vector3{0.8,0.2,0.2},
		shininess
	};

	//Car->modelMeshes[0]->Render( model, view , projection);
	//Car->Render(view,projection);

	for (auto const& model : Models)
	{
		model->Render(view, projection,render);
	}

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
	Models[0]->Keyboard(window, key, scancode, action, mods);

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

int main(int argc, char** argv) {

	// set a function to receive GLFW errors
	glfwSetErrorCallback(onError);

	// initialize GLFW (windowing abstraction library)
	if (!glfwInit()) {
		// initialization failed
		std::cerr << "GLFW Error:  Unable to initialize GLFW" << std::endl;
		return -1;
	}

	width = 1800;
	height = 1200;

	// create a window and an OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(width, height, "CSCI 3090U Base OpenGL Project", NULL, NULL);
	if (!window) {
		// window or OpenGL context creation failed
		std::cerr << "GLFW Error:  Unable to create window" << std::endl;
		return -1;
	}

	glfwSetMouseButtonCallback(window, mouseg);
	glfwSetCursorPosCallback(window, dragg);
	glfwSetKeyCallback(window, keyboard);
	glfwSetFramebufferSizeCallback(window, reshape);
	updateProjectionMatrix(width, height);
	glfwSwapInterval(1);

	glfwMakeContextCurrent(window);

	// initialize GLEW (OpenGL extension loading library)
	glewInit();
	if (!GLEW_VERSION_2_0) {
		std::cerr << "OpenGL 2.0 not available" << std::endl;
		return 1;
	}
	std::cout << "GLEW Version:   " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "GPU Vendor:     " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GPU Model:      " << glGetString(GL_RENDERER) << std::endl;

	// setup the error handling
	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openGlDebugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	ShaderProgram phong;
	// phong.loadShaders("shaders/gouraud_vertex.glsl", "shaders/gouraud_fragment.glsl");
	phong.loadShaders("shaders/phong_vertex.glsl", "shaders/phong_fragment.glsl");
	GLuint phongId = phong.getProgramId();

	ShaderProgram fullbright;
	// program.loadShaders("shaders/gouraud_vertex.glsl", "shaders/gouraud_fragment.glsl");
	fullbright.loadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");
	GLuint fullbrightId = fullbright.getProgramId();

	ShaderProgram skybox;
	// program.loadShaders("shaders/gouraud_vertex.glsl", "shaders/gouraud_fragment.glsl");
	skybox.loadShaders("shaders/skybox_vertex.glsl", "shaders/skybox_fragment.glsl");
	GLuint skyboxId = skybox.getProgramId();


	Model* Car = new class Car("meshes/car.obj",phongId);
	Car->position += glm::vec3(0, 0.15f, 5.0f);
	Car->scale = glm::vec3(1.0, 1.0, 1.0);
	Car->LoadGeometry();
	Models.push_back(Car);

	Model* Ground = new Model("meshes/cube.obj", phongId);
	Ground->scale = glm::vec3(500.0f, 0.1f, 500.0f);
	Ground->position = glm::vec3(0, 0.0f, 0);
	Ground->LoadGeometry();
	Models.push_back(Ground);

	Model* Ground2 = new Model("meshes/skybox.obj", skyboxId);
	Ground2->scale = glm::vec3(1000.0f, -500.5f, 1000.0f);
	Ground2->position = glm::vec3(0, 0, 0);
	Ground2->LoadGeometry();
	Models.push_back(Ground2);


	Model* ref = new Track(fullbrightId);
	//ref->position.z = .0f;
	ref->position = glm::vec3(0.0f, 0.1f, 0.0f);
	ref->LoadGeometry();
	Models.push_back(ref);


	printf("\n\nDrive along the track!\n\tUse W A S D to control the car\n");


	while (!glfwWindowShouldClose(window)) {
		// perform updates (e.g. physics)
		update();

		// re-draw
		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
