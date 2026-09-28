#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0,0,width,height);
}


const char* vertexShaderSource = R"(
#version 330 core

layout(location= 0) in vec3 aPos;

uniform float uoffset;

void main(){
	gl_Position = vec4(aPos.x + uoffset, aPos.y, aPos.z , 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main(){
	FragColor = vec4(1.0, 0.5, 0.0, 1.0);
}
)";

int main() {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	std::cout << "GLFW initialized successfully" << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800,500,"Akezhan - Graphics Lab", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	std::cout << "My graphics lesson starts here" << std::endl;

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);



	float red = 0.0f;
	float green = 0.0f;
	float blue = 1.0f;

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	unsigned int VBO,VAO;
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1, &VBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);




	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1 , &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infolog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, infolog);
		std::cout << "vertex shader compilation failed: \n" << infolog << std::endl;
		glDeleteShader(vertexShader);
		glfwTerminate();
		return -1;
	}


	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr );
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infolog[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infolog);
		std::cout << "fragment shader compilation failed: \n" << infolog << std::endl;
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		glfwTerminate();
		return -1;
	}
	std::cout << "Both shaders compiled successfully" << std::endl;



	unsigned int shaderProgram = glCreateProgram();
	
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);


	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infolog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infolog);
		std::cout << "shader program linking failed: \n" << infolog << std::endl;

		glDeleteProgram(shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glfwTerminate();
		return -1;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	int offsetLocation = glGetUniformLocation(shaderProgram, "uoffset");
	float offset = 0.0f;


	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_R)==GLFW_PRESS ) {
			red = 1.0f;
			green = 0.0f;
			blue = 0.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_G)== GLFW_PRESS) {
			red = 0.0f;
			green = 1.0f;
			blue = 0.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_B)== GLFW_PRESS) {
			red = 0.0f;
			green = 0.0f;
			blue = 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_A)== GLFW_PRESS) {
			offset -= 0.04f;
		}
		if (glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS) {
			offset += 0.04f;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE)== GLFW_PRESS) {
			offset = 0.0f;
		}
		glClearColor(red, green, blue, 1.0f);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shaderProgram);
		glUniform1f(offsetLocation, offset);


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}