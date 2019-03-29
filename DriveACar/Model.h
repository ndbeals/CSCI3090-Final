#pragma once
#include <string>
#include <vector>

#include <GL/glew.h>



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
	unsigned int VertexCount;
	GLuint positionsVBO = 0;
	GLuint textureCoordsVBO = 0;
	GLuint normalsVBO = 0;
	GLuint indexBuffer = 0;

	GLuint shaderProgram = 0;

	Mesh();
	~Mesh();

	void LoadGeometry(std::vector<Vector3> vertexBuffer, std::vector<Vector3> normalBuffer, std::vector<Vector2> textureCoordBuffer, std::vector<unsigned int> indexBuffer);

	void Render(glm::mat4 model, const glm::mat4 & view, const glm::mat4 & projection, RenderData data);

};

class Model
{
public:
	static float scaleFactor;

	std::string modelPath;

	std::vector<Mesh*> modelMeshes;
	GLuint shaderProgram = 0;

	glm::vec3 position;
	glm::vec3 angle;


	Model()
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
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

	void Render(const glm::mat4 & view, const glm::mat4 & projection, RenderData data);

	virtual void Update(int) {};
};

