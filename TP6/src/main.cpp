#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace glm;

struct STRVertex {
    vec3 position;
    vec3 color;
};

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        printf("Could not initialize glfw.\n");
        return -1;
    }

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL TP 1", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set GLFW window hints
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Could not initialize GLAD");
        return -1;
    }

    // Load shaders
    GLuint ShaderProgram = LoadShaders("C:/Users/saoud/Desktop/TP6_PART2/include/shader/SimpleVertexShader.vertexshader", "C:/Users/saoud/Desktop/TP6_PART2/include/shader/SimpleFragmentShader.fragmentshader");


    // Define vertices for a triangle
STRVertex vertices[] = {
        // Base of the pyramid
        {{ 1.0f, 0.0f,  1.0f}, {1.0f, 0.0f, 0.0f}}, // Red color
        {{-1.0f, 0.0f,  1.0f}, {1.0f, 0.0f, 0.0f}},
        {{-1.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f, 0.0f,  1.0f}, {1.0f, 0.0f, 0.0f}},
        {{-1.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 1.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        // Sides of the pyramid - defining manually
        // Front face - Green
        {{-1.0f, 0.0f,  1.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 1.0f, 0.0f,  1.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 0.0f, 1.0f,  0.0f}, {0.0f, 1.0f, 0.0f}},
        // Right face - Blue
        {{ 1.0f, 0.0f,  1.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.0f, 1.0f,  0.0f}, {0.0f, 0.0f, 1.0f}},
        // Back face - Yellow
        {{ 1.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
        {{-1.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
        {{ 0.0f, 1.0f,  0.0f}, {1.0f, 1.0f, 0.0f}},
        // Left face - Magenta
        {{-1.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 1.0f}},
        {{-1.0f, 0.0f,  1.0f}, {1.0f, 0.0f, 1.0f}},
        {{ 0.0f, 1.0f,  0.0f}, {1.0f, 0.0f, 1.0f}},
    };

    // Set up vertex data (and buffer(s)) and configure vertex attributes
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, color));
    glEnableVertexAttribArray(1);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    mat4 Projection = perspective(radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    mat4 View = lookAt(vec3(2,2,5), vec3(0,0,0), vec3(0,1,0));
    mat4 Model = mat4(1.0f);
    Model = translate(Model, vec3(-1.0f, 0.0f, 0.0f));
    Model = scale(Model, vec3(2.5f, 1.5f, 1.0f));
    Model = rotate(Model, radians(45.0f), vec3(0.0f, 0.0f, 1.0f));
    mat4 MVP = Projection * View * Model;

    GLuint MatrixID = glGetUniformLocation(ShaderProgram, "MVP");

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glUseProgram(ShaderProgram);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
