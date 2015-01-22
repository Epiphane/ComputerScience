// Include standard headers
#include <iostream>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "shader.hpp"

void window_size_callback(GLFWwindow *window, int width, int height);

int w_width, w_height;

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    
    // Open a window and create its OpenGL context
    w_width = 1024;
    w_height = 768;
    window = glfwCreateWindow( w_width, w_height, "Triangles of Doom", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, window_size_callback);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Dark blue background
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "Lab4.vertexshader", "Lab4.fragmentshader" );
    
    // Get a handle for our buffers
    GLuint vertexPositionID = glGetAttribLocation (programID, "vertexPosition");
    GLuint vertexColorID    = glGetAttribLocation (programID, "vertexColor");
    GLuint windowScaleID    = glGetUniformLocation(programID, "windowScale");
    GLuint windowCenterID   = glGetUniformLocation(programID, "windowCenter");
    GLuint window_widthID   = glGetUniformLocation(programID, "w_width");
    GLuint window_heightID  = glGetUniformLocation(programID, "w_height");
    
    static const GLfloat g_vertex_buffer_data[] = {
        -0.8f, -0.9f, 0.0f,
        0.8f, -0.9f, 0.0f,
        0.0f,  0.9f, 0.0f,
        
        -0.9f, -0.9f, 0.0f,
        -0.9f,  0.9f, 0.0f,
        -0.1f,  0.9f, 0.0f,
        
        0.9f, -0.9f, 0.0f,
        0.9f,  0.9f, 0.0f,
        0.1f,  0.9f, 0.0f,
    };
    
    GLfloat g_color_buffer_data[] = {
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };
    
    GLuint vertexBuffer, colorBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    
    do{
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
        
        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Use our shader
        glUseProgram(programID);
        
        glUniform2f(windowCenterID, w_width / 2, w_height / 2);
        if(w_width > w_height)
            glUniform2f(windowScaleID, (float) w_height / w_width, 1);
        else
            glUniform2f(windowScaleID, 1, (float) w_width / w_height);
        
        glUniform1i(window_heightID, w_height);
        glUniform1i(window_widthID, w_width);
        
        // First attribute buffer : vertices
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
        
        // Second attribute buffer : vertices
        glEnableVertexAttribArray(vertexColorID);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(
                              vertexColorID,      // The attribute we want to configure
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 9); // 3 indices starting at 0 -> 1 triangle
        
        glDisableVertexAttribArray(vertexPositionID);
        glDisableVertexAttribArray(vertexColorID);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    
    // Cleanup VBO
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &colorBuffer);
    glDeleteProgram(programID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}

void window_size_callback(GLFWwindow *window, int width, int height) {
    w_width = width;
    w_height = height;
}

