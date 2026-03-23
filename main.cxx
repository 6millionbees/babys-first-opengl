/*
 * main.cxx
 * 
 * Copyright 2026 6MillionBees <millionbees@millionbees-IdeaPad-3-17ITL6>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

// OpenGL loaders
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Math <3
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

// Just for fun
#include <iostream>

// My stuff
#include <mine/shader_s.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


int main(int argc, char **argv)
{
	// Initialization
	// =================================================================
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS
#endif

	// This creates the window (this window object is important)
	GLFWwindow* window = glfwCreateWindow(
		800, 600, "My Beautiful Daughter Named Golgotha", 0, 0);
	if (!window) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	// This is registers the framebuffer function with GLFW
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// Trying to load glad
	// gladLoadGLLoader() loads the OpenGL function pointers
	// It's OS specific so we use glfwGetProcAddress from the version
	// of GLFW we build for this OS to point to the correct dist of OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 1;
	}
	
	// This tells OpenGL the size of the viewport (or screen)
	// This allows OpenGL map the cordinate system it uses to coordinates
	// in the viewport.
	// Effectively making (-1 to 1) == (0, 800) or (0, 600) in relation
	// to the viewport
	glViewport(0, 0, 800, 600);
	

	// Shaders =========================================================

	Shader baseShader("shaders/vertex.glsl", "shaders/fragment.glsl");
	

	// Vertices & Indices ==============================================
	// Woh Square
	float vertices[] = {
         0.5f,  0.5f, -0.5f,  // front top right
         0.5f, -0.5f, -0.5f,  // front bottom right
        -0.5f, -0.5f, -0.5f,  // front bottom left
        -0.5f,  0.5f, -0.5f,  // front top left 
		 0.5f,  0.5f,  0.5f,  // back top right
         0.5f, -0.5f,  0.5f,  // back bottom right
        -0.5f, -0.5f,  0.5f,  // back bottom left
        -0.5f,  0.5f,  0.5f   // back top left 
		
    };
	unsigned int indices[] = {
		0, 1, 3, // 1
		1, 2, 3, // 1
		
		0, 1, 5, // 2
		0, 4, 5, // 2
		
		2, 1, 5, // 3
		2, 6, 7, // 3
		
		0, 3, 7, // 4
		0, 4, 7, // 4
		
		2, 3, 7, // 5
		2, 6, 7, // 5
		
		4, 5, 7, // 6
		5, 6, 7 //  6
		
	};
	

	// Create Buffer and Array objects
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
	
	
	
	// LETS MAKE THE GOD-FORSAKEN WINDOW DO SOMETHING
	// Main loop
	// =================================================================
	glClearColor(0.25f, 0.3f, 0.3f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while(!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);
		
		// Rendering stuff
		
		// Clears the Screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Update the transform matrix
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		trans = glm::rotate(trans, glm::radians(40.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		baseShader.use();
		baseShader.setTransform("transform", trans);
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		// check and call events and swap the buffers
		// whatever that means
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}

// This is the actual function definition
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}
