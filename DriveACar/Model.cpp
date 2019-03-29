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

#include "Model.h"
#include "ObjMesh.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tinyobj/tiny_obj_loader.h"

//#include "main.cpp"
//float scaleFactor = 0;

//Model::Model()
//{
//}

Model::~Model()
{
}


void Model::LoadGeometry()
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str(),"meshes/",true);
	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}
	if (!ret) {
		exit(1);
	}

	printf("# of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
	printf("# of normals   = %d\n", (int)(attrib.normals.size()) / 3);
	printf("# of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
	printf("# of materials = %d\n", (int)materials.size());
	printf("# of shapes    = %d\n", (int)shapes.size());


	// Loop over shapes
	for (size_t shapeId = 0; shapeId < shapes.size(); shapeId++) {
		Mesh* newMesh = new Mesh();

		std::vector<Vector3> vertexBuffer;
		std::vector<Vector3> normalBuffer;
		std::vector<Vector2> textureCoordBuffer;
		std::vector<unsigned int> indexBuffer;

		vertexBuffer.clear();
		normalBuffer.clear();
		textureCoordBuffer.clear();
		indexBuffer.clear();

		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[shapeId].mesh.num_face_vertices.size(); f++)
		{
			size_t fv = shapes[shapeId].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[shapeId].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];

				//printf("x: %f  y: %f  z: %f \n", vx,vy,vz);
				//printf("idx: %d  x: %f  y: %f  z: %f\n",idx.vertex_index, vx, vy, vz);

				vertexBuffer.emplace_back(Vector3{ vx, vy, vz });
				normalBuffer.emplace_back(Vector3{ nx, ny, nz });
				textureCoordBuffer.emplace_back(Vector2{ tx, ty });

				indexBuffer.emplace_back(index_offset + v);
				//printf("nx: %f  ny: %f  nz: %f\n", nx, ny, nz);
				//printf("tx: %f  ty: %f \n", tx, ty);
				// Optional: vertex colors
				// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
				// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
				// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
			}
			index_offset += fv;

			// per-face material
			//shapes[shapeId].mesh.material_ids[f];
		}

		newMesh->LoadGeometry(vertexBuffer, normalBuffer, textureCoordBuffer, indexBuffer);

		newMesh->shaderProgram = shaderProgram;

		modelMeshes.push_back(newMesh);
	}


	//


}

void Model::Render(const glm::mat4& view, const glm::mat4& projection, RenderData data )
{
	glm::vec3 scaleVector = glm::vec3(1.0f, 1.0f, 1.0f) * scaleFactor;

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, position);
	model = glm::scale(model, scaleVector);

	for (auto const& mesh : modelMeshes)
	{
		mesh->Render(model,view, projection,data);
	}
}

Mesh::Mesh()
{

}
Mesh::~Mesh()
{

}

float Model::scaleFactor = 2.0f;

void Mesh::LoadGeometry( std::vector<Vector3> vertexBuffer, std::vector<Vector3> normalBuffer, std::vector<Vector2> textureCoordBuffer, std::vector<unsigned int> newIndexBuffer)
{
	//ObjMesh mesh;
	//mesh.load("meshes/2cube.obj", false,false);

	//VertexCount = mesh.getNumIndexedVertices();
	VertexCount = newIndexBuffer.size();
	//Vector3* vertexPositions = mesh.getIndexedPositions();
	Vector3* vertexPositions = vertexBuffer.data();
	//Vector2* vertexTextureCoords = mesh.getIndexedTextureCoords();
	Vector2* vertexTextureCoords = textureCoordBuffer.data();
	//Vector3* vertexNormals = mesh.getIndexedNormals();
	Vector3* vertexNormals = normalBuffer.data();
	//unsigned int* indexData = mesh.getTriangleIndices();
	unsigned int* indexData = newIndexBuffer.data();

	printf("  LOADED SHAPE: VC: %d\n", VertexCount);

	//int numTriangles = mesh.getNumTriangles();

	glGenBuffers(1, &positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, VertexCount * sizeof(Vector3), vertexPositions, GL_STATIC_DRAW);

	glGenBuffers(1, &textureCoordsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, VertexCount * sizeof(Vector2), vertexTextureCoords, GL_STATIC_DRAW);

	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, VertexCount * sizeof(Vector3), vertexNormals, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * VertexCount, indexData, GL_STATIC_DRAW);
}

int c = 0;
void Mesh::Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, RenderData data)
{
	//printf("??? %f\n", data.shininess);
	c = glutGet(GLUT_ELAPSED_TIME);
	// activate our Meshes shader program
	glUseProgram(shaderProgram);

	float scaleFactor = 1.0f;
	// model matrix: translate, scale, and rotate the model
	glm::vec3 rotationAxis(0, 1, 0);
	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(-6.0f, -2.0f, 1.0));
	model = glm::rotate(model, glm::radians(0.01f*(float)c), glm::vec3(0, 1, 0)); // rotate about the y-axis
	model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));

	// model-view-projection matrix
	glm::mat4 mvp = projection * view * model;
	GLuint mvpMatrixId = glGetUniformLocation(shaderProgram, "u_MVPMatrix");
	glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &mvp[0][0]);

	// model-view matrix
	glm::mat4 mv = view * model;
	GLuint mvMatrixId = glGetUniformLocation(shaderProgram, "u_MVMatrix");
	glUniformMatrix4fv(mvMatrixId, 1, GL_FALSE, &mv[0][0]);


	// the position of our camera/eye
	GLuint eyePosId = glGetUniformLocation(shaderProgram, "u_EyePosition");
	//glUniform3f(eyePosId, data.eyePosition.x, data.eyePosition.y, data.eyePosition.z);
	glUniform3f(eyePosId, 40.0f,30.0f,30.0f);

	// the position of our light
	GLuint lightPosId = glGetUniformLocation(shaderProgram, "u_LightPos");
	glUniform3f(lightPosId, data.lightPosition.x,data.lightPosition.y,data.lightPosition.z);
	//glUniform3f(lightPosId, 10.0f,7.0f,3.0f);

	// the colour of our object
	GLuint diffuseColourId = glGetUniformLocation(shaderProgram, "u_DiffuseColour");
	glUniform4f(diffuseColourId, 0.8, 0.2, 0.2, 1.0);

	// the shininess of the object's surface
	GLuint shininessId = glGetUniformLocation(shaderProgram, "u_Shininess");
	glUniform1f(shininessId, 25.0f);


	// find the names (ids) of each vertex attribute
	GLint positionAttribId = glGetAttribLocation(shaderProgram, "position");
	GLint textureCoordsAttribId = glGetAttribLocation(shaderProgram, "textureCoords");
	GLint normalAttribId = glGetAttribLocation(shaderProgram, "normal");

	// provide the vertex positions to the shaders
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glEnableVertexAttribArray(positionAttribId);
	glVertexAttribPointer(positionAttribId, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//// provide the vertex texture coordinates to the shaders
	//glBindBuffer(GL_ARRAY_BUFFER, textureCoordsVBO);
	//glEnableVertexAttribArray(textureCoordsAttribId);
	//glVertexAttribPointer(textureCoordsAttribId, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	//// provide the vertex normals to the shaders
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glEnableVertexAttribArray(normalAttribId);
	glVertexAttribPointer(normalAttribId, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// draw the triangles
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, VertexCount, GL_UNSIGNED_INT, (void*)0);

	// for testing purposes
	//glutSolidTorus(0.5f, 1.5f, 12, 10);

	  // disable the attribute arrays
	glDisableVertexAttribArray(positionAttribId);
	//glDisableVertexAttribArray(textureCoordsAttribId);
	glDisableVertexAttribArray(normalAttribId);


}

//static void createGeometry(void) {
//	ObjMesh mesh;
//	//mesh.load("meshes/apple.obj", true, true);
//	//mesh.load("meshes/teapot.obj", true, true);
//	//mesh.load("meshes/horse.obj", true, true);
//	//mesh.load("meshes/LEGO_CAR_B2.obj", true, true);
//	//mesh.load("meshes/sphere2.obj", true, true);
//	//mesh.load("meshes/jeep2.obj", true, true);
//	mesh.load("meshes/Low-Poly-Racing-Car.obj", true, true);
//
//	numVertices = mesh.getNumIndexedVertices();
//	Vector3* vertexPositions = mesh.getIndexedPositions();
//	Vector2* vertexTextureCoords = mesh.getIndexedTextureCoords();
//	Vector3* vertexNormals = mesh.getIndexedNormals();
//
//	//printf("pos vbo: %d\n", positions_vbo);
//	//glGenBuffers(1, &positions_vbo);
//	//glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);
//	//glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), vertexPositions, GL_STATIC_DRAW);
//	//printf("pos vbo: %d\n", positions_vbo);
//
//	glGenBuffers(1, &textureCoords_vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, textureCoords_vbo);
//	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2), vertexTextureCoords, GL_STATIC_DRAW);
//
//	glGenBuffers(1, &normals_vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
//	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), vertexNormals, GL_STATIC_DRAW);
//
//	unsigned int* indexData = mesh.getTriangleIndices();
//	int numTriangles = mesh.getNumTriangles();
//
//	glGenBuffers(1, &indexBuffer);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numTriangles * 3, indexData, GL_STATIC_DRAW);
//}