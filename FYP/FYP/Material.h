#pragma once

struct Material
{
	glm::vec3 Ka;			//Ambient reflectivity
	glm::vec3 Kd;			//Diffuse reflectivity
	glm::vec3 Ks;			//Specular reflectivity
	float Shininess;	//Specular shininess factor

	Material(glm::vec3 ambientReflectivity, glm::vec3 diffuseReflectivity, glm::vec3 specularReflectivity, float shininess)
		: Ka(ambientReflectivity)
		, Kd(diffuseReflectivity)
		, Ks(specularReflectivity)
		, Shininess(shininess)
	{

	}

	void BindToShader(Shader* shader, std::string name)
	{
		GLint locations[4] = {
			shader->getUniformLocation((name + ".Ka").c_str()),
			shader->getUniformLocation((name + ".Kd").c_str()),
			shader->getUniformLocation((name + ".Ks").c_str()),
			shader->getUniformLocation((name + ".Shininess").c_str())
		};

		glUniform3fv(locations[0], 1, glm::value_ptr(Ka));
		glUniform3fv(locations[1], 1, glm::value_ptr(Kd));
		glUniform3fv(locations[2], 1, glm::value_ptr(Ks));
		glUniform1fv(locations[3], 1, &Shininess);
	}
};