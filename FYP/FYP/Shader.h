#pragma once

class Shader
{
public:

	Shader(GLint programId)
	:	m_programId(programId)
	{

	}
	/*template<typename T>
	void setUniform(const char* name, T value)
	{

	}

	template<typename T>
	T& getParamater(const char* name)
	{
		GLint index = glGetAttribLocation(m_programId, name);
		if (gVertexPos2DLocation == -1)
		{
			printf(name + " is not a valid glsl program variable!\n");
		}
		glUni
	}*/

	GLint getAttribLocation(const char* name)
	{
		GLint index = glGetAttribLocation(m_programId, name);
		if (index == -1)
		{
			std::string error = name;
			error += " is not a valid glsl program variable!\n";
			printf(error.c_str());
		}
		return index;
	}

	GLint getUniformLocation(const char* name)
	{
		GLint index = glGetUniformLocation(m_programId, name);
		if (index == -1)
		{
			std::string error = name;
			error += " is not a valid glsl program variable!\n";
			printf(error.c_str());
		}
		return index;
	}

	static void bind(Shader* shader)
	{
		//Bind program
		glUseProgram(shader->m_programId);
	}

	static void unbind()
	{
		//Unbind program
		glUseProgram(NULL);
	}
protected:
	
	GLuint m_programId;
};

