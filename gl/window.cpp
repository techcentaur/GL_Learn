// # How to open a window

#include <stdio.h>
#include <stdlib.h>

// include glew before gl and glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "shader.hpp"

#include <glm/gtc/matrix_transform.hpp>


using namespace glm;

int main(){
	// Initialise GLFW
	glewExperimental = true;
	if(!glfwInit()){
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	// Open a openGL window and create context
	GLFWwindow* window;
	window = glfwCreateWindow(500, 400, "Tut", NULL, NULL);
	if(window==NULL){
		fprintf(stderr, "Failed to open window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if(glewInit()!=GLEW_OK){
		fprintf(stderr, "Failed to Initialise GLEW\n");
		return -1;
	}

	mat4 Projection = perspective(radians(45.0f), (float) 500 / (float)400, 0.1f, 100.0f);
	mat4 View = lookAt(
	    vec3(4,3,3), // Camera is at (4,3,3), in World Space
	    vec3(0,0,0), // and looks at the origin
	    vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	    );
	mat4 Model = mat4(1.0f);
	mat4 mvp = Projection * View * Model;

	GLuint VertexyArrayID;
	glGenVertexArrays(1, &VertexyArrayID);
	glBindVertexArray(VertexyArrayID);

	static const GLfloat vertexBufferData[]={
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,		
	};


	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	// create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("vertex.shader", "fragment.shader");
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");


	//Ensure that we can capture the escape key
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	do{
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*drawing here*/
		// 1st attribute buffer: vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

		glVertexAttribPointer(
		   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   3,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   0,                  // stride
		   (void*)0            // array buffer offset
		);

		// use my shader
		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

		// draw triangles
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
       glfwWindowShouldClose(window) == 0);
}