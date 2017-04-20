//http://www.nexcius.net/2014/04/13/loading-meshes-using-assimp-in-opengl/ instanced rendering is mine though
#include "stdafx.h"
#include "Mesh.h"
#include <../dependancies/assimp/Importer.hpp>
#include <../dependancies/assimp/postprocess.h>

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2

#define MVP_LOCATION 4
#define MODEL_VIEW_LOCATION 8
#define NORMAL_MATRIX_LOCATION 12

/**
*	Constructor, loading the specified aiMesh
**/
MeshEntry::MeshEntry(aiMesh *mesh) {
	vbo[VERTEX_BUFFER] = NULL;
	vbo[TEXCOORD_BUFFER] = NULL;
	vbo[NORMAL_BUFFER] = NULL;
	vbo[INDEX_BUFFER] = NULL;
	vbo[MODELVIEW_MAT_VB] = NULL;
	vbo[MVP_MAT_VB] = NULL;
	vbo[NORMAL_MAT_VB] = NULL;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	elementCount = mesh->mNumFaces * 3;

	if (mesh->HasPositions()) {
		float *vertices = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			vertices[i * 3] = mesh->mVertices[i].x;
			vertices[i * 3 + 1] = mesh->mVertices[i].y;
			vertices[i * 3 + 2] = mesh->mVertices[i].z;
		}

		glGenBuffers(1, &vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		delete vertices;
	}


	if (mesh->HasTextureCoords(0)) {
		float *texCoords = new float[mesh->mNumVertices * 2];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
			texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}

		glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		delete texCoords;
	}


	if (mesh->HasNormals()) {
		float *normals = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;
		}

		glGenBuffers(1, &vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		delete normals;
	}


	if (mesh->HasFaces()) {
		unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
		for (int i = 0; i < mesh->mNumFaces; ++i) {
			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		glGenBuffers(1, &vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		delete indices;
	}


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
/*
	glGenBuffers(1, &vbo[MVP_LOCATION]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[MVP_LOCATION]);
	for (unsigned int i = 0; i < 4; i++) {
		glEnableVertexAttribArray(MVP_LOCATION + i);
		glVertexAttribPointer(MVP_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
		glVertexAttribDivisor(MVP_LOCATION + i, 1);
	}

	glGenBuffers(1, &vbo[MODELVIEW_MAT_VB]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[MODEL_VIEW_LOCATION]);
	for (unsigned int i = 0; i < 4; i++) {
		glEnableVertexAttribArray(MODEL_VIEW_LOCATION + i);
		glVertexAttribPointer(MODEL_VIEW_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
		glVertexAttribDivisor(MODEL_VIEW_LOCATION + i, 1);
	}

	glGenBuffers(1, &vbo[NORMAL_MAT_VB]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_MATRIX_LOCATION]);
	for (unsigned int i = 0; i < 4; i++) {
		glEnableVertexAttribArray(NORMAL_MATRIX_LOCATION + i);
		glVertexAttribPointer(NORMAL_MATRIX_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
		glVertexAttribDivisor(NORMAL_MATRIX_LOCATION + i, 1);
	}*/
	
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

/**
*	Deletes the allocated OpenGL buffers
**/
MeshEntry::~MeshEntry() {
	if (vbo[VERTEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[VERTEX_BUFFER]);
	}

	if (vbo[TEXCOORD_BUFFER]) {
		glDeleteBuffers(1, &vbo[TEXCOORD_BUFFER]);
	}

	if (vbo[NORMAL_BUFFER]) {
		glDeleteBuffers(1, &vbo[NORMAL_BUFFER]);
	}

	if (vbo[INDEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[INDEX_BUFFER]);
	}

	glDeleteVertexArrays(1, &vao);
}

/**
*	Renders this MeshEntry
**/
void MeshEntry::render() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void MeshEntry::renderInstanced(std::vector<glm::mat4> mvps, std::vector<glm::mat4> modelViews, std::vector<glm::mat4> normalMats)
{
	glBindVertexArray(vao);

	/*glBindBuffer(GL_ARRAY_BUFFER, vbo[MVP_MAT_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * mvps.size(), &mvps[0][0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[MODELVIEW_MAT_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * mvps.size(), &modelViews[0][0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_MAT_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * mvps.size(), &normalMats[0][0], GL_DYNAMIC_DRAW);
	*/
	//render();
	//int size;
	//glGetBufferParameteriv(GL_UNSIGNED_SHORT, GL_BUFFER_SIZE, &size);
	glDrawElementsInstanced(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0, mvps.size());

	//// Make sure the VAO is not changed from the outside    
	glBindVertexArray(0);
}

/**
*	Mesh constructor, loads the specified filename if supported by Assimp
**/
Mesh::Mesh(const char *filename) 
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename, NULL);
	if (!scene) {
		printf("Unable to laod mesh: %s\n", importer.GetErrorString());
	}

	for (int i = 0; i < scene->mNumMeshes; ++i) {
		meshEntries.push_back(new MeshEntry(scene->mMeshes[i]));
	}
}

/**
*	Clears all loaded MeshEntries
**/
Mesh::~Mesh(void)
{
	for (int i = 0; i < meshEntries.size(); ++i) {
		delete meshEntries.at(i);
	}
	meshEntries.clear();
}

/**
*	Renders all loaded MeshEntries
**/
void Mesh::render() {
	for (int i = 0; i < meshEntries.size(); ++i) {
		meshEntries.at(i)->render();
	}
}

std::vector<MeshEntry*>& Mesh::getMeshEntries() {
	return meshEntries;
}