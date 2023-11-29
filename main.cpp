//------- Ignore this ----------
#include <filesystem>
#include <chrono>
#include <thread>
namespace fs = std::filesystem;
//------------------------------

#include"Nuclear/Object/Mesh.hpp"
#include"Nuclear/FrameBuffer/FBO.hpp"
#include"Nuclear/FrameBuffer/FBT.hpp"

const unsigned int width = 720;
const unsigned int height = 720;

// Number of samples per pixel for MSAA
unsigned int samples = 8;

// Controls the gamma function
const float gamma = 2.2f;

// Target Frame rate
const double frameRate = 1.0 / 60.0;

float rectangleVertices[] = {
	//  Coords   // texCoords
	1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,

	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 1.0f};

// Vertices for plane with texture
std::vector<Vertex> vertices = {
	Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}};

// Indices for plane with texture
std::vector<GLuint> indices = {
	0, 1, 2,
	0, 2, 3};

/// @brief 
/// @return 
int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Only use this if you don't have a framebuffer
	// glfwWindowHint(GLFW_SAMPLES, samples);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow *window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	// Generates shaders
	Shader shaderProgram("./resources/shaders/default.vert.glsl", "./resources/shaders/default.frag.glsl", "./resources/shaders/default.geom.glsl");
	Shader framebufferProgram("./resources/shaders/framebuffer.vert.glsl", "./resources/shaders/framebuffer.frag.glsl");
	Shader blurProgram("./resources/shaders/framebuffer.vert.glsl", "./resources/shaders/blur.frag.glsl");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	shaderProgram.Activate();
	shaderProgram.setVec4f("lightColor", lightColor);
	shaderProgram.setVec3f("lightPos", lightPos);

	framebufferProgram.Activate();
	framebufferProgram.setInt("screenTexture", 0);
	framebufferProgram.setInt("bloomTexture", 1);
	framebufferProgram.setFloat("gamma", gamma);

	blurProgram.Activate();
	blurProgram.setInt("screenTexture", 0);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	// Enables Multisampling
	glEnable(GL_MULTISAMPLE);
	// Enables Cull Facing
	glEnable(GL_CULL_FACE);
	// Keeps front faces
	glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	glFrontFace(GL_CCW);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Prepare framebuffer rectangle VBO and VAO
	unsigned int rectVAO;
	glGenVertexArrays(1, &rectVAO);
	glBindVertexArray(rectVAO);

	unsigned int rectVBO;
	glGenBuffers(1, &rectVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

	// Variables to create periodic event for FPS displaying
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	double FPS;
	double ms;

	// Use this to disable VSync (not advized)
	// glfwSwapInterval(0);

	// Create Frame Buffer Object
	FBO postProcessingFBO;
	// Create Framebuffer Texture
	FBT postProcessingTexture(width, height, 0);
	// Create Second Framebuffer Texture
	FBT bloomTexture(width, height, 1);

	// Tell OpenGL we need to draw to both attachments
	unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, attachments);

	// Error checking framebuffer
	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Post-Processing Framebuffer error: " << fboStatus << std::endl;

	// Create Ping Pong Framebuffers for repetitive blurring
	unsigned int pingpongFBO[2];
	glGenFramebuffers(2, pingpongFBO);

	unsigned int pingpongBuffer[2];
	glGenTextures(2, pingpongBuffer);
	
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);

		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);

		fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Ping-Pong Framebuffer error: " << fboStatus << std::endl;
	}

	// Texture for the plane
	std::vector<Texture> textures =
		{
			Texture("./resources/textures/diffuse.png", "diffuse", 0)};

	// Plane with the texture
	Mesh plane(vertices, indices, textures);
	// Normal map for the plane
	Texture normalMap("./resources/textures/normal.png", "normal", 1);
	Texture displacementMap("./resources/textures/displacement.png", "displacement", 2);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Updates counter and times
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;

		// Bind the custom framebuffer
		postProcessingFBO.Bind();
		// Specify the color of the background
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Enable depth testing since it's disabled when drawing the framebuffer rectangle
		glEnable(GL_DEPTH_TEST);

		// Handles camera inputs (delete this if you have disabled VSync)
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		shaderProgram.Activate();
		normalMap.Bind();
		shaderProgram.setInt("normal0", 1);
		displacementMap.Bind();
		shaderProgram.setInt("displacement0", 2);

		// Draw the normal model
		plane.Draw(shaderProgram, camera);

		// Bounce the image data around to blur multiple times
		bool horizontal = true, first_iteration = true;
		// Amount of time to bounce the blur
		int amount = 2;
		blurProgram.Activate();
		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			blurProgram.setInt("horizontal", horizontal);

			// In the first bounc we want to get the data from the bloomTexture
			if (first_iteration)
			{
				bloomTexture.Bind();
				first_iteration = false;
			}
			// Move the data between the pingPong textures
			else
			{
				glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
			}

			// Render the image
			glBindVertexArray(rectVAO);
			glDisable(GL_DEPTH_TEST);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// Switch between vertical and horizontal blurring
			horizontal = !horizontal;
		}

		// Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Draw the framebuffer rectangle
		framebufferProgram.Activate();
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glActiveTexture(GL_TEXTURE0);
		postProcessingTexture.Bind();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();

		// Limit FPS
		if (timeDiff >= frameRate)
		{
			// Creates new title
			FPS = 1.0 / timeDiff;
			ms = timeDiff * 1000;
			std::string newTitle = "YoutubeOpenGL - " + std::to_string(FPS) + "FPS / " + std::to_string(ms) + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			double sleepTime = frameRate / ms;

			std::this_thread::sleep_for(std::chrono::milliseconds((int)(sleepTime * 1000)));

			// Reset time
			prevTime = crntTime;

			// Use this if you have disabled VSync
			// camera.Inputs(window);
		}
	}

	// Delete all the objects we've created
	shaderProgram.Delete();
	blurProgram.Delete();
	framebufferProgram.Delete();

	// Delete the Custom Frame Buffer Object
	postProcessingFBO.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}