#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

const float PI = 3.14159265359f;
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec3 vColor;

out vec3 fColor;

void main() {
    gl_Position = vec4(vPosition, 0.0, 1.0);
    fColor = vColor;
}
)";
const char* fragmentShaderSource = R"(
#version 330 core
in vec3 fColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(fColor, 1.0);
}
)";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(500, 500, "2D Polygons with Color", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::vector<float> vertices;
    int segments = 50;

    int ellipseOffset = vertices.size() / 5;
    vertices.push_back(-0.65f); vertices.push_back(0.7f);
    vertices.push_back(1.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * PI * i / segments;
        vertices.push_back(-0.65f + 0.25f * cos(angle));
        vertices.push_back(0.7f + 0.25f * sin(angle) * 0.6f);
        vertices.push_back(1.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
    }
    int ellipseCount = (vertices.size() / 5) - ellipseOffset;

    int triangleOffset = vertices.size() / 5;
    float r_tri = 0.28f;
    float angles_tri[3] = { PI / 2.0f, 7.0f * PI / 6.0f, 11.0f * PI / 6.0f };
    float colors_tri[3][3] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} };
    for (int i = 0; i < 3; ++i) {
        vertices.push_back(0.0f + r_tri * cos(angles_tri[i]));
        vertices.push_back(0.6f + r_tri * sin(angles_tri[i]));
        vertices.push_back(colors_tri[i][0]);
        vertices.push_back(colors_tri[i][1]);
        vertices.push_back(colors_tri[i][2]);
    }
    int triangleCount = 3;

    int circleOffset = vertices.size() / 5;
    vertices.push_back(0.65f); vertices.push_back(0.7f);
    vertices.push_back(0.3f); vertices.push_back(0.0f); vertices.push_back(0.0f);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * PI * i / segments;
        vertices.push_back(0.65f + 0.2f * cos(angle));
        vertices.push_back(0.7f + 0.2f * sin(angle));
        float red = 0.1f + 0.9f * ((cos(angle) + 1.0f) / 2.0f);
        vertices.push_back(red); vertices.push_back(0.0f); vertices.push_back(0.0f);
    }
    int circleCount = (vertices.size() / 5) - circleOffset;

    int squaresOffset = vertices.size() / 5;
    int num_squares = 6;
    float max_radius = 0.7f;
    for (int s = 0; s < num_squares; ++s) {
        float radius = max_radius - s * 0.12f;
        float color = (s % 2 == 0) ? 1.0f : 0.0f;

        vertices.push_back(0.0f); vertices.push_back(-0.3f);
        vertices.push_back(color); vertices.push_back(color); vertices.push_back(color);

        for (int i = 0; i <= 4; ++i) {
            float angle = PI / 4.0f + (PI / 2.0f) * i;
            vertices.push_back(0.0f + radius * cos(angle));
            vertices.push_back(-0.3f + radius * sin(angle));
            vertices.push_back(color); vertices.push_back(color); vertices.push_back(color);
        }
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLE_FAN, ellipseOffset, ellipseCount);

        glDrawArrays(GL_TRIANGLES, triangleOffset, triangleCount);

        glDrawArrays(GL_TRIANGLE_FAN, circleOffset, circleCount);

        for (int s = 0; s < num_squares; ++s) {
            glDrawArrays(GL_TRIANGLE_FAN, squaresOffset + (s * 6), 6);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}