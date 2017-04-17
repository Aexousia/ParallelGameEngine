#pragma once

struct Light
{
	glm::vec3 Position;	//Light Position in eye-coords
	glm::vec3 La;		//Ambient light intensity
	glm::vec3 Ld;		//Diffuse light intensity
	glm::vec3 Ls;		//Specular light intensity

	Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		: Position(position)
		, La(ambient)
		, Ld(diffuse)
		, Ls(specular)
	{

	}

	void BindToShader(Shader* shader, int index, std::string name)
	{
		GLint locations[4] = {
			shader->getUniformLocation((name + "[" + std::to_string(index) + "].Position").c_str()),
			shader->getUniformLocation((name + "[" + std::to_string(index) + "].La").c_str()),
			shader->getUniformLocation((name + "[" + std::to_string(index) + "].Ld").c_str()),
			shader->getUniformLocation((name + "[" + std::to_string(index) + "].Ls").c_str())
		};

		glUniform3fv(locations[0], 1, glm::value_ptr(Position));
		glUniform3fv(locations[1], 1, glm::value_ptr(La));
		glUniform3fv(locations[2], 1, glm::value_ptr(Ld));
		glUniform3fv(locations[2], 1, glm::value_ptr(Ls));
	}
};