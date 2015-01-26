#include <iostream>
#include <stdlib.h>

// Include graphics libraries
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"

#define NUM_POINTS 40

// Globals
int w_width, w_height;
GLFWwindow *window;
using namespace std;

void window_size_callback(GLFWwindow *window, int width, int height);

int main(void) {
    // Initialize GLFW
    if(!glfwInit()) {
        cerr << "Failed to initialize GLFW." << endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    // Open window and create OpenGL Context
    w_width = 1024;
    w_height = 768;
    window = glfwCreateWindow(w_width, w_height, "Attractive Points", NULL, NULL);
    if(window == NULL) {
        cerr << "Failed to open GLFW window." << endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, window_size_callback);
    
    // Initialize GLEW
    if(glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW." << endl;
        return -1;
    }
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    
    // Initialize program
    GLuint programID = LoadShaders("VertexShader.glsl", "FragmentShader.glsl");
    
    GLuint vertexPositionID = glGetAttribLocation(programID, "vertexPosition");
    GLuint circleID = glGetUniformLocation(programID, "circle");
    GLuint radiusID = glGetUniformLocation(programID, "radius");
    GLuint timeID = glGetUniformLocation(programID, "time");
    GLuint speedID = glGetUniformLocation(programID, "speed");
    
    srand(time(NULL));
    GLfloat *points = (GLfloat *)calloc(NUM_POINTS * 3, sizeof(GLfloat));
    for(int i = 0; i < NUM_POINTS; i ++) {
        points[3*i+0] = float(rand() % w_width)  * 2 / w_width  - 1;
        points[3*i+1] = float(rand() % w_height) * 2 / w_height - 1;
        points[3*1+2] = 0;
    }
    
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3* NUM_POINTS * sizeof(GLfloat), points, GL_STATIC_DRAW);
    
    do {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(programID);
        
        glUniform1f(radiusID, 50);
        glUniform1f(timeID, 0.0f);
        glUniform1f(speedID, 0);
        glUniform3f(circleID, 0, 0, 0);
        
        glEnableVertexAttribArray(vertexPositionID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(
                              vertexPositionID,   // The attribute we want to configure
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        glDrawArrays(GL_POINTS, 0, NUM_POINTS);
        
        glDisableVertexAttribArray(vertexPositionID);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);
    
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteProgram(programID);
    
    glfwTerminate();
    
    return 0;
}

void window_size_callback(GLFWwindow *window, int width, int height) {
    w_width = width;
    w_height = height;
}