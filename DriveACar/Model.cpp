#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ShaderProgram.h"
//#include "ObjMesh.h"

#include "Model.h"
//#include "ObjMesh.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tinyobj/tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//#include "main.cpp"
//float scaleFactor = 0;

//Model::Model()
//{
//}

std::map<std::string, GLuint> Mesh::loadedTextures;


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

	// Append `default` material
	materials.push_back(tinyobj::material_t());

	for (size_t i = 0; i < materials.size(); i++) {
		//printf("material[%d].diffuse_texname = %s\n", int(i),
			//materials[i].diffuse_texname.c_str());
		tinyobj::material_t* mp = &materials[i];

		if (mp->diffuse_texname.length() > 0) {
			// Only load the texture if it is not already loaded
			if (Mesh::loadedTextures.find(mp->diffuse_texname) == Mesh::loadedTextures.end()) {
				GLuint texture_id;
				int w, h;
				int comp;

				std::string texture_filename = mp->diffuse_texname;
				texture_filename = "meshes/" + mp->diffuse_texname;

				unsigned char* image =
					stbi_load(texture_filename.c_str(), &w, &h, &comp, STBI_default);
				if (!image) {
					std::cerr << "Unable to load texture: " << texture_filename
						<< std::endl;
					exit(1);
				}
				//std::cout << "Loaded texture: " << texture_filename << ", w = " << w
				//	<< ", h = " << h << ", comp = " << comp << std::endl;

				glGenTextures(1, &texture_id);

				// make the texture active
				glBindTexture(GL_TEXTURE_2D, texture_id);

				// make a texture mip map
				glGenerateTextureMipmap(texture_id);
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

				// specify the functions to use when shrinking/enlarging the texture image
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

				// specify the tiling parameters
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				// send the data to OpenGL
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

				// bind the texture to unit 0
				glBindTexture(GL_TEXTURE_2D, texture_id);
				glActiveTexture(GL_TEXTURE0);

				stbi_image_free(image);
				Mesh::loadedTextures.insert(std::make_pair(mp->diffuse_texname, texture_id));
			}
		}
	}

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


				tinyobj::real_t tx = 0;
				tinyobj::real_t ty = 0;
				if (idx.texcoord_index>-1)
				{
					tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				}

				vertexBuffer.emplace_back(Vector3{ vx, vy, vz });
				normalBuffer.emplace_back(Vector3{ nx, ny, nz });
				textureCoordBuffer.emplace_back(Vector2{ tx, ty });
				//textureCoordBuffer.emplace_back(Vector2{ 0.5,0.5 });

				indexBuffer.emplace_back(index_offset + v);
			}
			index_offset += fv;

			// per-face material
			//shapes[shapeId].mesh.material_ids[f];
		}

		newMesh->LoadGeometry(vertexBuffer, normalBuffer, textureCoordBuffer, indexBuffer);
		//newMesh->LoadTexture();

		newMesh->shaderProgram = shaderProgram;
		
		int matid = shapes[shapeId].mesh.material_ids[0];
		if (matid>-1)
		{
			newMesh->ambientColour = Vector3{ materials[matid].ambient[0],materials[matid].ambient[1],materials[matid].ambient[2] };
			newMesh->diffuseColour = Vector3{ materials[matid].diffuse[0],materials[matid].diffuse[1],materials[matid].diffuse[2] };
			newMesh->specularColour = Vector3{ materials[matid].specular[0],materials[matid].specular[1],materials[matid].specular[2] };
			newMesh->shininess = materials[matid].shininess;

			if (materials[matid].diffuse_texname.length()>0)
			{
				newMesh->textureId = Mesh::loadedTextures[materials[matid].diffuse_texname];
			}
		} else
		{
			//newMesh->ambientColour = Vector3{ 1.0f,1.0f,1.0f };
			//newMesh->specularColour = Vector3{ 1.0f,1.0f,1.0f };
			//newMesh->diffuseColour = Vector3{ 112.0f / 255.0f, 78.0f / 255.0f, 53.0f / 255.0f };

			matid = materials.size() - 1;
			newMesh->ambientColour = Vector3{ materials[matid].ambient[0],materials[matid].ambient[1],materials[matid].ambient[2] };
			newMesh->diffuseColour = Vector3{ materials[matid].diffuse[0],materials[matid].diffuse[1],materials[matid].diffuse[2] };
			//newMesh->textureId = Mesh::loadedTextures[materials[matid].diffuse_texname];
		}

		modelMeshes.push_back(newMesh);
	}
}

void Model::Update(double time, double dtime)
{
	////glm::vec3 scaleVector = glm::vec3(1.0f, 1.0f, 1.0f) * scaleFactor;
	modelm = glm::mat4(1.0f);
	////model = glm::rotate(model, glm::radians(0.01f*(float)c), glm::vec3(0, 1, 0)); // rotate about the y-axis
	modelm = glm::translate(modelm, position);
	modelm = glm::scale(modelm, scale);
	modelm = glm::rotate(modelm, glm::radians(angle.x), glm::vec3(1, 0, 0)); // rotate about the x-axis
	modelm = glm::rotate(modelm, glm::radians(angle.y), glm::vec3(0, 1, 0)); // rotate about the y-axis
	modelm = glm::rotate(modelm, glm::radians(angle.z), glm::vec3(0, 0, 1)); // rotate about the z-axis
}

void Model::Render(const glm::mat4& view, const glm::mat4& projection, const RenderData& data )
{
	for (auto const& mesh : modelMeshes)
	{
		mesh->Render(modelm,view, projection,data);
	}
}

Mesh::Mesh()
{

}
Mesh::~Mesh()
{

}

//float Model::scaleFactor = 2.0f;

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

	//printf("  LOADED SHAPE: VC: %d\n", VertexCount);

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

void Mesh::LoadTexture()
{
	int imageWidth, imageHeight;
	int numComponents;

	// load the image data into a bitmap
	unsigned char *bitmap = stbi_load("textures/brickwall.jpg",
		&imageWidth,
		&imageHeight,
		&numComponents, 4);

	// generate a texture name
	glGenTextures(1, &textureId);

	// make the texture active
	glBindTexture(GL_TEXTURE_2D, textureId);

	// make a texture mip map
	glGenerateTextureMipmap(textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	// specify the functions to use when shrinking/enlarging the texture image
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	// specify the tiling parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// send the data to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageWidth, imageHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);

	// bind the texture to unit 0
	glBindTexture(GL_TEXTURE_2D, textureId);
	glActiveTexture(GL_TEXTURE0);

	// free the bitmap data
	stbi_image_free(bitmap);
}

void Mesh::Render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const RenderData& data)
{
	// activate our Meshes shader program
	glUseProgram(shaderProgram);

	// model matrix
	//glm::mat4 mv = view * model;
	//GLuint mMatrixId = glGetUniformLocation(shaderProgram, "u_MMatrix");
	//glUniformMatrix4fv(mMatrixId, 1, GL_FALSE, &model[0][0]);

	// model-view-projection matrix
	glm::mat4 mvp = projection * view * model;
	GLuint mvpMatrixId = glGetUniformLocation(shaderProgram, "u_MVPMatrix");
	glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &mvp[0][0]);

	// model-view matrix
	glm::mat4 mv = view * model;
	GLuint mvMatrixId = glGetUniformLocation(shaderProgram, "u_MVMatrix");
	glUniformMatrix4fv(mvMatrixId, 1, GL_FALSE, &mv[0][0]);

	// the position of our camera/eye
	glm::vec4 viewEye =  glm::vec4(data.eyePosition, 1);
	//glm::vec4 viewEye = glm::vec4(data.eyePosition,1);
	GLuint eyePosId = glGetUniformLocation(shaderProgram, "u_EyePosition");
	glUniform3f(eyePosId, viewEye.x,viewEye.y,viewEye.z);

	//glm::vec4 lightPos = model * glm::vec4(data.lightPosition, 1);
	//glm::vec3 lightPos = model * glm::vec4((data.lightPosition - data.eyePosition), 1);//glm::vec3(model*glm::vec4(0, 0, 0, 1));
	//glm::vec3 lightPos = view * glm::vec4((data.lightPosition - data.eyePosition), 1);//glm::vec3(model*glm::vec4(0, 0, 0, 1));
	//glm::vec3 lightPos = mv*glm::vec4((data.lightPosition), 1);//glm::vec3(model*glm::vec4(0, 0, 0, 1));
	glm::vec3 lightPos = data.lightPosition;
	// the position of our light
	GLuint lightPosId = glGetUniformLocation(shaderProgram, "u_LightPos");
	glUniform3f(lightPosId, lightPos.x, lightPos.y, lightPos.z);

	// the colour of our object
	GLuint diffuseColourId = glGetUniformLocation(shaderProgram, "u_DiffuseColour");
	glUniform4f(diffuseColourId, diffuseColour.x,diffuseColour.y,diffuseColour.z, 1.0);
	
	GLuint ambientColourId = glGetUniformLocation(shaderProgram, "u_AmbientColour");
	glUniform4f(ambientColourId, ambientColour.x,ambientColour.y,ambientColour.z, 1.0);
	
	GLuint specularColourId = glGetUniformLocation(shaderProgram, "u_SpecularColour");
	glUniform4f(specularColourId, specularColour.x,specularColour.y,specularColour.z, 1.0);

	// the shininess of the object's surface
	GLuint shininessId = glGetUniformLocation(shaderProgram, "u_Shininess");
	glUniform1f(shininessId, shininess);

	GLuint hasTextureId = glGetUniformLocation(shaderProgram, "u_hasTexture");
	glUniform1i(hasTextureId, textureId );

	GLuint textureUniformId = glGetUniformLocation(shaderProgram, "textureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glUniform1i(textureUniformId, 0);

	// find the names (ids) of each vertex attribute
	GLint positionAttribId = glGetAttribLocation(shaderProgram, "position");
	GLint textureCoordsAttribId = glGetAttribLocation(shaderProgram, "textureCoords");
	GLint normalAttribId = glGetAttribLocation(shaderProgram, "normal");

	//if (positionAttribId>-1)
	//{
		// provide the vertex positions to the shaders
		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
		glEnableVertexAttribArray(positionAttribId);
		glVertexAttribPointer(positionAttribId, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	//}

	if (textureCoordsAttribId>-1)
	{
		//// provide the vertex texture coordinates to the shaders
		glBindBuffer(GL_ARRAY_BUFFER, textureCoordsVBO);
		glEnableVertexAttribArray(textureCoordsAttribId);
		glVertexAttribPointer(textureCoordsAttribId, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	}

	if (normalAttribId>-1)
	{
		//// provide the vertex normals to the shaders
		glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
		glEnableVertexAttribArray(normalAttribId);
		glVertexAttribPointer(normalAttribId, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	}


	// draw the triangles
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, VertexCount, GL_UNSIGNED_INT, (void*)0);


	// disable the attribute arrays
	glDisableVertexAttribArray(positionAttribId);

	if (textureCoordsAttribId > -1)
	{
		glDisableVertexAttribArray(textureCoordsAttribId);
	}

	if (normalAttribId > -1)
	{
		glDisableVertexAttribArray(normalAttribId);
	}

}