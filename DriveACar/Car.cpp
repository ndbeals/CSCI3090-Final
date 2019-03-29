#include <cmath>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
//#include <GLFW/glfw3.h>

#include "Car.h"



//Car::Car()
//{
//}


Car::~Car()
{
}


void Car::Update(double time, double dtime)
{
	float dt = (float)dtime;

	//angle.y += 0.05f * (float)dtime;
	//position.z -= 0.001f * float(dtime);
	//angle.z += 0.02f * (float)dtime;
	//angle.x = glm::radians(90.0f);// 3.14159f / 2.0f;
	//angle.y = 3.14159f / 2.0f

	//glm::vec3 scaleVector = glm::vec3(1.0f, 1.0f, 1.0f) * scaleFactor;
	modelm = glm::mat4(1.0f);
	//modelm = glm::scale(modelm, scaleVector);


	//angle.x = 0.0f;
	//angle.y = 10.0f;
	//angle.z = 0.0f;


	//glm::vec3 pivot = glm::vec3(0, 0, 0.5f);

	scale = glm::vec3(1, 1, 1);

	//model = glm::rotate(model, glm::radians(0.01f*(float)c), glm::vec3(0, 1, 0)); // rotate about the y-axis
	modelm = glm::scale(modelm, scale);
	modelm = glm::translate(modelm, position);
	modelm = glm::translate(modelm, pivotPoint);

	modelm = glm::rotate(modelm, glm::radians(angle.x), glm::vec3(1, 0, 0)); // rotate about the x-axis
	modelm = glm::rotate(modelm, glm::radians(angle.y), glm::vec3(0, 1, 0)); // rotate about the y-axis
	modelm = glm::rotate(modelm, glm::radians(angle.z), glm::vec3(0, 0, 1)); // rotate about the z-axis
	modelm = glm::translate(modelm, -pivotPoint);


	//modelm = glm::translate(modelm, position);

	//printf("X: %f\n", angle.x);


	glm::mat4 tm = glm::mat4(1.0f);
	tm = glm::rotate(tm, glm::radians(angle.x), glm::vec3(1, 0, 0)); // rotate about the x-axis
	tm = glm::rotate(tm, glm::radians(angle.y), glm::vec3(0, 1, 0)); // rotate about the y-axis
	tm = glm::rotate(tm, glm::radians(angle.z), glm::vec3(0, 0, 1)); // rotate about the z-axis

	glm::vec3 forward = glm::normalize((tm * glm::vec4(0, 0, -1, 1)));

	if (wDown)
	{
		velocity += 0.08f;
		velocity = std::min(velocity, maxForward);

		//if (velocity>1.4)
		//{
		//	//printf("a");
		//}

		//position += forward * dt * velocity;
		//position.z -= 0.01f;
	}
	else if (sDown)
	{
		if (velocity>0)
		{
			velocity -= 0.1f;

		}
		velocity -= 0.05f;
		
		velocity = std::max(velocity, maxBackward);
	}
	else
	{
		if (velocity>0)
		{
			velocity -= slowFactor;
		}
		else
		{
			velocity += slowFactor;
		}
	}
	position += forward * velocity * dt;

	//if
	//{
	//	 
	//}

	if (aDown)
	{
		angle.y += 100.0f * dt;
	}
	else if (dDown)
	{
		angle.y -= 100.0f * dt;
		//angle.y -= turnOffset * dt;

	}
}

void Car::Keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action==GLFW_PRESS)
	{
		if (key == GLFW_KEY_W)
		{
			//position.x += 0.1f;
			//position = modelm * glm::vec4(0, 0, -1,1);
			wDown = true;
		}
		else if (key == GLFW_KEY_S)
		{
			//position.x -= 0.1f;
			sDown = true;
		}
		else if (key == GLFW_KEY_A)
		{
			//angle.y += 0.15f;
			aDown = true;
			dDown = false;
		}
		else if (key== GLFW_KEY_D)
		{
			//angle.y -= 0.4f;
			dDown = true;
			aDown = false;
		}
	}
	else if (action==GLFW_RELEASE)
	{
		if (key == GLFW_KEY_W)
		{
			//position.x += 0.1f;
			//position = modelm * glm::vec4(0, 0, -1,1);
			wDown = false;
		}
		else if (key == GLFW_KEY_S)
		{
			//position.x -= 0.1f;
			sDown = false;
		}
		else if (key == GLFW_KEY_A)
		{
			//angle.y += 0.15f;
			aDown = false;
		}
		else if (key == GLFW_KEY_D)
		{
			//angle.y -= 0.4f;
			dDown = false;
		}
	}
}

void Car::KeyboardUp(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		//position.x += 0.1f;
		//position = modelm * glm::vec4(0, 0, -1,1);
		wDown = false;
	}
	else if (key == 's')
	{
		//position.x -= 0.1f;
		sDown = false;
	}
	else if (key == 'a')
	{
		//angle.y += 0.15f;
		aDown = false;
	}
	else if (key == 'd')
	{
		//angle.y -= 0.4f;
		dDown = false;
	}
}