#pragma once
#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <GLFW/glfw3.h>
//#include


struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Vector2 {
	float y;
	float x;
};

struct RenderData
{
	glm::vec3 eyePosition;
	glm::vec3 lightPosition;
	Vector3 diffuseColour;
	float shininess;
};

//float scaleFactor = 1.0f;


class Mesh
{
public:
	static std::map<std::string, GLuint> loadedTextures;

	unsigned int VertexCount;
	GLuint positionsVBO = 0;
	GLuint textureCoordsVBO = 0;
	GLuint normalsVBO = 0;
	GLuint indexBuffer = 0;

	GLuint shaderProgram = 0;
	GLuint textureId = -1;
	std::string textureName = "";

	float shininess = 25.0f;
	Vector3 ambientColour;
	Vector3 diffuseColour;
	Vector3 specularColour;


	Mesh();
	~Mesh();

	void LoadGeometry(std::vector<Vector3> vertexBuffer, std::vector<Vector3> normalBuffer, std::vector<Vector2> textureCoordBuffer, std::vector<unsigned int> indexBuffer);

	void LoadTexture();

	void Render(const glm::mat4 & model, const glm::mat4 & view, const glm::mat4 & projection, const RenderData & data);
};

class Model
{
public:
	//static std::vector<Model*> Models;

	//static float scaleFactor;

	std::string modelPath;

	std::vector<Mesh*> modelMeshes;
	GLuint shaderProgram = 0;

	glm::vec3 scale;
	glm::vec3 position;
	glm::vec3 angle;
	glm::mat4 modelm;


	Model()
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		angle = glm::vec3(0, 0, 0);
		modelm = glm::mat4(1);

		scale = glm::vec3(1, 1, 1);
	}
	~Model();

	Model(std::string model) : modelPath(model) {
		Model();
	}

	Model(std::string model, GLuint shader) : modelPath(model), shaderProgram(shader) 
	{
		Model();
	}


	void LoadGeometry();

	void Update(int time, int dtime);

	void Render(const glm::mat4 & view, const glm::mat4 & projection, const RenderData & data);
	

	virtual void Keyboard(GLFWwindow*, int, int, int, int) {};

	//virtual void KeyboardUp(unsigned char, int, int) {};
};

