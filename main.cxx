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


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// I could not find a better way to do this sooooo...
const char *vertexShaderSource = "#version 460 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";


// This is an early definition for a function that tells GLFW what to
// do when the window gets resized
// In this one it resizes the viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// The inputter
void processInput(GLFWwindow *window);

int main(int argc, char **argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS


	// This creates the window (this window object is important)
	GLFWwindow* window = glfwCreateWindow(
		800,
		600,
		"Oh God PLease Work",
		0,
		0
	);
	if (!window) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	
	// Trying to load glad
	// istg if this doesn't work I'll really do it this time
	// gladLoadGLLoader() loads the OpenGL function pointers
	// This is OS specific so we use glfwGetProcAddress from the version
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
	
	
	// This is registers the framebuffer function with GLFW
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Shaders =========================================================
	// This creates the vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	// Assign the shader defined at the start of the file to the object above
	glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
	glCompileShader(vertexShader);
	
	// fragement shader time
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
	glCompileShader(fragmentShader);
	
	// add the shaders together
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	// kill the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	// Vertices & Indices ==============================================
	// Woh Square
	float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
	
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// Create Buffer and Array objects
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
	glClearColor(0.25f, 0.3f, 0.3f, 1.0f);
	while(!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);
		
		// Rendering stuff
		
		// Clears the Screen
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
