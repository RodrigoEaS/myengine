#include"Shader.h"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{	
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{	
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Constructor that build the Shader Program
Shader::Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile)
{	
	// Create Shader Program Object and get its reference
	ID = glCreateProgram();

	if(std::string(vertexFile) != "None"){
		// Read vertexFile and store the string
		std::string vertexCode = get_file_contents(vertexFile);

		// Convert the shader source strings into character arrays
		const char* vertexSource = vertexCode.c_str();

		// Create Vertex Shader Object and get its reference
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// Attach Vertex Shader source to the Vertex Shader Object
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		// Compile the Vertex Shader into machine code
		glCompileShader(vertexShader);
		// Checks if Shader compiled succesfully
		compileErrors(vertexShader, "VERTEX");

		// Attach the Vertex Shader to the Shader Program
		glAttachShader(ID, vertexShader);

		// Delete the now useless Vertex Shader object
		glDeleteShader(vertexShader);
	}

	if(std::string(fragmentFile) != "None"){
		// Read fragmentFile and store the string
		std::string fragmentCode = get_file_contents(fragmentFile);

		// Convert the shader source strings into character arrays
		const char* fragmentSource = fragmentCode.c_str();

		// Create Fragment Shader Object and get its reference
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		// Attach Fragment Shader source to the Fragment Shader Object
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		// Compile the Fragment Shader into machine code
		glCompileShader(fragmentShader);
		// Checks if Shader compiled succesfully
		compileErrors(fragmentShader, "FRAGMENT");

		// Attach the Fragment Shader to the Shader Program
		glAttachShader(ID, fragmentShader);

		// Delete the now useless Fragment Shader object
		glDeleteShader(fragmentShader);
	}

	if(std::string(geometryFile) != "None"){
		// Read GeometryFile and store the string
		std::string geometryCode = get_file_contents(geometryFile);

		// Convert the shader source strings into character arrays
		const char* geometrySource = geometryCode.c_str();

		// Create Geometry Shader Object and get its reference
		GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		// Attach Geometry Shader source to the Fragment Shader Object
		glShaderSource(geometryShader, 1, &geometrySource, NULL);
		// Compile the Geometry Shader into machine code
		glCompileShader(geometryShader);
		// Checks if Shader compiled succesfully
		compileErrors(geometryShader, "GEOMETRY");
		
		// Attach the Geometry Shaders to the Shader Program
		glAttachShader(ID, geometryShader);

		// Delete the now useless Geometry Shader object
		glDeleteShader(geometryShader);
	}

	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	// Checks if Shaders linked succesfully
	compileErrors(ID, "PROGRAM");
}

// Activates the Shader Program
void Shader::Activate()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}

//Set Uniform1i for the shader
void Shader::setInt(std::string name, int value)
{ 
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
//Set Uniform1f for the shader
void Shader::setFloat(std::string name, float value)
{ 
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}
//Set Uniform2f for the shader
void Shader::setVec2f(std::string name, glm::vec2 value)
{ 
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y); 
}
//Set Uniform3f for the shader
void Shader::setVec3f(std::string name, glm::vec3 value)
{ 
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z); 
}
//Set Uniform4f for the shader
void Shader::setVec4f(std::string name, glm::vec4 value)
{ 
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w); 
}

// Checks if the different Shaders have compiled properly
void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}