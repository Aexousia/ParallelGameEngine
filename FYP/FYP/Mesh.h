//http://www.nexcius.net/2014/04/13/loading-meshes-using-assimp-in-opengl/ instanced rendering is mine though
#pragma once
#include <../dependancies/GL/glew.h>
#include <../dependancies/sdl/SDL_opengl.h>
#include <../dependancies/glm/mat4x4.hpp>
#include <../dependancies/assimp/scene.h>
#include <../dependancies/assimp/mesh.h>

#include <vector>

struct MeshEntry {
	static enum BUFFERS {
		VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER, MVP_MAT_VB, MODELVIEW_MAT_VB, NORMAL_MAT_VB
	};
	GLuint vao;
	GLuint vbo[7];

	unsigned int elementCount;

	MeshEntry(aiMesh *mesh);
	~MeshEntry();

	void load(aiMesh *mesh);
	void render();
	void renderInstanced(std::vector<glm::mat4> mvps, std::vector<glm::mat4> modelViews, std::vector<glm::mat4> normalMats);
};

class Mesh
{
public:
	std::vector<MeshEntry*> meshEntries;

public:
	Mesh(const char *filename);
	~Mesh(void);

	void render();
	std::vector<MeshEntry*>& getMeshEntries();
};

