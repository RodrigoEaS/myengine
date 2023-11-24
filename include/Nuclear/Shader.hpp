#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor that build the Shader Program
	Shader(const char* vertexFile="None", const char* fragmentFile="None", const char* geometryFile="None");

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

	//Set Uniform1i for the shader
	void setInt(std::string name, int value);
	//Set Uniform1f for the shader
	void setFloat(std::string name, float value);
	//Set Uniform2f for the shader
	void setVec2f(std::string name, glm::vec2 value);
	//Set Uniform3f for the shader
	void setVec3f(std::string name, glm::vec3 value);
	//Set Uniform4f for the shader
	void setVec4f(std::string name, glm::vec4 value);

private:
	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, const char* type);
};

// Generates default shaders


#endif