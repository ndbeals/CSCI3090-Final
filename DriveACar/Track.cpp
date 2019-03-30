#include "Track.h"
#include <cmath>
#include <random>
#define PI 3.141592f

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

//glm::vec3 controlPoint1(-3.00f, 0, 3.00f);
//glm::vec3 controlPoint2(2.50f, 0.0f, -1.50f);
//glm::vec3 controlPoint3(-6.00f, 0.0f, -5.50f);
//glm::vec3 controlPoint4(2.00f, 0.0f, -10.50f);

void Track::LoadGeometry()
{
	Mesh* newMesh = new Mesh();

	std::vector<Vector3> controlPoints;
	std::vector<Vector3> vertexPositions;

	std::vector<Vector3> vertexBuffer;
	std::vector<Vector3> normalBuffer;
	std::vector<Vector2> textureCoordBuffer;
	std::vector<unsigned int> indexBuffer;


	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(-20.0f, 20.0f);
	std::uniform_real_distribution<float> pdist(15.0f, 50.0f);

	float stepSize = 1.0f / 100.0f;

	float radius = pdist(mt);
	//float periods = pdist(mt) / 5.0f;
	float periods = 2.0f;

	position.x = radius / 2.0f;

	float accumx = 0.0f;
	float accumz = 0.0f;
	for (float t = 0.0f; t < (1.0f + stepSize); t += stepSize)
	{
		float rz = dist(mt);
		//float z = (t * 100.0f);// +rz;
		//float z = sin(5.0f*t)*100.0f;
		float z = radius * sin(t * periods * PI) + rz * sin(t * periods * PI);
		accumz += 0.1f*z + exp(t);

		float rx = (dist(mt));
		//float x = rx;
		//float x = 100.0f*t+ cos(2.0f*t)*1.0f;
		float x = radius * cos(t * periods * PI) + rz * cos(t * periods * PI);
		accumx += 3.0f*t + 1;
		//float x = rx;

		controlPoints.emplace_back(Vector3{ x,0.0f,z });
	}
	
	stepSize = 1.0f / 50.0f;

	for (int cp = 3; cp < controlPoints.size(); cp+=2)
	{
		Vector3 controlPoint1 = controlPoints[cp - 3];
		Vector3 controlPoint2 = controlPoints[cp - 2];
		Vector3 controlPoint3 = controlPoints[cp - 1];
		Vector3 controlPoint4 = controlPoints[cp - 0];


		// cubic interpolation.
		unsigned int index = 0;
		for (float t = 0.20f; t < (0.75f + stepSize); t += stepSize)
		{
			float vx = pow((1 - t), 3) * controlPoint1.x + pow((1 - t), 2) * 3 * t*controlPoint2.x + (1 - t) * 3 * pow(t, 2)*controlPoint3.x + pow(t, 3)*controlPoint4.x;
			float vy = pow((1 - t), 3) * controlPoint1.y + pow((1 - t), 2) * 3 * t*controlPoint2.y + (1 - t) * 3 * pow(t, 2)*controlPoint3.y + pow(t, 3)*controlPoint4.y;
			float vz = pow((1 - t), 3) * controlPoint1.z + pow((1 - t), 2) * 3 * t*controlPoint2.z + (1 - t) * 3 * pow(t, 2)*controlPoint3.z + pow(t, 3)*controlPoint4.z;
			//float vy = 0;


			vertexPositions.emplace_back(Vector3{ vx - 0.5f,vy,vz-0.5f });
			vertexPositions.emplace_back(Vector3{ vx + 0.5f,vy,vz+0.5f});
		}
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
