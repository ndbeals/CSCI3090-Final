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
#include "ObjMesh.h"
//#include "error.h"


#include "Model.h"
float scaleFactor;

//struct test22
//{
//	float shininess;
//};
//
//struct RenderData
//{
//	glm::vec3 eyePosition;
//	glm::vec3 lightPosition;
//	Vector3 diffuseColour;
//	float shininess;
//};

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "error.h"

//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/transform.hpp>

#define SCALE_FACTOR 2.0f
#define SHININESS_STEP 1.0f

int width, height;

GLuint programId;
GLuint vertexBuffer;
GLuint indexBuffer;
GLenum positionBufferId;
GLuint positions_vbo = 0;
GLuint textureCoords_vbo = 0;
GLuint normals_vbo = 0;
GLuint colours_vbo = 0;

unsigned int numVertices;

float angle = 0.0f;
float lightOffsetY = 0.0f;
//glm::vec3 eyePosition(4, 3, 5);
//glm::vec3 eyePosition(40, 30, 30);
glm::vec3 eyePosition(0, 30, 30);
bool animateLight = true;
bool rotateObject = true;
//float scaleFactor = 1.0f;
float lastX = std::numeric_limits<float>::infinity();
float lastY = std::numeric_limits<float>::infinity();
float shininess = 25.0f;


Model* Car;

// Global Variables and data containers
std::vector<Model*> Models;



static void update(void) {
	//int milliseconds = 50;
    int milliseconds = glutGet(GLUT_ELAPSED_TIME);

    // rotate the shape about the y-axis so that we can see the shading
    if (rotateObject) {
      float degrees = (float)milliseconds / 80.0f;
      angle = degrees;
    }

    // move the light position over time along the x-axis, so we can see how it affects the shading
    if (animateLight) {
      float t = milliseconds / 1000.0f;
      lightOffsetY = sinf(t) * 100.0f;
    }

    glutPostRedisplay();
}

static void render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// turn on depth buffering
	glEnable(GL_DEPTH_TEST);

	// projection matrix - perspective projection
	// FOV:           45°
	// Aspect ratio:  4:3 ratio
	// Z range:       between 0.1 and 100.0
	float aspectRatio = (float)width / (float)height;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.001f, 1000.0f);

	// projection matrix - orthographic (non-perspective) projection
	// Note:  These are in world coordinates
	// xMin:          -10
	// xMax:          +10
	// yMin:          -10
	// yMax:          +10
	// zMin:           0
	// zMax:          +100
	//glm::mat4 projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);

	// view matrix - orient everything around our preferred view
	glm::mat4 view = glm::lookAt(
		eyePosition,
		glm::vec3(0,0,0),    // where to look
		glm::vec3(0,1,0)     // up
	);

	// model matrix: translate, scale, and rotate the model
	//glm::vec3 rotationAxis(0,1,0);
	glm::mat4 mmodel = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(-6.0f, -2.0f, 1.0));
	mmodel = glm::rotate(mmodel, glm::radians(angle*5.0f), glm::vec3(0, 1, 0)); // rotate about the y-axis
	mmodel = glm::scale(mmodel, glm::vec3(scaleFactor, scaleFactor, scaleFactor));

	glm::vec3 test = glm::vec3(1, 8 + lightOffsetY, -2);

	RenderData render = {
		eyePosition,
		test,
		Vector3{0.8,0.2,0.2},
		shininess
	};

	//test22 render = test22{ shininess };

	//Car->modelMeshes[0]->Render( model, view , projection);
	//Car->Render(view,projection);

	for (auto const& model : Models)
	{
		model->Render(view, projection,render);
		//model->modelMeshes[0]->Render(mmodel, view, projection,render);
		//model->modelMeshes[1]->Render(mmodel, view, projection,render);

		//mmodel = glm::translate(mmodel, glm::vec3(10.0f, 0.0f, 0.0f));
		//model->modelMeshes[1]->Render(mmodel, view, projection,render);
		//model->modelMeshes[0]->Render(mmodel, view, projection,shininess);
		//model->modelMeshes[0]->Render(mmodel, view, projection);
	}

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
	Model::scaleFactor = scaleFactor;
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

static void keyboard(unsigned char key, int x, int y) {
   std::cout << "Key pressed: " << key << std::endl;
   if (key == 'l') {
      animateLight = !animateLight;
   } else if (key == 'r') {
      rotateObject = !rotateObject;
   } else if (key == 'w') {
      shininess += SHININESS_STEP;
   } else if (key == 's') {
      shininess -= SHININESS_STEP;
   }
   else if (key == 27) {
	   exit(0);
   }
}

//static void __stdcall openGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
//
//	// ignore non-significant error/warning codes
//	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
//
//	std::cout << "---------------" << std::endl;
//	std::cout << "Debug message (" << id << "): " << message << std::endl;
//
//	switch (source) {
//	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
//	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
//	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
//	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
//	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
//	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
//	} std::cout << std::endl;
//
//	switch (type) {
//	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
//	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
//	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
//	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
//	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
//	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
//	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
//	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
//	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
//	} std::cout << std::endl;
//
//	switch (severity) {
//	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
//	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
//	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
//	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
//	} std::cout << std::endl;
//	std::cout << std::endl;
//}


int main(int argc, char** argv) {
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
    glutKeyboardFunc(&keyboard);

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


	ShaderProgram program;
	// program.loadShaders("shaders/gouraud_vertex.glsl", "shaders/gouraud_fragment.glsl");
	program.loadShaders("shaders/phong_vertex.glsl", "shaders/phong_fragment.glsl");
	programId = program.getProgramId();


	//Model Car();
	//Car = new Model("meshes/Low-Poly-Racing-Car.obj",programId);
	Car = new Model("meshes/car.obj",programId);

	Car->position += glm::vec3(20.0f, 0, 0);

	printf("pos vbo init: %d\n", positions_vbo);
    //createGeometry();
	printf("pos vbo post def: %d\n", positions_vbo);

	Car->LoadGeometry();

	

	//numVertices = Car->modelMeshes.at(0)->VertexCount;
	//positions_vbo = Car->modelMeshes.at(0)->positionsVBO;
	//textureCoords_vbo = Car->modelMeshes.at(0)->textureCoordsVBO;
	//normals_vbo = Car->modelMeshes.at(0)->normalsVBO;
	//indexBuffer = Car->modelMeshes.at(0)->indexBuffer;

	//printf("pos vbo post class: %d\n", positions_vbo);



	//Car->modelMeshes[0]->shaderProgram = programId;
	//Car->modelMeshes[1]->shaderProgram = programId;

	Models.push_back(Car);
	
	glutMainLoop();

    return 0;
}


//void onError(int error, const char* description) {
//	std::cerr << "Error: " << description << std::endl;
//}
//
//int main(int argc, char** argv) {
//
//	// set a function to receive GLFW errors
//	//glfwSetErrorCallback(onError);
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
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
//	GLFWwindow* window = glfwCreateWindow(width, height, "CSCI 3090U Base OpenGL Project", NULL, NULL);
//	if (!window) {
//		// window or OpenGL context creation failed
//		std::cerr << "GLFW Error:  Unable to create window" << std::endl;
//		return -1;
//	}
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
//	programId = program.getProgramId();
//
//
//	//Model Car();
//	//Car = new Model("meshes/Low-Poly-Racing-Car.obj",programId);
//	Car = new Model("meshes/triangle.obj", programId);
//
//	printf("pos vbo init: %d\n", positions_vbo);
//	//createGeometry();
//	printf("pos vbo post def: %d\n", positions_vbo);
//
//	Car->LoadGeometry();
//
//
//
//	//numVertices = Car->modelMeshes.at(0)->VertexCount;
//	//positions_vbo = Car->modelMeshes.at(0)->positionsVBO;
//	//textureCoords_vbo = Car->modelMeshes.at(0)->textureCoordsVBO;
//	//normals_vbo = Car->modelMeshes.at(0)->normalsVBO;
//	//indexBuffer = Car->modelMeshes.at(0)->indexBuffer;
//
//	//printf("pos vbo post class: %d\n", positions_vbo);
//
//
//
//	//Car->modelMeshes[0]->shaderProgram = programId;
//	//Car->modelMeshes[1]->shaderProgram = programId;
//
//	Models.push_back(Car);
//
//	//glfwSetMouseButtonCallback(window, mouse);
//	//glfwSetCursorPosCallback(window, drag);
//	//glfwSetKeyCallback(window, keyboard);
//	//glfwSetFramebufferSizeCallback(window, reshape);
//	//updateProjectionMatrix(width, height);
//	glfwSwapInterval(1);
//
//	// determine the initial camera position
//	//eyePosition.x = 0.0f;
//	//eyePosition.y = 0.0f;
//	//eyePosition.z = cameraDistance;
//
//	//createGeometry();
//	//programId = createShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");
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
