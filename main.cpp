// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include <GL/glew.h>
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

#include <common/shader.hpp>

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	int width = 1024;
	int height = 768;
	window = glfwCreateWindow(width, height, "Tutorial 01", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);



	glewExperimental = GL_TRUE;
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// -------------------------------------------------------------
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// ---------------------------Load Shaders----------------------------------
	GLuint programID = LoadShaders("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader");

	// ---------------------------MVP----------------------------------

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");	
	
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
	//glm::mat4 Projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);

	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 15), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model;
	glm::mat4 MVP2 = Projection * View * Model;


	glm::mat4 myTransMatrix = glm::translate(MVP, vec3(-5, 0, 0));
	MVP = myTransMatrix;

	myTransMatrix = glm::translate(MVP2, vec3(5, 0, 0));
	MVP2 = myTransMatrix;
	
	//scaling code
	/*glm::mat4 myScalingMatrix = glm::scale(MVP, glm::vec3(10, 20, 1));
	MVP = myScalingMatrix * MVP;*/

	// ---------------------------Make Object---------------------------------
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	static GLfloat g_color_buffer_data[12*3*3];

	for (int v = 0; v < 12 * 3; v++) {
		g_color_buffer_data[3 * v + 0] = 1;
		g_color_buffer_data[3 * v + 1] = 0;
		g_color_buffer_data[3 * v + 2] = 1;
	}

	static const GLfloat g_vertex_buffer_data_tri[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	static const GLfloat g_color_buffer_data_tri[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	GLuint vertexbuffer_tri;
	glGenBuffers(1, &vertexbuffer_tri);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_tri);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_tri), g_vertex_buffer_data_tri, GL_STATIC_DRAW);

	GLuint colorbuffer_tri;
	glGenBuffers(1, &colorbuffer_tri);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_tri);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data_tri), g_color_buffer_data_tri, GL_STATIC_DRAW);
	
	GLuint vertexbuffer;	
	glGenBuffers(1, &vertexbuffer);	
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);	
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	do {
		for (int v = 0; v < 12 * 3; v++) {
			g_color_buffer_data[3 * v + 0] = ((float)rand() / (float)(RAND_MAX)) * 1.0f;
			g_color_buffer_data[3 * v + 1] = ((float)rand() / (float)(RAND_MAX)) * 1.0f;
			g_color_buffer_data[3 * v + 2] = ((float)rand() / (float)(RAND_MAX)) * 1.0f;
		}

		GLuint colorbuffer;
		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//
		glUseProgram(programID);

		//apply window with MVP
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);		
		
		//draw object
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_tri);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_tri);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glEnableVertexAttribArray(0);					
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);	
		glVertexAttribPointer(							
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);
		glDrawArrays(GL_TRIANGLES, 0, 12*3);		

		//paint object
		glEnableVertexAttribArray(1);					
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);	
		glVertexAttribPointer(							
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

