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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_R:
                glClearColor(1.0f, 0.0f, 0.0f, 0.5f); // Set clear color to red
                break;
            case GLFW_KEY_V:
                glClearColor(0.0f, 1.0f, 0.0f, 0.5f); // Set clear color to green
                break;
            case GLFW_KEY_B:
                glClearColor(0.0f, 0.0f, 1.0f, 0.5f); // Set clear color to blue
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE); // Close the window
                break;
            default:
                glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Set clear color to black
        }
    }
}

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
    // Base (Square) - Assume it's blue
    {{ 1.0f, 0.0f,  1.0f}, {0.0f, 0.0f, 1.0f}}, // Front Right
    {{-1.0f, 0.0f,  1.0f}, {0.0f, 0.0f, 1.0f}}, // Front Left
    {{-1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}}, // Back Left
    {{ 1.0f, 0.0f,  1.0f}, {0.0f, 0.0f, 1.0f}}, // Front Right
    {{-1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}}, // Back Left
    {{ 1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}}, // Back Right

    // Side 1 (Front Face) - Assume it's magenta
    {{-1.0f, 0.0f,  1.0f}, {1.0f, 0.0f, 1.0f}}, // Front Left
    {{ 1.0f, 0.0f,  1.0f}, {1.0f, 0.0f, 1.0f}}, // Front Right
    {{ 0.0f, 1.0f,  0.0f}, {1.0f, 0.0f, 1.0f}}, // Top

    // Side 2 (Right Face) - Assume it's yellow
    {{ 1.0f, 0.0f,  1.0f}, {1.0f, 1.0f, 0.0f}}, // Front Right
    {{ 1.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}}, // Back Right
    {{ 0.0f, 1.0f,  0.0f}, {1.0f, 1.0f, 0.0f}}, // Top

    // Side 3 (Back Face) - Assume it's cyan
    {{ 1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 1.0f}}, // Back Right
    {{-1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 1.0f}}, // Back Left
    {{ 0.0f, 1.0f,  0.0f}, {0.0f, 1.0f, 1.0f}}, // Top

    // Side 4 (Left Face) - Assume it's green
    {{-1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}}, // Back Left
    {{-1.0f, 0.0f,  1.0f}, {0.0f, 1.0f, 0.0f}}, // Front Left
    {{ 0.0f, 1.0f,  0.0f}, {0.0f, 1.0f, 0.0f}}, // Top
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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwSetKeyCallback(window, key_callback);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(ShaderProgram);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
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
