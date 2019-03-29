#pragma once
#include <string>
#include <vector>

#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <GLFW/glfw3.h>
#include "Model.h"


class Car : public Model
{
public:
	bool wDown;
	bool aDown;
	bool dDown;
	bool sDown;

	float turnOffset = 0;
	float velocity = 0;

	float slowFactor = 0.1f;
	float accelFactor = 0.8f;

	float maxForward = 15.5f;
	float maxBackward = -2.0f;

	glm::vec3 pivotPoint = glm::vec3(0, 0, 1.1f);

	//Car();
	~Car();
	Car() : Model() {};

	Car(std::string model) : Model(model) {};

	Car(std::string model, GLuint shader) : Model(model, shader) {};


	void Update(int time, int dtime);

	virtual void Keyboard(GLFWwindow*, int, int, int, int);
	void KeyboardUp(unsigned char key, int x, int y);
};

