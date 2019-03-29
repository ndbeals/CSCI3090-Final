#include "Track.h"
#include <cmath>


Track::Track()
{
}

Track::Track(GLuint shader)
{
	shaderProgram = shader;
}


Track::~Track()
{
}

glm::vec3 controlPoint1(-2.00f, -1.50f, 2.00f);
glm::vec3 controlPoint2(2.50f, 2.50f, -0.50f);
glm::vec3 controlPoint3(-1.00f, 1.00f, -2.50f);
glm::vec3 controlPoint4(2.00f, -2.50f, -0.50f);

void Track::LoadGeometry()
{
	Mesh* newMesh = new Mesh();

	std::vector<Vector3> vertexPositions;

	std::vector<Vector3> vertexBuffer;
	std::vector<Vector3> normalBuffer;
	std::vector<Vector2> textureCoordBuffer;
	std::vector<unsigned int> indexBuffer;

	float stepSize = 1.0f / 100.0f;

	//vertexBuffer.emplace_back(Vector3{ 0.0f,0.0f,0.0f });
	//vertexBuffer.emplace_back(Vector3{ 1.0f,0.0f,0.0f });

	unsigned int index = 0;
	for (float t = 0; t < (1 + stepSize); t += stepSize)
	{
		//float vx = pow((1 - t), 3) * controlPoint1.x + pow((1 - t), 2) * 3 * t*controlPoint2.x + (1 - t) * 3 * pow(t, 2)*controlPoint3.x + pow(t, 3)*controlPoint4.x;
		//float vy = pow((1 - t), 3) * controlPoint1.y + pow((1 - t), 2) * 3 * t*controlPoint2.y + (1 - t) * 3 * pow(t, 2)*controlPoint3.y + pow(t, 3)*controlPoint4.y;
		//float vz = pow((1 - t), 3) * controlPoint1.z + pow((1 - t), 2) * 3 * t*controlPoint2.z + (1 - t) * 3 * pow(t, 2)*controlPoint3.z + pow(t, 3)*controlPoint4.z;

		float vy = 0;

		float vx = 3.0f*sin(10.0f*3.141590f*t);
		float vz = -50.0f*t;

		vertexPositions.emplace_back(Vector3{ vx - 0.5f,vy,vz });
		vertexPositions.emplace_back(Vector3{ vx + 0.5f,vy,vz });

		//indexBuffer.emplace_back(index);
		//index++;
	}

	//vertexBuffer.emplace_back(Vector3{ -10.0f,0.0f,10.0f });
	//vertexBuffer.emplace_back(Vector3{ 10.0f,0.0f,10.0f });
	//vertexBuffer.emplace_back(Vector3{ -10.0f,0.0f,-10.0f });
	//vertexBuffer.emplace_back(Vector3{ -10.0f,0.0f,-10.0f });
	//vertexBuffer.emplace_back(Vector3{ 10.0f,0.0f,10.0f });
	//vertexBuffer.emplace_back(Vector3{ 10.0f,0.0f,-10.0f });


	//normalBuffer.emplace_back(Vector3{ 0.0f,1.0f,0.0f });

	//indexBuffer.emplace_back(0);
	//indexBuffer.emplace_back(1);
	//indexBuffer.emplace_back(2);
	//indexBuffer.emplace_back(3);
	//indexBuffer.emplace_back(4);
	//indexBuffer.emplace_back(5);

	unsigned int newIdx = 0;
	for (unsigned int idx = 3; idx < vertexPositions.size(); idx+=2)
	{
		Vector3 p1 = vertexPositions[idx - 3];
		Vector3 p2 = vertexPositions[idx - 2];
		Vector3 p3 = vertexPositions[idx - 1];
		Vector3 p4 = vertexPositions[idx - 0];

		vertexBuffer.push_back(p1);
		vertexBuffer.push_back(p2);
		vertexBuffer.push_back(p3);
		indexBuffer.push_back(newIdx);
		indexBuffer.push_back(newIdx+1);
		indexBuffer.push_back(newIdx+2);
		newIdx += 3;

		vertexBuffer.push_back(p3);
		vertexBuffer.push_back(p2);
		vertexBuffer.push_back(p4);

		indexBuffer.push_back(newIdx);
		indexBuffer.push_back(newIdx + 1);
		indexBuffer.push_back(newIdx + 2);
		newIdx += 3;
	}

	newMesh->LoadGeometry(vertexBuffer, normalBuffer, textureCoordBuffer, indexBuffer);

	newMesh->shaderProgram = shaderProgram;

	newMesh->ambientColour = Vector3{ 1.0,1.0,1.0};
	newMesh->diffuseColour = Vector3{ 1.0,1.0,1.0 };


	modelMeshes.push_back(newMesh);


	//vertexBuffer.clear();
	//normalBuffer.clear();
	//textureCoordBuffer.clear();
	//indexBuffer.clear();
}
