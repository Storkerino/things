/*
 *  Created on: Jun 10, 2018
 *      Author: stork
 */

#include <iostream>
#include <stdlib.h>

#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h> //some opengl libraries


std::string LoadFileToString(const char* filepath){
	/*read file to string*/
	std::string data;
	std::ifstream stream(filepath, std::ios::in);
	if(!stream.is_open()){
		std::cerr << "File failed to load" << std::endl;
	}

	if(stream.is_open()){
		std::string line ="";
		while(getline(stream, line)){
			data += "\n" + line;
		}
		stream.close();
	}
	return data;
}

GLuint LoadShaders(const char* vertShaderPath, const char* fragShaderPath){
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertShaderSource = LoadFileToString(vertShaderPath);
	std::string fragShaderSource = LoadFileToString(fragShaderPath);

	const char* rawVertShaderSource = vertShaderSource.c_str();
	const char* rawFragShaderSource = fragShaderSource.c_str();

	glShaderSource(vertShader ,1, &rawVertShaderSource, NULL);
	glShaderSource(fragShader ,1, &rawFragShaderSource, NULL);

	glCompileShader(vertShader);
	glCompileShader(fragShader);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	return program;
}

int main(int argc, char** argv) {
	/* initialize GLFW */
	if(!glfwInit()){
		std::cerr << "GLFW did not initialise" <<std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a Window */
	GLFWwindow* window;
	window = glfwCreateWindow(1600, 900, "Test 1" , NULL, NULL);
	if (!window){
		std::cerr << "Window was not created" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true;

	/* initialize GLEW */
	if (glewInit() != GLEW_OK){
		std::cerr << "GLEW failed to initialised" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Generate VAO
	GLuint vaoID;
	glGenVertexArrays(1,&vaoID);
	glBindVertexArray(vaoID);

	static const GLfloat verts[] = {
			// X, Y ,Z
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 0.0f,  1.0f, 0.0f,
			 //0.0f, -1.0f, 0.0f
	};

	GLuint program = LoadShaders("shader.vertshader", "shader.fragshader");

	// Generate VBO
	GLuint vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	/* loop */
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	 }
	return 0;


}

