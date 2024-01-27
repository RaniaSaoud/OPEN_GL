#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace glm;

struct STRVertex
{
    vec3 position;
    vec3 color;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    switch (key)
    {
        case GLFW_KEY_R :
        glClearColor(1.0f, 0.0f, 0.0f, 0.5f);
        break;
        case GLFW_KEY_V :
        glClearColor(0.0f, 1.0f, 0.0f, 0.5f);
        break;
        case GLFW_KEY_B :
        glClearColor(0.0f, 0.0f, 1.0f, 0.5f);
        break;
        case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
        default:
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    }
    if(action == GLFW_RELEASE){
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    }
}


double Xpos, Ypos;
GLfloat fov = 45.0f;
mat4 Model = mat4(1.0f);


static void cursor_position(GLFWwindow* window, double x, double y)
{
    double diffX = Xpos - x;
    double diffY = y - Ypos;
    Model = rotate(Model,radians(float(diffX*0.005)),vec3(0.0f,0.0f,1.0f));
    Model = rotate(Model,radians(float(diffY*0.005)),vec3(1.0f,0.0f,0.0f));
}


void mouse_button (GLFWwindow* window, int button, int action, int mods){
    if(action == GLFW_PRESS){
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_RIGHT:
            Model = scale(Model, vec3(2.0f, 2.0f, 2.0f));
            break;
        case GLFW_MOUSE_BUTTON_LEFT:
            Model = scale(Model, vec3(0.5f, 0.5f, 0.5f));
        default:
            Model = scale(Model,vec3(1.0f, 1.0f, 1.0f) );
            break;
        }
    }
}

void scroll (GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
    fov -= yoffset;
    if (fov <= 1.0f)
    fov = 1.0f;
    if (fov >= 45.0f)
    fov = 45.0f;
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
    GLuint ShaderProgram = LoadShaders("C:/Users/saoud/Desktop/TP7_PART2/include/shader/SimpleVertexShader.vertexshader", "C:/Users/saoud/Desktop/TP7_PART2/include/shader/SimpleFragmentShader.fragmentshader");


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

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   
    glBindVertexArray(0);

    //mat4 Projection = perspective(radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    mat4 View = lookAt(vec3(2,2,5), vec3(0,0,0), vec3(0,1,0));
    // mat4 Model = mat4(1.0f);
    // Model = translate(Model, vec3(-1.0f, 0.0f, 0.0f));
    // Model = scale(Model, vec3(2.5f, 1.5f, 1.0f));
    // Model = rotate(Model, radians(45.0f), vec3(0.0f, 0.0f, 1.0f));
    // mat4 MVP = Projection * View * Model;

    GLuint MatrixID = glGetUniformLocation(ShaderProgram, "MVP");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glfwGetCursorPos (window, &Xpos, &Ypos );
    glfwSetCursorPosCallback(window, cursor_position);
    glfwSetMouseButtonCallback(window, mouse_button);
    glfwSetScrollCallback(window, scroll);
    // glfwSetCursorPosCallback(window, cursor_position_callback);
    // glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetScrollCallback(window, scroll_callback);

    // float radius = 5.0f;


    vec3 camPos = vec3(0.0f, 0.0f, 3.0f);
    vec3 camDirection = vec3(0.0f, 0.0f, -1.0f);
    vec3 camUp = vec3(0.0f, 1.0f, 0.0f);
    vec3 camRight = vec3(1.0f, 0.0f, 0.0f);

    // Render loop
    while (!glfwWindowShouldClose(window)) {

        //glfwGetCursorPos (window, &mousePosX, &mousePosY );

        //glfwPollEvents();
        // camRight = glm::normalize(glm::cross(camUp, -camDirection));
        // // float camX = sin(glfwGetTime()) * radius;
        // // float camZ = cos(glfwGetTime()) * radius;
        //  View = glm::lookAt(camPos, camPos + camDirection, camUp);
        
        float rayon = 5.0;
        float Xcam = rayon * sin(glfwGetTime());
        float Zcam = rayon * cos(glfwGetTime());
        View = lookAt(vec3(Xcam, 0.0, Zcam),vec3(0,0,0),vec3(0,1,0) );

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
        // // Free-fly camera movement logic
        // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        //     camPos += camSpeed * camDirection;
        // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        //     camPos -= camSpeed * camDirection;

        // // Combine the view matrix from the free-fly camera logic with the trackball camera's view matrix
        // View = glm::lookAt(camPos, camPos + camDirection, camUp);

        glUseProgram(ShaderProgram);

        int state = glfwGetKey(window, GLFW_KEY_E);
        //mat4 MVP = Projection * View * Model;
        if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)

        mat4 Projection = perspective(radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);

        mat4 MVP = Projection * View * Model;

        GLuint matrixID = glGetUniformLocation(shaderProgramId, "MVP");
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
        
        // 3. now draw the object
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 18);


        glfwSwapBuffers(window);

        glfwPollEvents();

        // glfwSetCursorPosCallback(window, cursor_position);

        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glUseProgram(ShaderProgram);
        // glfwSetScrollCallback(window, scroll);
        // glfwSetScrollCallback(window, scroll);

        // GLuint MatrixID = glGetUniformLocation(ShaderProgram, "MVP");

        // glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 100);

        // glfwSwapBuffers(window);
        // glfwPollEvents();
    }

    // Deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        switch (key)
        {
            case GLFW_KEY_R :
                glClearColor(1.0f, 0.0f, 0.0f, 0.5f);
                break;
            case GLFW_KEY_V :
                glClearColor(0.0f, 1.0f, 0.0f, 0.5f);
                break;
            case GLFW_KEY_B :
                glClearColor(0.0f, 0.0f, 1.0f, 0.5f);
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            default:
                glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
        }
    if (action == GLFW_RELEASE)
        glClearColor(0.3f, 0.4f, 0.3f, 0.5f);
}
