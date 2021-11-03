/*
Student Information
Student ID: 1155127347
Student Name: Lau Long Ching
*/

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"

#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Texture.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>


// screen setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

// struct for storing the obj file
struct Vertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct Model {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

Model loadOBJ(const char* objPath)
{
	// function to load the obj file
	// Note: this simple function cannot load all obj files.

	struct V {
		// struct for identify if a vertex has showed up
		unsigned int index_position, index_uv, index_normal;
		bool operator == (const V& v) const {
			return index_position == v.index_position && index_uv == v.index_uv && index_normal == v.index_normal;
		}
		bool operator < (const V& v) const {
			return (index_position < v.index_position) ||
				(index_position == v.index_position && index_uv < v.index_uv) ||
				(index_position == v.index_position && index_uv == v.index_uv && index_normal < v.index_normal);
		}
	};

	std::vector<glm::vec3> temp_positions;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::map<V, unsigned int> temp_vertices;

	Model model;
	unsigned int num_vertices = 0;

	std::cout << "\nLoading OBJ file " << objPath << "..." << std::endl;

	std::ifstream file;
	file.open(objPath);

	// Check for Error
	if (file.fail()) {
		std::cerr << "Impossible to open the file! Do you use the right path? See Tutorial 6 for details" << std::endl;
		exit(1);
	}

	while (!file.eof()) {
		// process the object file
		char lineHeader[128];
		file >> lineHeader;

		if (strcmp(lineHeader, "v") == 0) {
			// geometric vertices
			glm::vec3 position;
			file >> position.x >> position.y >> position.z;
			temp_positions.push_back(position);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			// texture coordinates
			glm::vec2 uv;
			file >> uv.x >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			// vertex normals
			glm::vec3 normal;
			file >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			// Face elements
			V vertices[3];
			for (int i = 0; i < 3; i++) {
				char ch;
				file >> vertices[i].index_position >> ch >> vertices[i].index_uv >> ch >> vertices[i].index_normal;
			}

			// Check if there are more than three vertices in one face.
			std::string redundency;
			std::getline(file, redundency);
			if (redundency.length() >= 5) {
				std::cerr << "There may exist some errors while load the obj file. Error content: [" << redundency << " ]" << std::endl;
				std::cerr << "Please note that we only support the faces drawing with triangles. There are more than three vertices in one face." << std::endl;
				std::cerr << "Your obj file can't be read properly by our simple parser :-( Try exporting with other options." << std::endl;
				exit(1);
			}

			for (int i = 0; i < 3; i++) {
				if (temp_vertices.find(vertices[i]) == temp_vertices.end()) {
					// the vertex never shows before
					Vertex vertex;
					vertex.position = temp_positions[vertices[i].index_position - 1];
					vertex.uv = temp_uvs[vertices[i].index_uv - 1];
					vertex.normal = temp_normals[vertices[i].index_normal - 1];

					model.vertices.push_back(vertex);
					model.indices.push_back(num_vertices);
					temp_vertices[vertices[i]] = num_vertices;
					num_vertices += 1;
				}
				else {
					// reuse the existing vertex
					unsigned int index = temp_vertices[vertices[i]];
					model.indices.push_back(index);
				}
			} // for
		} // else if
		else {
			// it's not a vertex, texture coordinate, normal or face
			char stupidBuffer[1024];
			file.getline(stupidBuffer, 1024);
		}
	}
	file.close();

	std::cout << "There are " << num_vertices << " vertices in the obj file.\n" << std::endl;
	return model;
}

void get_OpenGL_info()
{
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

Model Penguin_obj;
GLuint Penguin_vao, Penguin_vbo, Penguin_ebo;
Texture Penguin_texture[2];
Model Snow_obj;
GLuint Snow_vao, Snow_vbo, Snow_ebo;
Texture Snow_texture[2];
Model Penguin2_obj;
GLuint Penguin2_vao, Penguin2_vbo, Penguin2_ebo;
Texture Penguin2_texture;
Shader shader;

int PenguinTexture = 1;
int SnowTexture = 1;
int PenguinAlive = 1;
float PenguinMoveDirection;
float PenguinRotateDirection;

void Penguin_bind()
{
	Penguin_texture[0].setupTexture("resources/penguin/penguin_01.png");
	Penguin_texture[1].setupTexture("resources/penguin/penguin_02.png");
	Penguin_obj = loadOBJ("resources/penguin/penguin.obj");

	glGenVertexArrays(1, &Penguin_vao);
	glBindVertexArray(Penguin_vao);

	glGenBuffers(1, &Penguin_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Penguin_vbo);
	glBufferData(GL_ARRAY_BUFFER, Penguin_obj.vertices.size() * sizeof(Vertex), &Penguin_obj.vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &Penguin_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Penguin_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Penguin_obj.indices.size() * sizeof(unsigned int), &Penguin_obj.indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
}

void Snow_bind()
{
	Snow_texture[0].setupTexture("resources/snow/snow_01.jpg");
	Snow_texture[1].setupTexture("resources/snow/snow_02.jpg");
	Snow_obj = loadOBJ("resources/snow/snow.obj");

	glGenVertexArrays(1, &Snow_vao);
	glBindVertexArray(Snow_vao);

	glGenBuffers(1, &Snow_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Snow_vbo);
	glBufferData(GL_ARRAY_BUFFER, Snow_obj.vertices.size() * sizeof(Vertex), &Snow_obj.vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &Snow_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Snow_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Snow_obj.indices.size() * sizeof(unsigned int), &Snow_obj.indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
}

void Penguin2_bind()
{
	Penguin2_texture.setupTexture("resources/penguin/penguin2.jpg");
	Penguin2_obj = loadOBJ("resources/penguin/penguin.obj");

	glGenVertexArrays(1, &Penguin2_vao);
	glBindVertexArray(Penguin2_vao);

	glGenBuffers(1, &Penguin2_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Penguin2_vbo);
	glBufferData(GL_ARRAY_BUFFER, Penguin2_obj.vertices.size() * sizeof(Vertex), &Penguin2_obj.vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &Penguin2_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Penguin2_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Penguin2_obj.indices.size() * sizeof(unsigned int), &Penguin2_obj.indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
}


void sendDataToOpenGL()
{
	Penguin_bind();
	Snow_bind();
	Penguin2_bind();
}

void initializedGL(void) //run only once
{
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not OK." << std::endl;
	}

	get_OpenGL_info();
	sendDataToOpenGL();

	shader.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	shader.use();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}



glm::vec3 camera_pos;
glm::vec3 camera_foc(0.0f);
float cameraMoveX;
float cameraMoveY;
float zoom = 45.0f;
float diffuseBrightness = 0.0f;
double x_position, y_position;
double mouseDeltaX, mouseDeltaY;
float Yradian;


bool runningLight = true;
float c_time;
float timecolor;
bool increaseState = true;


void paintGL(void)  //always run
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	GLint projectionMatrixUniformLocation;
	GLint cameraMatrixUniformLocation;
	glm::mat4 cameraMatrix;
	glm::mat4 projectionMatrix;

	camera_pos = glm::vec3(0.0f, 10.0f, -10.0f);
	cameraMatrix = glm::lookAt(camera_pos, camera_foc, glm::vec3(0.0f, 0.0f, 1.0f));
	cameraMatrix = glm::translate(cameraMatrix, glm::vec3(0.0f, cameraMoveY, -cameraMoveX + 2.0f));
	cameraMatrix = glm::rotate(cameraMatrix, glm::radians(Yradian), glm::vec3(0, 1, 0));

	cameraMatrixUniformLocation = glGetUniformLocation(shader.Return_ID(), "cameraMatrix");
	glUniformMatrix4fv(cameraMatrixUniformLocation, 1, GL_FALSE, &cameraMatrix[0][0]);

	projectionMatrix = glm::perspective(glm::radians(45.0f + zoom), 1.3f, 0.5f, 100.0f);
	projectionMatrixUniformLocation = glGetUniformLocation(shader.Return_ID(), "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	glLineWidth(2.0f);

	GLint ambientLightingUniformLocation = glGetUniformLocation(shader.Return_ID(), "ambientLight");
	float ambientLight = 0.5f;
	glUniform1f(ambientLightingUniformLocation, ambientLight);

	//diffuse light

	float delta_time = 0.1f;
	if (runningLight) {
		if (increaseState == true)
			c_time += delta_time;
		else
			c_time -= delta_time;
		timecolor = c_time * 0.1f;
		if (c_time * 0.1f > 4.0f)
			increaseState = false;
		if (c_time * 0.1f < 0.0f)
			increaseState = true;
	}
	float speed = 0.001f;
	glm::vec3 lightPosition1, lightPosition2, lightPosition3;
	GLint directionalLightBrightness = glGetUniformLocation(shader.Return_ID(), "directionalLightBrightness");
	glUniform1f(directionalLightBrightness, diffuseBrightness);

	glm::vec3 color1(1.0f + timecolor, 1.0f + 0.1f * timecolor, 1.0f + 0.2f * timecolor);
	GLint lightColor1 = glGetUniformLocation(shader.Return_ID(), "diffuseLightColor1");
	glUniform3fv(lightColor1, 1, &color1[0]);

	GLint lightPositionUniformLocation1 = glGetUniformLocation(shader.Return_ID(), "lightPositionWorld1");
	lightPosition1 = glm::vec3(20.0f * sin(c_time * speed), 20.0f, 20.0f * cos(c_time * speed));
	glUniform3fv(lightPositionUniformLocation1, 1, &lightPosition1[0]);

	glm::vec3 color2(1.0f + timecolor, 1.0f + 0.1f * timecolor, 1.0f + 0.2f * timecolor);
	GLint lightColor2 = glGetUniformLocation(shader.Return_ID(), "diffuseLightColor2");
	glUniform3fv(lightColor2, 1, &color2[0]);

	GLint lightPositionUniformLocation2 = glGetUniformLocation(shader.Return_ID(), "lightPositionWorld2");
	lightPosition2 = glm::vec3(20.0f * sin(c_time * speed + glm::radians(120.0f)), 20.0f, 20.0f * cos(c_time * speed + glm::radians(120.0f)));
	glUniform3fv(lightPositionUniformLocation2, 1, &lightPosition2[0]);

	glm::vec3 color3(1.0f + timecolor, 1.0f + 0.1f * timecolor, 1.0f + 0.2f * timecolor);
	GLint lightColor3 = glGetUniformLocation(shader.Return_ID(), "diffuseLightColor3");
	glUniform3fv(lightColor3, 1, &color3[0]);

	GLint lightPositionUniformLocation3 = glGetUniformLocation(shader.Return_ID(), "lightPositionWorld3");
	lightPosition3 = glm::vec3(20.0f * sin(c_time * speed + glm::radians(240.0f)), 20.0f, 20.0f * cos(c_time * speed + glm::radians(240.0f)));
	glUniform3fv(lightPositionUniformLocation3, 1, &lightPosition3[0]);

	//eye pos

	GLint eyePositionUniformLocation = glGetUniformLocation(shader.Return_ID(), "eyePositionWorld");
	glUniform3fv(eyePositionUniformLocation, 1, &camera_pos[0]);

	//penguin

	GLint modelTransformMatrixUniformLocation;
	glm::mat4 modelTransformMatrix;
	GLuint TextureID;

	TextureID = glGetUniformLocation(shader.Return_ID(), "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	if (PenguinTexture == 1) {
		Penguin_texture[0].bind(0);
	}
	else {
		Penguin_texture[1].bind(0);
	}
	glUniform1i(TextureID, 0);

	glBindVertexArray(Penguin_vao);
	modelTransformMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f));

	modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(0.f, -0.1f, 0.f + PenguinMoveDirection - 1.f));
	modelTransformMatrix = glm::rotate(modelTransformMatrix, glm::radians(PenguinRotateDirection), glm::vec3(0.0f, 1.0f, 0.0f));
	if (PenguinAlive == -1) modelTransformMatrix = glm::rotate(modelTransformMatrix, glm::radians(90.f), glm::vec3(0.0f, .0f, 1.0f));

	modelTransformMatrixUniformLocation = glGetUniformLocation(shader.Return_ID(), "modelTransformMatrix");

	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, Penguin_obj.indices.size(), GL_UNSIGNED_INT, 0);

	modelTransformMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);

	// snow

	TextureID = glGetUniformLocation(shader.Return_ID(), "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	if (SnowTexture == 1) {
		Snow_texture[0].bind(0);
	}
	else {
		Snow_texture[1].bind(0);
	}
	glUniform1i(TextureID, 0);

	glBindVertexArray(Snow_vao);
	modelTransformMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	modelTransformMatrix = glm::scale(modelTransformMatrix, glm::vec3(5.0f, 1.0f, 5.0f));
	modelTransformMatrixUniformLocation = glGetUniformLocation(shader.Return_ID(), "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, Snow_obj.indices.size(), GL_UNSIGNED_INT, 0);

	// penguin2
	
	TextureID = glGetUniformLocation(shader.Return_ID(), "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	Penguin2_texture.bind(0);
	glUniform1i(TextureID, 0);

	glBindVertexArray(Penguin2_vao);
	modelTransformMatrix = glm::scale(modelTransformMatrix, glm::vec3(5.0f, 1.0f, 5.0f));
	modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(0.f, +1.f, +1.f));
	modelTransformMatrixUniformLocation = glGetUniformLocation(shader.Return_ID(), "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, Penguin2_obj.indices.size(), GL_UNSIGNED_INT, 0);



	glFlush();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

float move = 3.0f;
bool drag = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Sets the mouse-button callback for the current window.
	glfwGetCursorPos(window, &x_position, &y_position);
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		drag = true;
	}
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
	{
		drag = false;
	}
}

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
	// Sets the cursor position callback for the current window
	if (drag == true)
	{
		glfwGetCursorPos(window, &x, &y);
		mouseDeltaX = (x - x_position) * 0.1f;

		mouseDeltaY = (y - y_position) * 0.1f;
		x_position = x;
		y_position = y;
		Yradian += mouseDeltaX;
		cameraMoveY += mouseDeltaY;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Sets the scoll callback for the current window.
	if (yoffset > 0)
		cameraMoveX -= 0.3f;
	else
		cameraMoveX += 0.3f;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Sets the Keyboard callback for the current window.
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		PenguinTexture = 1;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		PenguinTexture = 2;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		SnowTexture = 1;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		SnowTexture = 2;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		diffuseBrightness = diffuseBrightness + 0.1f;
		if (diffuseBrightness > 1)
			diffuseBrightness = 1;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		PenguinAlive = -PenguinAlive;
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		diffuseBrightness = diffuseBrightness - 0.1f;
		if (diffuseBrightness < 0)
			diffuseBrightness = 0;
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		diffuseBrightness = diffuseBrightness + 0.1f;
		if (diffuseBrightness > 1)
			diffuseBrightness = 1;
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		diffuseBrightness = diffuseBrightness + 0.1f;
		if (diffuseBrightness > 1)
			diffuseBrightness = 1;
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		PenguinMoveDirection += 0.1f;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		PenguinMoveDirection -= 0.1f;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		PenguinRotateDirection = PenguinRotateDirection + 2.0f;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		PenguinRotateDirection = PenguinRotateDirection - 2.0f;
	}
}


int main(int argc, char* argv[])
{
	GLFWwindow* window;

	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment 2", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*register callback functions*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);                                                                     
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	initializedGL();

	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}






