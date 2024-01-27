#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
using namespace glm;

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




STRVertex vertices[] = {
   
    {vec3(1.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)},
    {vec3(-1.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)},
    {vec3(-1.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 1.0f)},
    {vec3(1.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 1.0f)},

    
    {vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)},
};


unsigned int vertex_indices[] = {
    0, 1, 2,   
    2, 3, 0,  

  
    0, 4, 1,
    
    1, 4, 2,

  
    2, 4, 3,

    
    3, 4, 0
};


    glfwSetKeyCallback(window, key_callback) ;


    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex) , (void*)offsetof(STRVertex,position) );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex) , (void*)offsetof(STRVertex,color) );

    glEnableVertexAttribArray(1);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    GLuint ShaderProgram;
    ShaderProgram = LoadShaders( "include/shader/SimpleVertexShader.vertexshader",
    "include/shader/SimpleFragmentShader.fragmentshader" );


     mat4 View = lookAt(vec3(-2,2,5),vec3(0,0,0),vec3(0,1,0) );
    

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    vec3 camPos = vec3(0.0f, 0.0f, 3.0f);
    vec3 camDirection = vec3(0.0f, 0.0f, -1.0f);
    vec3 camUp = vec3(0.0f, 1.0f, 0.0f);
    vec3 camRight = vec3(1.0f, 0.0f, 0.0f);

     while (!glfwWindowShouldClose(window))
    {
    

        float rayon = 5.0;
        float Xcam = rayon * sin(glfwGetTime());
        float Zcam = rayon * cos(glfwGetTime());
        View = lookAt(vec3(Xcam, 0.0, Zcam),vec3(0,0,0),vec3(0,1,0) );

        glfwGetCursorPos (window, &Xpos, &Ypos );
        glfwSetCursorPosCallback(window, cursor_position);

        glfwSetScrollCallback(window, scroll);
        mat4 Projection = perspective(radians(fov), 4.0f / 3.0f, 0.1f, 100.0f) ;

    
        float camSpeed = 0.0005f;
        if (glfwGetKey (window, GLFW_KEY_W) == GLFW_PRESS)
        camPos += camSpeed * camDirection;
        if (glfwGetKey (window, GLFW_KEY_S) == GLFW_PRESS)
        camPos -= camSpeed * camDirection;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camPos -= camSpeed * camRight;  
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camPos += camSpeed * camRight;  

        View = lookAt(camPos, camPos + camDirection , camUp );

        mat4 MVP = Projection * View * Model;


        GLuint MatrixID = glGetUniformLocation(ShaderProgram, "MVP");

        glfwSetMouseButtonCallback(window, mouse_button);






        glUseProgram(ShaderProgram);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
        glBindVertexArray(VAO);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);



       glDrawElements(GL_TRIANGLES, sizeof(vertex_indices) / sizeof(unsigned int), GL_UNSIGNED_INT, (void*)0);


        glfwSwapBuffers(window);
        glfwPollEvents();


    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}