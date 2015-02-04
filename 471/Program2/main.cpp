// Program 2A - Thomas Steinke

#include <iostream>
#include <stdlib.h>

// Include graphics libraries
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"

#define NUM_POINTS 200000

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
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Initialize program
    GLuint programID   = LoadShaders("VertexShader.glsl", "FragmentShader.glsl");
    GLuint bgProgramID = LoadShaders("BGVertexShader.glsl", "BGFragmentShader.glsl");
    
    GLuint vertexPositionID = glGetAttribLocation(programID, "vertexPosition");
    GLuint circleID     = glGetUniformLocation(programID, "circle");
    GLuint radiusID     = glGetUniformLocation(programID, "radius");
    GLuint timeID       = glGetUniformLocation(programID, "time");
    GLuint speedID      = glGetUniformLocation(programID, "speed");
    GLuint windowScaleID= glGetUniformLocation(programID, "windowScale");
    GLuint w_widthID    = glGetUniformLocation(programID, "w_width");
    GLuint w_heightID   = glGetUniformLocation(programID, "w_height");
    
    GLuint bg_vertexPositionID  = glGetAttribLocation(bgProgramID, "vertexPosition");
    GLuint bg_w_widthID         = glGetUniformLocation(bgProgramID, "w_width");
    GLuint bg_w_heightID        = glGetUniformLocation(bgProgramID, "w_height");
    GLuint bg_radiusID          = glGetUniformLocation(bgProgramID, "radius");
    
    srand(time(NULL));
    GLfloat *points = (GLfloat *)calloc(NUM_POINTS * 3, sizeof(GLfloat));

    for(int i = 0; i < NUM_POINTS; i ++) {
        GLfloat x, y;
        do {
            x = points[3*i+0] = float(rand() % w_width)  * 6 / w_width  - 3;
            y = points[3*i+1] = float(rand() % w_height) * 6 / w_height - 3;
        } while(x * x + y * y < 0.2f);
        points[3*1+2] = 0.1f;
    }
    
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3* NUM_POINTS * sizeof(GLfloat), points, GL_STATIC_DRAW);
    
    GLfloat background[] = {
        -1, -1, -1,
         1, -1, -1,
         1,  1, -1,
        
        -1, -1, -1,
         1,  1, -1,
        -1,  1, -1
    };
    
    GLuint bgBuffer;
    glGenBuffers(1, &bgBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bgBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(background), background, GL_STATIC_DRAW);
    
    GLfloat time = 0;
    
    do {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // ------ BACKGROUND ------------
        
        glUseProgram(bgProgramID);
        glUniform1f(bg_radiusID, 0.17f);
        glUniform1i(bg_w_widthID, w_width);
        glUniform1i(bg_w_heightID, w_height);
        
        glEnableVertexAttribArray(bg_vertexPositionID);
        glBindBuffer(GL_ARRAY_BUFFER, bgBuffer);
        glVertexAttribPointer(
                              bg_vertexPositionID,// The attribute we want to configure
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glDisableVertexAttribArray(bg_vertexPositionID);
        
        // ------ FOREGROUND --------------
        
        glUseProgram(programID);
        
        // Set window scale
        if(w_width > w_height)
            glUniform2f(windowScaleID, (float) w_height / w_width, 1);
        else
            glUniform2f(windowScaleID, 1, (float) w_width / w_height);
        
        // Set window center
        glUniform1i(w_widthID, w_width);
        glUniform1i(w_heightID, w_height);
        
        glUniform1f(radiusID, 0.2f);
        time += 0.01f;
        glUniform1f(timeID, time);
        glUniform1f(speedID, 0.6f);
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