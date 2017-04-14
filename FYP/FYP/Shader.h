#pragma once
#include "Singleton.h"
#include <iostream>
#include "../dependancies/sdl/SDL_opengl.h"
#include <fstream>

class Shader
{
public:
	bool loadFromFile(const char* vertexPath, const char* fragmentPath)
	{
		//Generate program
		m_programId = glCreateProgram();

		//Create vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		std::string src = readFile(vertexPath);
		const GLchar* vertexSource[] = { src.c_str() };
		const GLint size = src.size();

		//Set vertex source
		glShaderSource(vertexShader, 1, vertexSource, NULL);

		//Compile vertex source
		glCompileShader(vertexShader);

		//Check vertex shader for errors
		GLint vShaderCompiled = GL_FALSE;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
		if (vShaderCompiled != GL_TRUE)
		{
			printf("Unable to compile vertex shader %d!\n", vertexShader);
			printShaderLog(vertexShader);
			throw std::exception("vertex shader error");
		}

		//Attach vertex shader to program
		glAttachShader(m_programId, vertexShader);


		//Create fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		src = readFile(fragmentPath);
		const GLint size2 = src.size();

		//Get fragment source
		const GLchar* fragmentSource[] =
		{
			src.c_str()
		};

		//Set fragment source
		glShaderSource(fragmentShader, 1, fragmentSource, &size2);

		//Compile fragment source
		glCompileShader(fragmentShader);

		//Check fragment shader for errors
		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			printf("Unable to compile fragment shader %d!\n", fragmentShader);
			printShaderLog(fragmentShader);
			throw std::exception("fragment shader error");
		}
		else
		{
			//Attach fragment shader to program
			glAttachShader(m_programId, fragmentShader);


			//Link program
			glLinkProgram(m_programId);

			//Check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv(m_programId, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", m_programId);
				printProgramLog(m_programId);
				throw std::exception("program linker errors");
			}
		}
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

	static void printProgramLog(GLuint program)
	{
		//Make sure name is shader
		if (glIsProgram(program))
		{
			//Program log length
			int infoLogLength = 0;
			int maxLength = infoLogLength;

			//Get info string length
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			//Allocate string
			char* infoLog = new char[maxLength];

			//Get info log
			glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
			if (infoLogLength > 0)
			{
				//Print Log
				printf("%s\n", infoLog);
			}

			//Deallocate string
			delete[] infoLog;
		}
		else
		{
			printf("Name %d is not a program\n", program);
		}
	}

	static void printShaderLog(GLuint shader)
	{
		//Make sure name is shader
		if (glIsShader(shader))
		{
			//Shader log length
			int infoLogLength = 0;
			int maxLength = infoLogLength;

			//Get info string length
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			//Allocate string
			char* infoLog = new char[maxLength];

			//Get info log
			glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
			if (infoLogLength > 0)
			{
				//Print Log
				printf("%s\n", infoLog);
			}

			//Deallocate string
			delete[] infoLog;
		}
		else
		{
			printf("Name %d is not a shader\n", shader);
		}
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
	std::string readFile(const char* filePath)
	{
		std::ifstream shaderFile(filePath);

		// find the file size
		shaderFile.seekg(0, std::ios::end);
		std::streampos          length = shaderFile.tellg();
		shaderFile.seekg(0, std::ios::beg);

		// read whole file into a vector:
		std::vector<char>       buffer(length);
		shaderFile.read(&buffer[0], length);

		// return the shader string
		return std::string(buffer.begin(), buffer.end());
	}
	GLuint m_programId;
};

