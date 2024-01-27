#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "model/objload.hpp"

using namespace glm;
using namespace std;

void Reshape(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

struct STRVertex
{
    vec3 position;
    vec3 color;
};


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
  if (action == GLFW_RELEASE)  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
}


double Xpos,Ypos;
double diffX=0,diffY=0;
mat4 Model = mat4(1.0f);
mat4 View = mat4(1.0f);

double x=2.5f,y=1.5f,z=1.0f;
float fov=1.0f;


void mouse_button (GLFWwindow* window, int button, int action, int mods){
 if (action == GLFW_PRESS)
 switch (button)
 {
    case GLFW_MOUSE_BUTTON_RIGHT :
    Model = scale(Model,vec3( 2.0 , 2.0 , 2.0));
        break;

    case GLFW_MOUSE_BUTTON_LEFT :
    Model = scale(Model,vec3( 1.0/2.0 , 1.0/2.0 , 1.0/2.0));
        break;
}
}


static void cursor_position(GLFWwindow* window, double x, double y)
{
diffX = Xpos - x;
diffY = y - Ypos;
printf("DIFFX: %lf , DIFFY: %lf \n",diffX,diffY);

Model = rotate(Model,radians(float(diffX)),vec3(0.0f,0.0f,1.0f));
Model = rotate(Model,radians(float(diffY)),vec3(1.0f,0.0f,0.0f));
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


int main()
{
    if (!glfwInit())
   {
      printf("Could not initialize glfw.\n");
      return -1;
   }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR , 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow *window;
   window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
   glfwSetFramebufferSizeCallback(window, Reshape);


   if (!window)
   {
      glfwTerminate();
      return -1;
   }

   glfwMakeContextCurrent( window );




   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   {
        printf("Could not initialize GLAD.\n");
        return -1;
   }


    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textures;
    std::vector<glm::vec3> normals;

    loadOBJ("include/model/torus.obj", positions, textures, normals);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button);
    glfwSetCursorPosCallback(window, cursor_position);
    glfwSetScrollCallback(window, scroll);

    GLuint VAO;
    GLuint VBOPosition;
    GLuint VBOColor;
    GLuint VBONormal;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBOPosition);
    glBindBuffer(GL_ARRAY_BUFFER, VBOPosition);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);


    glGenBuffers(1, &VBOColor);
    std::vector<vec3> colors(positions.size(), vec3(1.0f, 0.0f, 1.0f));
    glBindBuffer(GL_ARRAY_BUFFER, VBOColor);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glGenBuffers(1, &VBONormal);
    glBindBuffer(GL_ARRAY_BUFFER, VBONormal);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2); 
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint ShaderProgram;
    ShaderProgram = LoadShaders("include/shader/SimpleVertexShader.vertexshader", "include/shader/SimpleFragmentShader.fragmentshader");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    

    vec3 camPos = vec3(0.0f, 0.0f, 3.0f);
    vec3 camDirection = vec3(0.0f, 0.0f, -1.0f);
    vec3 camUp = vec3(0.0f, 1.0f, 0.0f);
    vec3 camRight = vec3(1.0f, 0.0f, 0.0f);
    vec3 ambientLightColor = vec3(1.0f, 0.0f, 1.0f);
    float ambientIntensity = 0.8;

    glUseProgram(ShaderProgram);

    GLuint ProjectionLocation = glGetUniformLocation(ShaderProgram, "Projection");
    GLuint ViewLocation = glGetUniformLocation(ShaderProgram, "View");
    glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, &Projection[0][0]);
    glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &View[0][0]);
    GLuint ambientColorLocation = glGetUniformLocation(ShaderProgram, "ambientLightColor");
    GLuint ambientIntensityLocation = glGetUniformLocation(ShaderProgram, "ambientIntensity");
    glUniform3fv(ambientColorLocation, 1, &ambientLightColor[0]);
    vec3 diffuseLightColor = vec3(1.0f, 0.0f, 1.0f); // Example purple color for the diffuse light
    GLuint diffuseColorLocation = glGetUniformLocation(ShaderProgram, "diffuseLightColor");
    glUniform3fv(diffuseColorLocation, 1, &diffuseLightColor[0]);

    glUniform1f(ambientIntensityLocation, ambientIntensity);

    vec3 lightPos = vec3(1.0f, 1.0f, 5.0f); // Adjust as necessary
    GLuint LightPosID = glGetUniformLocation(ShaderProgram, "lightPos");
    glUniform3fv(LightPosID, 1, &lightPos[0]);

    vec3 camPos = vec3(0.0f, 0.0f, 3.0f);
    GLuint camPosLocation = glGetUniformLocation(ShaderProgram, "viewPos");
    glUniform3fv(camPosLocation, 1, &camPos[0]);


     while (!glfwWindowShouldClose(window))
    {
    

        float rayon = 5.0;
        float Xcam = rayon * sin(glfwGetTime());
        float Zcam = rayon * cos(glfwGetTime());
        View = lookAt(vec3(Xcam, 0.0, Zcam), vec3(0, 0, 0), vec3(0, 1, 0));

        glfwGetCursorPos(window, &Xpos, &Ypos);
        glfwSetCursorPosCallback(window, cursor_position);

        mat4 Projection = perspective(radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);

        float camSpeed = 0.0005f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camPos += camSpeed * camDirection;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camPos -= camSpeed * camDirection;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camPos -= camSpeed * camRight;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camPos += camSpeed * camRight;

        View = lookAt(camPos, camPos + camDirection, camUp);

        mat4 MVP = Projection * View * Model;

        GLuint MatrixID = glGetUniformLocation(ShaderProgram, "MVP");

        glUseProgram(ShaderProgram);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(VAO);
        glUniform3fv(LightPosID, 1, &lightPos[0]);
        //glUniform3fv(camPosLocation, 1, &camPos[0]);
        glUniform1f(ambientIntensityLocation, ambientIntensity);
        glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, &Projection[0][0]);
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &View[0][0]);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        //Draw the object
        glDrawArrays(GL_TRIANGLES, 0, positions.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBOPosition);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}