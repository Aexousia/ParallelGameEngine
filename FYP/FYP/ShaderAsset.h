#pragma once
#include "AssetLoader.h"
#include "../dependancies/sdl/SDL_opengl.h"
#include <iostream>
#include <fstream>
#include "IOHelperFunctions.h"
#include "Shader.h"

class ShaderAsset : public Asset
{
public:
	ShaderAsset(const char* key, const char* vertexPath, const char* fragmentPath)
	:	Asset(key)
	,	m_vertexPath(vertexPath)
	,	m_fragmentPath(fragmentPath)
	{

	}

	virtual bool load() override
	{
		assert(fileExists(m_vertexPath));
		assert(fileExists(m_fragmentPath));

		//Generate program
		GLint programId = glCreateProgram();

		//Create vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		std::string src = readFile(m_vertexPath);
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
		glAttachShader(programId, vertexShader);


		//Create fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		src = readFile(m_fragmentPath);
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
			glAttachShader(programId, fragmentShader);


			//Link program
			glLinkProgram(programId);

			//Check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", programId);
				printProgramLog(programId);
				throw std::exception("program linker errors");
			}
		}

		SINGLETON(AssetLoader)->addAssetToMap<Shader>(getKey(), new Shader(programId));
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

private:
	const char * m_vertexPath;
	const char* m_fragmentPath;
};
