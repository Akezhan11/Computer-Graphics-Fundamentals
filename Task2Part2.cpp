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

void addRectangle(std::vector<float>& vertices, float x, float y, float width, float height,
    float rB, float gB, float bB, float rT, float gT, float bT) {
    vertices.insert(vertices.end(), { x, y, rB, gB, bB });
    vertices.insert(vertices.end(), { x + width, y, rB, gB, bB });
    vertices.insert(vertices.end(), { x, y + height, rT, gT, bT });
    vertices.insert(vertices.end(), { x + width, y + height, rT, gT, bT });
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(500, 500, "Part 2: Cityscape", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

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

    int sunOffset = vertices.size() / 5;
    int segments = 40;
    vertices.push_back(0.0f); vertices.push_back(0.4f);
    vertices.push_back(1.0f); vertices.push_back(0.9f); vertices.push_back(0.2f);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * PI * i / segments;
        vertices.push_back(0.0f + 0.2f * cos(angle));
        vertices.push_back(0.4f + 0.2f * sin(angle));
        vertices.push_back(1.0f); vertices.push_back(0.5f); vertices.push_back(0.0f);
    }
    int sunCount = (vertices.size() / 5) - sunOffset;

    int leftBuildingOffset = vertices.size() / 5;
    addRectangle(vertices, -0.8f, -1.0f, 0.4f, 1.2f, 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f);
    addRectangle(vertices, -0.7f, 0.2f, 0.2f, 0.15f, 0.1f, 0.1f, 0.2f, 0.15f, 0.15f, 0.25f);
    int leftBuildingBlocks = 2;

    int rightBuildingOffset = vertices.size() / 5;
    addRectangle(vertices, 0.3f, -1.0f, 0.5f, 1.0f, 0.08f, 0.05f, 0.1f, 0.15f, 0.1f, 0.2f);
    addRectangle(vertices, 0.4f, 0.0f, 0.3f, 0.2f, 0.15f, 0.1f, 0.2f, 0.2f, 0.15f, 0.25f);
    int rightBuildingBlocks = 2;

    int birdsOffset = vertices.size() / 5;
    vertices.insert(vertices.end(), { -0.3f, 0.7f,  0.1f, 0.1f, 0.1f });
    vertices.insert(vertices.end(), { -0.2f, 0.65f, 0.0f, 0.0f, 0.0f });
    vertices.insert(vertices.end(), { -0.1f, 0.7f,  0.1f, 0.1f, 0.1f });
    vertices.insert(vertices.end(), { 0.15f, 0.8f,  0.1f, 0.1f, 0.1f });
    vertices.insert(vertices.end(), { 0.2f,  0.77f, 0.0f, 0.0f, 0.0f });
    vertices.insert(vertices.end(), { 0.25f, 0.8f,  0.1f, 0.1f, 0.1f });
    vertices.insert(vertices.end(), { 0.0f,  0.55f, 0.2f, 0.1f, 0.0f });
    vertices.insert(vertices.end(), { 0.03f, 0.53f, 0.1f, 0.0f, 0.0f });
    vertices.insert(vertices.end(), { 0.06f, 0.55f, 0.2f, 0.1f, 0.0f });
    int birdsCount = 9;
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

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLE_FAN, sunOffset, sunCount);

        for (int i = 0; i < leftBuildingBlocks; ++i) {
            glDrawArrays(GL_TRIANGLE_STRIP, leftBuildingOffset + (i * 4), 4);
        }

        for (int i = 0; i < rightBuildingBlocks; ++i) {
            glDrawArrays(GL_TRIANGLE_STRIP, rightBuildingOffset + (i * 4), 4);
        }

        glDrawArrays(GL_TRIANGLES, birdsOffset, birdsCount);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}