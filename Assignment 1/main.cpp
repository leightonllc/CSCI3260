///////////////////////////////////////////
////Type your name and student ID here/////
////Name: Lau Long Ching///////////////////
////Student ID: 1155127347/////////////////
///////////////////////////////////////////

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"

#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>


GLuint programID;
int zoom = 0;
int day = 1;
float hori = 0.;
float verti = 0.;
float fov = 0.;
int rotate = 0;


void get_OpenGL_info() {
    // OpenGL information
    const GLubyte* name = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* glversion = glGetString(GL_VERSION);
    std::cout << "OpenGL company: " << name << std::endl;
    std::cout << "Renderer name: " << renderer << std::endl;
    std::cout << "OpenGL version: " << glversion << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLuint VAOs[20];
GLuint VBOs[20];
GLuint EBOs[20];


void sendDataToOpenGL() {


    const GLfloat ground[] =
    {
        +1.0f,-0.0f,+1.0f,
        (float)151.0 / 256.0,(float)129.0 / 256.0, (float)85.0 / 256.0,

        +1.0f,-0.0f,-1.0f,
        (float)151.0 / 256.0,(float)129.0 / 256.0, (float)85.0 / 256.0,

        -1.0f,-0.0f,+1.0f,
        (float)224.0 / 256.0,(float)200.0 / 256.0, (float)157.0 / 256.0,

        -1.0f,-0.0f,-1.0f,
        (float)214.0 / 256.0,(float)180.0 / 256.0, (float)137.0 / 256.0,
    };


    glGenVertexArrays(1, &VAOs[0]);
    glBindVertexArray(VAOs[0]);  //first VAO

    glGenBuffers(1, &VBOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);

    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    //// with indexing (uncomment to use)
    GLuint ground_indices[] = { 2, 3, 0, 0, 1, 3 };
    //// index buffer
    glGenBuffers(1, &EBOs[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_indices), ground_indices, GL_STATIC_DRAW);

    const GLfloat frame[] =
    {
        -1.0f, 0.01f, -1.0f,
        (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,
        -1.0f, 0.01f, 1.0f,
       (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,
        -0.9f, 0.01f, -1.0f,
       (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,
        -0.9f, 0.01f, 1.0f,
       (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,

        -1.0f, 0.01f, -1.0f,
        (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,
        1.0f, 0.01f, -1.0f,
        (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,
        -1.0f, 0.01f, -0.9f,
       (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,
        1.0f, 0.01f, -0.9f,
        (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,

        1.0f, 0.01f, -1.0f,
        (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,
        1.0f, 0.01f, 1.0f,
        (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,
        0.9f, 0.01f, -1.0f,
        (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,
        0.9f, 0.01f, 1.0f,
        (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,

        1.0f, 0.01f, 1.0f,
        (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,
        -1.0f, 0.01f, 1.0f,
        (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,
        1.0f, 0.01f, 0.9f,
        (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,
        -1.0f, 0.01f, 0.9f,
        (float)73.0 / 256.0,(float)67.0 / 256.0, (float)47.0 / 256.0,
    };

    glGenVertexArrays(1, &VAOs[1]);
    glBindVertexArray(VAOs[1]);

    glGenBuffers(1, &VBOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frame), frame, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint frame_indices[] = {0, 1, 2, 1, 2, 3, 4, 5, 6, 5, 6, 7, 8, 9, 10, 9, 10, 11, 12, 13, 14, 13, 14, 15};

    glGenBuffers(1, &EBOs[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(frame_indices), frame_indices, GL_STATIC_DRAW);

    const GLfloat sun[] =
    {
        .0f, .0f, .0f,
        (float)255.0 / 256.0,(float)255.0 / 256.0, (float)0.0 / 256.0,
        -.05f, .1f, .0f,
        (float)255.0 / 256.0,(float)139.0 / 256.0, (float)40.0 / 256.0,
        .05f, .1f, .0f,
        (float)255.0 / 256.0,(float)139.0 / 256.0, (float)40.0 / 256.0,
        .1f, .05f, .0f,
        (float)255.0 / 256.0,(float)139.0 / 256.0, (float)40.0 / 256.0,
        .1f, -.05f, .0f,
        (float)255.0 / 256.0,(float)139.0 / 256.0, (float)40.0 / 256.0,
        .05f, -.1f, .0f,
        (float)255.0 / 256.0,(float)139.0 / 256.0, (float)40.0 / 256.0,
        -.05f, -.1f, .0f,
        (float)255.0 / 256.0,(float)139.0 / 256.0, (float)40.0 / 256.0,
        -.1f, -.05f, .0f,
        (float)255.0 / 256.0,(float)139.0 / 256.0, (float)40.0 / 256.0,
        -.1f, .05f, .0f,
        (float)255.0 / 256.0,(float)139.0 / 256.0, (float)40.0 / 256.0,

    };

    glGenVertexArrays(1, &VAOs[2]);
    glBindVertexArray(VAOs[2]); 

    glGenBuffers(1, &VBOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sun), sun, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint sun_indices[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7, 0, 7, 8, 0, 8, 1};

    glGenBuffers(1, &EBOs[2]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sun_indices), sun_indices, GL_STATIC_DRAW);

    const GLfloat labyrinth[] =
    {
        .0f,.01f, .0f,
        .4f, .4, .4f,
        .0f,.01f, 18.f,
        .4f, .4, .4f,
        1.0f,.01f, .0f,
        .4f, .4, .4f,
        1.f,.01f, 18.f,
        .4f, .4, .4f,

        1.f,.01f, 18.f,
        .4f, .4, .4f,
        17.f,.01f, 18.f,
        .4f, .4, .4f,
        1.f,.01f, 17.f,
        .4f, .4, .4f,
        17.f,.01f, 17.f,
        .4f, .4, .4f,

        17.f, .01f, 18.f,
        .4f, .4, .4f,
        17.f, .01f, .0f,
        .4f, .4, .4f,
        16.f, .01f, 18.f,
        .4f, .4, .4f,
        16.f, .01f, 0.f,
        .4f, .4, .4f,

        1.0f, .01f, 0.0f,
        .4f, .4f, .4f,
        6.0f, .01f, 0.f,
        .4f, .4f, .4f,
        1.f, .01f, 1.f,
        .4f, .4, .4f,
        6.f, .01f, 1.f,
        .4f, .4, .4f,

        2.f, .01f, 2.f,
        .4f, .4f, .4f,
        4.f, .01f, 2.f,
        .4f, .4f, .4f,
        2.f, .01f, 3.f,
        .4f, .4f, .4f,
        4.f, .01f, 3.f,
        .4f, .4f, .4f,

        2.f, .01f, 3.f,
        .4f, .4f, .4f,
        2.f, .01f, 16.f,
        .4f, .4f, .4f,
        3.f, .01f, 3.f,
        .4f, .4f, .4f,
        3.f, .01f, 16.f,
        .4f, .4f, .4f,

        3.f, .01f, 16.f,
        .4f, .4f, .4f,
        15.f, .01f, 16.f,
        .4f, .4f, .4f,
        3.f, .01f, 15.f,
        .4f, .4f, .4f,
        15.f, .01f, 15.f,
        .4f, .4f, .4f,

        14.f, .01f, 15.f,
        .4f, .4f, .4f,
        14.f, .01f, 2.f,
        .4f, .4f, .4f,
        15.f, .01f, 15.f,
        .4f, .4f, .4f,
        15.f, .01f, 2.f,
        .4f, .4f, .4f,

        9.f, .01f, 2.f,
        .4f, .4f, .4f,
        14.f, .01f, 2.f,
        .4f, .4f, .4f,
        9.f, .01f, 3.f,
        .4f, .4f, .4f,
        14.f, .01f, 3.f,
        .4f, .4f, .4f,

        9.f, .01f, 7.f,
        .4f, .4f, .4f,
        10.f, .01f, 7.f,
        .4f, .4f, .4f,
        9.f, .01f, 3.f,
        .4f, .4f, .4f,
        10.f, .01f, 3.f,
        .4f, .4f, .4f,

        //10

        10.f, .01f, 6.f,
        .4f, .4f, .4f,
        11.f, .01f, 6.f,
        .4f, .4f, .4f,
        10.f, .01f, 12.f,
        .4f, .4f, .4f,
        11.f, .01f, 12.f,
        .4f, .4f, .4f,

        10.f, .01f, 12.f,
        .4f, .4f, .4f,
        6.f, .01f, 12.f,
        .4f, .4f, .4f,
        10.f, .01f, 11.f,
        .4f, .4f, .4f,
        6.f, .01f, 11.f,
        .4f, .4f, .4f,

        6.f, .01f, 6.f,
        .4f, .4f, .4f,
        7.f, .01f, 6.f,
        .4f, .4f, .4f,
        6.f, .01f, 11.f,
        .4f, .4f, .4f,
        7.f, .01f, 11.f,
        .4f, .4f, .4f,

        7.f, .01f, 0.f,
        .4f, .4f, .4f,
        7.f, .01f, 7.f,
        .4f, .4f, .4f,
        8.f, .01f, 0.f,
        .4f, .4f, .4f,
        8.f, .01f, 7.f,
        .4f, .4f, .4f,

        8.f, .01f, 0.f,
        .4f, .4f, .4f,
        8.f, .01f, 1.f,
        .4f, .4f, .4f,
        16.f, .01f, 0.f,
        .4f, .4f, .4f,
        16.f, .01f, 1.f,
        .4f, .4f, .4f,

        5.f, .01f, 1.f,
        .4f, .4f, .4f,
        6.f, .01f, 1.f,
        .4f, .4f, .4f,
        5.f, .01f, 5.f,
        .4f, .4f, .4f,
        6.f, .01f, 5.f,
        .4f, .4f, .4f,

        4.f, .01f, 4.f,
        .4f, .4f, .4f,
        5.f, .01f, 4.f,
        .4f, .4f, .4f,
        4.f, .01f, 14.f,
        .4f, .4f, .4f,
        5.f, .01f, 14.f,
        .4f, .4f, .4f,

        5.f, .01f, 13.f,
        .4f, .4f, .4f,
        5.f, .01f, 14.f,
        .4f, .4f, .4f,
        13.f, .01f, 13.f,
        .4f, .4f, .4f,
        13.f, .01f, 14.f,
        .4f, .4f, .4f,

        11.f, .01f, 4.f,
        .4f, .4f, .4f,
        11.f, .01f, 5.f,
        .4f, .4f, .4f,
        13.f, .01f, 4.f,
        .4f, .4f, .4f,
        13.f, .01f, 5.f,
        .4f, .4f, .4f,

        12.f, .01f, 5.f,
        .4f, .4f, .4f,
        13.f, .01f, 5.f,
        .4f, .4f, .4f,
        12.f, .01f, 13.f,
        .4f, .4f, .4f,
        13.f, .01f, 13.f,
        .4f, .4f, .4f,

    };

    glGenVertexArrays(1, &VAOs[3]);
    glBindVertexArray(VAOs[3]);  

    glGenBuffers(1, &VBOs[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(labyrinth), labyrinth, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint labyrinth_indices[] = { 0, 1, 2, 1, 2, 3, 4, 5, 6, 5, 6, 7, 8, 9, 10, 9, 10, 11, 12, 13, 14, 13, 14, 15,
    16, 17, 18, 17, 18, 19, 20, 21, 22, 21, 22, 23, 24, 25, 26, 25, 26, 27,
    28, 29, 30, 29, 30, 31, 32, 33, 34, 33, 34, 35, 36, 37, 38, 37, 38, 39,
    40, 41, 42, 41, 42, 43, 44, 45, 46, 45, 46, 47, 48, 49, 50, 49, 50, 51,
    52, 53, 54, 53, 54, 55, 56, 57, 58, 57, 58, 59, 60, 61, 62, 61, 62, 63,
    64, 65, 66, 65, 66, 67, 68, 69, 70, 69, 70, 71, 72, 73, 74, 73, 74, 75,
    76, 77, 78, 77, 78, 79};

    glGenBuffers(1, &EBOs[3]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(labyrinth_indices), labyrinth_indices, GL_STATIC_DRAW);

    const GLfloat rock[] = {
        0.f, 0.03f, 0.f,
        .0f, .0f, .0f,
        .0f, .03f, 1.f,
        1.0f, 1.0f, 1.0f,
        1.f, .03f, 0.f,
        .0f, .0f, .0f,
        1.f, .03f, 1.f,
        .0f, .0f, .0f,

        .3f, .5f, .3f,
        (float)50.0 / 256.0,(float)87.0 / 256.0, (float)27.0 / 256.0,
        .3f, .5f, .7f,
        (float)50.0 / 256.0,(float)87.0 / 256.0, (float)27.0 / 256.0,
        .0f, .03f, .0f,
        (float)37.0 / 256.0,(float)34.0 / 256.0, (float)30.0 / 256.0,
        .0f, .03f, 1.f,
        (float)37.0 / 256.0,(float)34.0 / 256.0, (float)30.0 / 256.0,

        .3f, .5f, .7f,
        (float)50.0 / 256.0,(float)87.0 / 256.0, (float)27.0 / 256.0,
        .7f, .5f, .7f,
        (float)50.0 / 256.0,(float)87.0 / 256.0, (float)27.0 / 256.0,
        .0f, .03f, 1.f,
        (float)37.0 / 256.0,(float)34.0 / 256.0, (float)30.0 / 256.0,
        1.f, .03f, 1.f,
        (float)37.0 / 256.0,(float)34.0 / 256.0, (float)30.0 / 256.0,

        .7f, .5f, .7f,
        (float)50.0 / 256.0,(float)87.0 / 256.0, (float)27.0 / 256.0,
        .7f, .5f, .3f,
        (float)50.0 / 256.0,(float)87.0 / 256.0, (float)27.0 / 256.0,
        1.f, .03f, 1.f,
        (float)37.0 / 256.0,(float)34.0 / 256.0, (float)30.0 / 256.0,
        1.f, .03f, 0.f,
        (float)37.0 / 256.0,(float)34.0 / 256.0, (float)30.0 / 256.0,

        .3f, .5f, .3f,
        (float)50.0 / 256.0,(float)87.0 / 256.0, (float)27.0 / 256.0,
        .7f, .5f, .3f,
        (float)50.0 / 256.0,(float)87.0 / 256.0, (float)27.0 / 256.0,
        0.f, .03f, 0.f,
        (float)37.0 / 256.0,(float)34.0 / 256.0, (float)30.0 / 256.0,
        1.f, .03f, .0f, 
        (float)37.0 / 256.0,(float)34.0 / 256.0, (float)30.0 / 256.0,

        .3f, .5f, .3f,
        (float)50.0 / 256.0,(float)87.0 / 256.0, (float)27.0 / 256.0,
        .3f, .5f, .7f,
        (float)50.0 / 256.0,(float)87.0 / 256.0, (float)27.0 / 256.0,
        .7f, .5f, .3f,
        (float)50.0 / 256.0,(float)87.0 / 256.0, (float)27.0 / 256.0,
        .7f, .5f, .7f,
        (float)50.0 / 256.0,(float)87.0 / 256.0, (float)27.0 / 256.0,

    };

    glGenVertexArrays(1, &VAOs[4]);
    glBindVertexArray(VAOs[4]);  

    glGenBuffers(1, &VBOs[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rock), rock, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint rock_indices[] = {4, 5, 6, 5, 6, 7,
                             8, 9, 10, 9, 10, 11, 12, 13, 14, 13, 14, 15,
                             16, 17, 18, 17, 18, 19, 20, 21, 22, 21, 22, 23};

    glGenBuffers(1, &EBOs[4]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rock_indices), rock_indices, GL_STATIC_DRAW);

    const GLfloat moon[] = {
        .0f, .0f, .0f,
        (float)240.0 / 256.0,(float)196.0 / 256.0, (float)32.0 / 256.0,
        -.05f, .1f, .0f,
        (float)255.0 / 256.0,(float)244.0 / 256.0, (float)180.0 / 256.0,
        .05f, .1f, .0f,
        (float)255.0 / 256.0,(float)244.0 / 256.0, (float)180.0 / 256.0,
        .1f, .05f, .0f,
        (float)255.0 / 256.0,(float)244.0 / 256.0, (float)180.0 / 256.0,
        .1f, -.05f, .0f,
        (float)240.0 / 256.0,(float)196.0 / 256.0, (float)32.0 / 256.0,
        .05f, -.1f, .0f,
        (float)240.0 / 256.0,(float)196.0 / 256.0, (float)32.0 / 256.0,
        -.05f, -.1f, .0f,
        (float)240.0 / 256.0,(float)196.0 / 256.0, (float)32.0 / 256.0,
        -.1f, -.05f, .0f,
        (float)240.0 / 256.0,(float)196.0 / 256.0, (float)32.0 / 256.0,
        -.1f, .05f, .0f,
        (float)240.0 / 256.0,(float)196.0 / 256.0, (float)32.0 / 256.0,

    };

    glGenVertexArrays(1, &VAOs[5]);
    glBindVertexArray(VAOs[5]);  

    glGenBuffers(1, &VBOs[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(moon), moon, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint moon_indices[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7, 0, 7, 8, 0, 8, 1};

    glGenBuffers(1, &EBOs[5]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(moon_indices), moon_indices, GL_STATIC_DRAW);

    const GLfloat tree[] = {
        -2.5f, .0f, .0f,
        (float)86.0 / 256.0,(float)67.0 / 256.0, (float)53.0 / 256.0,
        .0f, 1.f, .0f,
        (float)118.0 / 256.0,(float)92.0 / 256.0, (float)72.0 / 256.0,
        .0f, .0f, 1.f, 
        (float)86.0 / 256.0,(float)67.0 / 256.0, (float)53.0 / 256.0,
        .0f, .0f, -1.f,
        (float)54.0 / 256.0,(float)42.0 / 256.0, (float)34.0 / 256.0,
        2.f, .0f, .0f,
        (float)86.0 / 256.0,(float)67.0 / 256.0, (float)53.0 / 256.0,
        .0f, .0f, 2.5f,
        (float)86.0 / 256.0,(float)67.0 / 256.0, (float)53.0 / 256.0,
        -1.0f, .0f, .0f, 
        (float)86.0 / 256.0,(float)67.0 / 256.0, (float)53.0 / 256.0,
        1.f, .0f, .0f,
        (float)54.0 / 256.0,(float)42.0 / 256.0, (float)34.0 / 256.0,
        .0f, .0f, -2.5f,
        (float)86.0 / 256.0,(float)67.0 / 256.0, (float)53.0 / 256.0,

        -1.f, 5.f, 0.f, 
        (float)118.0 / 256.0,(float)92.0 / 256.0, (float)72.0 / 256.0,
        .0f, 5.f, 1.f, 
        (float)118.0 / 256.0,(float)92.0 / 256.0, (float)72.0 / 256.0,
        1.0f, 5.f, .0f, 
        (float)118.0 / 256.0,(float)92.0 / 256.0, (float)72.0 / 256.0,
        0.f, 5.f, -1.f, 
        (float)118.0 / 256.0,(float)92.0 / 256.0, (float)72.0 / 256.0,

        2.5f, 5.f, 2.5f,
        (float)32.0 / 256.0,(float) 51.0 / 256.0, (float)23.0 / 256.0,
        -2.5f, 5.f, 2.5f,
        (float)45.0 / 256.0,(float)71.0 / 256.0, (float)32.0 / 256.0,
        2.5f, 5.f, -2.5f, 
        (float)66.0 / 256.0,(float)105.0 / 256.0, (float)47.0 / 256.0,
        -2.5f, 5.f, -2.5f,
        (float)66.0 / 256.0,(float)105.0 / 256.0, (float)47.0 / 256.0,

        2.5f, 9.f, 2.5f,
        (float)32.0 / 256.0,(float)51.0 / 256.0, (float)23.0 / 256.0,
        -2.5f, 9.f, 2.5f,
        (float)45.0 / 256.0,(float)71.0 / 256.0, (float)32.0 / 256.0,
        2.5f, 9.f, -2.5f,
        (float)66.0 / 256.0,(float)105.0 / 256.0, (float)47.0 / 256.0,
        -2.5f, 9.f, -2.5f,
        (float)66.0 / 256.0,(float)105.0 / 256.0, (float)47.0 / 256.0,

    };

    glGenVertexArrays(1, &VAOs[6]);
    glBindVertexArray(VAOs[6]);  

    glGenBuffers(1, &VBOs[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tree), tree, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint tree_indices[] = {
        0, 1, 2, 0, 1, 3,
        4, 1, 2, 4, 1, 3,
        5, 1, 6, 5, 1, 7,
        8, 1, 6, 8, 1, 7,
        
        2, 10, 6, 10, 6, 9,
        2, 10, 7, 10, 7, 11,
        7, 11, 3, 11, 3, 12,
        3, 12, 6, 12, 6, 9,

        13, 17, 14, 17, 14, 18,
        13, 17, 15, 17, 15, 19,
        15, 19, 16, 19, 16, 20,
        16, 20, 14, 20, 14, 18

    };

    glGenBuffers(1, &EBOs[6]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[6]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tree_indices), tree_indices, GL_STATIC_DRAW);

    const GLfloat pond[] = {
       .0f, .0f, .0f,
       (float)78.0 / 256.0,(float)165.0 / 256.0, (float)181.0 / 256.0,
       .0f, .0f, 1.f,
       (float)78.0 / 256.0,(float)165.0 / 256.0, (float)181.0 / 256.0,
       1.f, .0f, 0.f,
       (float)55.0 / 256.0,(float)107.0 / 256.0, (float)117.0 / 256.0,
       1.f, .0f, 1.f,
       (float)55.0 / 256.0,(float)107.0 / 256.0, (float)117.0 / 256.0,
    };

    glGenVertexArrays(1, &VAOs[7]);
    glBindVertexArray(VAOs[7]); 

    glGenBuffers(1, &VBOs[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pond), pond, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

    GLuint pond_indices[] = {
        0, 1, 2, 1, 2, 3
    };

    glGenBuffers(1, &EBOs[7]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[7]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pond_indices), pond_indices, GL_STATIC_DRAW);

}

bool checkStatus(
    GLuint objectID,
    PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
    PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
    GLenum statusType)
{
    GLint status;
    objectPropertyGetterFunc(objectID, statusType, &status);
    if (status != GL_TRUE)
    {
        GLint infoLogLength;
        objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* buffer = new GLchar[infoLogLength];

        GLsizei bufferSize;
        getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
        std::cout << buffer << std::endl;

        delete[] buffer;
        return false;
    }
    return true;
}

bool checkShaderStatus(GLuint shaderID) {
    return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID) {
    return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string readShaderCode(const char* fileName) {
    std::ifstream meInput(fileName);
    if (!meInput.good()) {
        std::cout << "File failed to load ... " << fileName << std::endl;
        exit(1);
    }
    return std::string(
        std::istreambuf_iterator<char>(meInput),
        std::istreambuf_iterator<char>()
    );
}

void installShaders() {
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* adapter[1];
    //adapter[0] = vertexShaderCode;
    std::string temp = readShaderCode("VertexShaderCode.glsl");
    adapter[0] = temp.c_str();
    glShaderSource(vertexShaderID, 1, adapter, 0);
    //adapter[0] = fragmentShaderCode;
    temp = readShaderCode("FragmentShaderCode.glsl");
    adapter[0] = temp.c_str();
    glShaderSource(fragmentShaderID, 1, adapter, 0);

    glCompileShader(vertexShaderID);
    glCompileShader(fragmentShaderID);

    if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
        return;

    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    if (!checkProgramStatus(programID))
        return;
    glUseProgram(programID);

}

void initializedGL(void) {
    // run only once
    sendDataToOpenGL();
    installShaders();
}

void paintGL(void) {
    // always run
    if (day == 1) glClearColor((float) 165.0/256.0, (float) 201.0/256.0, (float) 219 / 256, 0.0f);  //specify the background color
    else glClearColor((float)20.0 / 256.0, (float)24.0 / 256.0, (float)82 / 256, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 1.5f + fov, 0.1f, 20.0f);
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0, 1.2, 3 ), glm::vec3(0.1 , -1, -10), glm::vec3(0, 30, 0));
    glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
    GLint modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
    GLint viewMatrixUniformLocation = glGetUniformLocation(programID, "viewMatrix");
    GLint projectionMatrixUniformLocation = glGetUniformLocation(programID, "projectionMatrix");

    glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
    glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projectionMatrix[0][0]);


    glBindVertexArray(VAOs[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(VAOs[1]);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

    if (day == 1) {
        
        modelTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.2f + 0.01f * zoom, 1.6f, 0.f));
        modelTransformMatrix = glm::scale(modelTransformMatrix, glm::vec3(1.f + 0.2f * zoom, 1.f + 0.2f * zoom, 1.f));
        
        glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
        glBindVertexArray(VAOs[2]);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
        modelTransformMatrix = glm::mat4(1.0f);
        glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
    }
    
    modelTransformMatrix = glm::scale(modelTransformMatrix, glm::vec3(0.06f, 1.f, 0.07f));
    modelTransformMatrix = glm::rotate(modelTransformMatrix, glm::radians(180.f), glm::vec3(0, 1, 0));
    modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-3.f, 0.f, -9.4f));
    glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
    glBindVertexArray(VAOs[3]);
    glDrawElements(GL_TRIANGLES, 20*6, GL_UNSIGNED_INT, 0);
    modelTransformMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);

    modelTransformMatrix = glm::scale(modelTransformMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
    modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-2.f+hori, 0.f, 4.f+0.8f+verti));
    glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
    glBindVertexArray(VAOs[4]);
    glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
    modelTransformMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);

    if (day != 1) {
        modelTransformMatrix = glm::scale(modelTransformMatrix, glm::vec3(1.2f, 1.2f, 1.0f));
        modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(1.f, 1.3f, 0.f));
        modelTransformMatrix = glm::rotate(modelTransformMatrix, glm::radians(rotate * 4.5f), glm::vec3(0.0f, .0f, 1.0f));
        glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
        glBindVertexArray(VAOs[5]);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
        modelTransformMatrix = glm::mat4(1.0f);
        glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
    }

    modelTransformMatrix = glm::scale(modelTransformMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
    modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(+4.2f, 0.f,0.2f -3));
    glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
    glBindVertexArray(VAOs[6]);
    glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, 0);
    modelTransformMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);

    modelTransformMatrix = glm::scale(modelTransformMatrix, glm::vec3(0.6f, 1.f, 0.7f));
    modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(+0.38f, 0.03f, 0.2f));
    glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
    glBindVertexArray(VAOs[7]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_Q && action == GLFW_PRESS && day == 1) zoom += 1;
    if (key == GLFW_KEY_E && action == GLFW_PRESS && day == 1) 
        if (zoom >= -4) zoom -= 1;
    if (key == GLFW_KEY_Q && action == GLFW_PRESS && day == -1) rotate += 1;
    if (key == GLFW_KEY_E && action == GLFW_PRESS && day == -1) rotate -= 1;
    if (key == GLFW_KEY_D && action == GLFW_PRESS) day = -day;
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) hori += 0.1f;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) hori -= 0.1f;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) verti += 0.1f;
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) verti -= 0.1f;
    if (key == GLFW_KEY_O && action == GLFW_PRESS) fov += 0.1f;
    if (key == GLFW_KEY_P && action == GLFW_PRESS) fov -= 0.1f;

}

int main(int argc, char* argv[]) {
    GLFWwindow* window;

    /* Initialize the glfw */
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    /* glfw: configure; necessary for MAC */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* do not allow resizing */
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1024, 768, argv[0], NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    /* Initialize the glew */
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    get_OpenGL_info();
    initializedGL();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        paintGL();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
