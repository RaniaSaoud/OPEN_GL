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

double Xpos, Ypos;
double diffX = 0.0, diffY = 0.0;
float fov = 45.0f;
mat4 View = lookAt(vec3(2, 2, 5), vec3(0, 0, 0), vec3(0, 1, 0));
mat4 Model = mat4(1.0f);

void Reshape(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_R:
            glClearColor(1.0f, 0.0f, 0.0f, 0.5f); // Red
            break;
        case GLFW_KEY_V:
            glClearColor(0.0f, 1.0f, 0.0f, 0.5f); // Green
            break;
        case GLFW_KEY_B:
            glClearColor(0.0f, 0.0f, 1.0f, 0.5f); // Blue
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        default:
            glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Black
        }
    }
}

static void cursor_position(GLFWwindow* window, double x, double y)
{
    diffX = Xpos - x;
    diffY = y - Ypos;
    Model = rotate(Model, radians(float(diffX)), vec3(0.0f, 0.0f, 1.0f));
    Model = rotate(Model, radians(float(diffY)), vec3(1.0f, 0.0f, 0.0f));

}

struct STRVertex
{
    vec3 position;
    //vec3 color;
    vec2 texCoord;
    int textureIndex;
};


void scroll(GLFWwindow* window, double xoffset, double yoffset) {
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

void mouse_button(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        cout << "Left mouse button pressed." << endl;
    }
    
}

void Track_ball_cam() {
    float rayon = 5.0;
    float Xcam = rayon * sin(glfwGetTime());
    float Zcam = rayon * cos(glfwGetTime());
    View = lookAt(vec3(Xcam, 0.0, Zcam), vec3(0, 0, 0), vec3(0, 1, 0));
}

GLuint textures[5]; 

void InitTexture(void)
{
    char data[128 * 128 * 3];
    FILE* f = fopen("include/textures/damier.raw", "rb");
    if (f)
    {
        fread(data, 128 * 128 * 3, 1, f);
        fclose(f);
        glGenTextures(1, &textures[0]);
        glBindTexture(GL_TEXTURE_2D, textures[0]  );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }


    // This is the first face 
    FILE* f0 = fopen("include/textures/2.raw", "rb");
    fread(data, 128 * 128 * 3, 1, f0);
    fclose(f0);
    glGenTextures(1, &textures[1]);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // This is the second face
    FILE* f1 = fopen("include/textures/3.raw", "rb");
    fread(data, 128 * 128 * 3, 1, f1);
    fclose(f1);
    glGenTextures(1, &textures[2]);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    
    // This is the third face
    FILE* f2 = fopen("include/textures/4.raw", "rb");
    fread(data, 128 * 128 * 3, 1, f2);
    fclose(f2);
    glGenTextures(1, &textures[3]);
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    
    // This is the fourth face
    FILE* f3 = fopen("include/textures/1.raw", "rb");
    fread(data, 128 * 128 * 3, 1, f3);
    fclose(f3);
    glGenTextures(1, &textures[4]);
    glBindTexture(GL_TEXTURE_2D, textures[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    
    // // This is the last face
    // FILE* f4 = fopen("include/textures/5.raw", "rb");
    // fread(data, 128 * 128 * 3, 1, f4);
    // fclose(f4);
    // glGenTextures(1, &textures[5]);
    // glBindTexture(GL_TEXTURE_2D, textures[5]);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);



};


int main()
{


    if (!glfwInit())
    {
        printf("Could not initialize glfw.\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, Reshape);


    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);




    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Could not initialize GLAD.\n");
        return -1;
    }





    int repeatFactor = 5;

    STRVertex vertices[]= {
        
        // Carré
        {vec3(1.0f, 0.0f, 1.0f), vec2(repeatFactor, repeatFactor),0},
        {vec3(-1.0f, 0.0f, 1.0f), vec2(0.0f, repeatFactor),0},
        {vec3(-1.0f, 0.0f, -1.0f), vec2(0.0f, 0.0f),0},

        {vec3(-1.0f, 0.0f, -1.0f), vec2(0.0f, 0.0f),0},
        {vec3(1.0f, 0.0f, -1.0f), vec2(repeatFactor, 0.0f),0},
        {vec3(1.0f, 0.0f, 1.0f), vec2(repeatFactor, repeatFactor),0},

        // Triangle 1
        {vec3(0.0f, 1.0f, 0.0f), vec2(0.5f, 0.0f), 1},
        {vec3(-1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), 1},
        {vec3(-1.0f, 0.0f, -1.0f), vec2(0.0f, 1.0f), 1},

        // Triangle 2
        {vec3(0.0f, 1.0f, 0.0f), vec2(0.5f, 0.0f), 2},
        {vec3(1.0f, 0.0f, -1.0f), vec2(1.0f, 1.0f), 2},
        {vec3(-1.0f, 0.0f, -1.0f), vec2(0.0f, 1.0f), 2},

        // Triangle 3
        {vec3(0.0f, 1.0f, 0.0f), vec2(0.5f, 0.0f), 3},
        {vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), 3},
        {vec3(-1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), 3},

        // Triangle 4
        {vec3(0.0f, 1.0f, 0.0f), vec2(0.5f, 0.0f), 4},
        {vec3(1.0f, 0.0f, -1.0f), vec2(1.0f, 1.0f), 4},
        {vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), 4},

    };


    // camera FreeFly
    vec3 camPos = vec3(0.0f, 0.0f, 3.0f);
    vec3 camDirection = vec3(0.0f, 0.0f, -1.0f);
    vec3 camUp = vec3(0.0f, 1.0f, 0.0f);
    View = lookAt(camPos, camPos + camDirection, camUp);


    GLuint VAO;
    GLuint VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attribut 0 : position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, position));
    glEnableVertexAttribArray(0);

    // Attribut 2 : coordonnées de texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, texCoord));
    glEnableVertexAttribArray(2);

    // attribut 3 index 
     
   //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, textureIndex));
   //glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    // Load the texture
    //GLuint textureID;
    //glGenTextures(1, &textureID);
    //glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture wrapping mode to GL_REPEAT
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    GLuint ShaderProgram;
    ShaderProgram = LoadShaders("include/shader/SimpleVertexShader.vertexshader",
        "include/shader/SimpleFragmentShader.fragmentshader");

    using namespace glm;
    mat4 Projection = perspective(radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);
   

  

    //    mat4 Model = mat4(1.0f);
    //    Model = translate(Model,vec3(-1.0f, 0.0f, 0.0f));
    //    Model = scale(Model,vec3(2.5f, 1.5f ,1.0f));
    //    Model = rotate(Model,radians(45.0f),vec3(0.0f,0.0f,1.0f));

   
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    
    float camSpeed = 0.005f;
    glfwSetKeyCallback(window, keyCallback);
   // glfwSetScrollCallback(window, scroll_callback);
    glfwSetScrollCallback(window, scroll);
    glfwSetCursorPosCallback(window, cursor_position);

    InitTexture();
    GLuint TextureID = glGetUniformLocation(ShaderProgram, "OurTexture");
   

    while (!glfwWindowShouldClose(window))
    {


        glUseProgram(ShaderProgram);

        //Track_ball_cam();
        
       
       

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(VAO);

      
        // Update camera position based on user input
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camPos += camSpeed * camDirection;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camPos -= camSpeed * camDirection;

        // Ajouter le mouvement vers la droite et la gauche
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camPos += camSpeed * normalize(cross(camDirection, camUp)); // Utilisez cross pour obtenir le vecteur vers la droite
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camPos -= camSpeed * normalize(cross(camDirection, camUp)); //

        // Update the View matrix based on the new camera position
        View = lookAt(camPos, camPos + camDirection, camUp);



        glfwGetCursorPos(window, &Xpos, &Ypos);

        // Mettre � jour la matrice de projection en fonction du zoom de la souris
        Projection = perspective(radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);

        mat4 MVP = Projection * View * Model;
        GLuint MatrixID = glGetUniformLocation(ShaderProgram, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);



        // Repeat the 
        // Bind the texture outside of the loop

       /* glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);  // Assuming you want to use the texture at index 0
        glUniform1i(TextureID, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glDrawArrays(GL_TRIANGLES, 0, 18);*/


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glUniform1i(TextureID, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glUniform1i(TextureID, 1);
        glDrawArrays(GL_TRIANGLES, 6, 3);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textures[2]);
        glUniform1i(TextureID, 2);
        glDrawArrays(GL_TRIANGLES, 9, 3);


        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, textures[3]);
        glUniform1i(TextureID, 3);
        glDrawArrays(GL_TRIANGLES, 12, 3);


        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, textures[4]);
        glUniform1i(TextureID, 4);
        glDrawArrays(GL_TRIANGLES, 15, 3);

        glBindVertexArray(VAO);

    //    for (int i = 0; i < 6; ++i) {
    //         glActiveTexture(GL_TEXTURE0);
    //         glBindTexture(GL_TEXTURE_2D, textures[vertices[i * 3].textureIndex]);
    //         glUniform1i(TextureID, 0);

    //         glDrawArrays(GL_TRIANGLES, i * 3, 3);
            
    //     }
      
     

        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}
