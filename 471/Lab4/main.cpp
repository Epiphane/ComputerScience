// Include standard headers
#include <iostream>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include "glfw3.h"
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "shader.hpp"

void window_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void window_size_callback(GLFWwindow *window, int width, int height);

int w_width, w_height;
GLuint program1, program2, program3, currentProgram;

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
    glfwSetKeyCallback(window, window_key_callback);
    
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
    program1 = LoadShaders( "VertexShader.glsl", "FragmentShader_1.glsl" );
    program2 = LoadShaders( "VertexShader.glsl", "FragmentShader_2.glsl" );
    program3 = LoadShaders( "VertexShader.glsl", "FragmentShader_3.glsl" );
    currentProgram = program1;
    
    // Get a handle for our buffers
    GLuint p1_vertexPositionID    = glGetAttribLocation (program1, "vertexPosition");
    GLuint p1_vertexColorID       = glGetAttribLocation (program1, "vertexColor");
    GLuint p1_vertexBarycentricID = glGetAttribLocation (program1, "vertexBC");
    GLuint p1_windowScaleID       = glGetUniformLocation(program1, "windowScale");
    GLuint p1_window_widthID      = glGetUniformLocation(program1, "w_width");
    GLuint p1_window_heightID     = glGetUniformLocation(program1, "w_height");
    
    GLuint p2_vertexPositionID    = glGetAttribLocation (program2, "vertexPosition");
    GLuint p2_vertexColorID       = glGetAttribLocation (program2, "vertexColor");
    GLuint p2_vertexBarycentricID = glGetAttribLocation (program2, "vertexBC");
    GLuint p2_windowScaleID       = glGetUniformLocation(program2, "windowScale");
    GLuint p2_window_widthID      = glGetUniformLocation(program2, "w_width");
    GLuint p2_window_heightID     = glGetUniformLocation(program2, "w_height");
    GLuint p2_windowCenterID      = glGetUniformLocation(program2, "windowCenter");
    
    GLuint p3_vertexPositionID    = glGetAttribLocation (program3, "vertexPosition");
    GLuint p3_vertexColorID       = glGetAttribLocation (program3, "vertexColor");
    GLuint p3_vertexBarycentricID = glGetAttribLocation (program3, "vertexBC");
    GLuint p3_windowScaleID       = glGetUniformLocation(program3, "windowScale");
    
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
    
    GLfloat g_barycentric_data[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    
    GLuint vertexBuffer, colorBuffer, barycentricBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    
    glGenBuffers(1, &barycentricBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, barycentricBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_barycentric_data), g_barycentric_data, GL_STATIC_DRAW);
    
    do{
        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Use selected shader
        glUseProgram(currentProgram);
        
        GLuint vertexPositionID, vertexColorID, vertexBarycentricID, windowScaleID;
        if(currentProgram == program1) {
            vertexPositionID = p1_vertexPositionID;
            vertexColorID = p1_vertexColorID;
            vertexBarycentricID = p1_vertexBarycentricID;
            windowScaleID = p1_windowScaleID;
            
            // Set window size
            glUniform1i(p1_window_heightID, w_height);
            glUniform1i(p1_window_widthID, w_width);
        }
        else if(currentProgram == program2) {
            vertexPositionID = p2_vertexPositionID;
            vertexColorID = p2_vertexColorID;
            vertexBarycentricID = p2_vertexBarycentricID;
            windowScaleID = p2_windowScaleID;
            
            // Set window center
            glUniform2f(p2_windowCenterID, w_width / 2, w_height / 2);
            
            // Set window size
            glUniform1i(p2_window_heightID, w_height);
            glUniform1i(p2_window_widthID, w_width);
        }
        else {
            vertexPositionID = p3_vertexPositionID;
            vertexColorID = p3_vertexColorID;
            vertexBarycentricID = p3_vertexBarycentricID;
            windowScaleID = p3_windowScaleID;
        }
        
        // Set window scale
        if(w_width > w_height)
            glUniform2f(windowScaleID, (float) w_height / w_width, 1);
        else
            glUniform2f(windowScaleID, 1, (float) w_width / w_height);
        
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
        
        // Second attribute buffer : vertices
        glEnableVertexAttribArray(vertexBarycentricID);
        glBindBuffer(GL_ARRAY_BUFFER, barycentricBuffer);
        glVertexAttribPointer(
                              vertexBarycentricID,// The attribute we want to configure
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
        glDisableVertexAttribArray(vertexBarycentricID);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    
    // Cleanup VBO
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &colorBuffer);
    glDeleteBuffers(1, &barycentricBuffer);
    glDeleteProgram(program1);
    glDeleteProgram(program2);
    glDeleteProgram(program3);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}

void window_size_callback(GLFWwindow *window, int width, int height) {
    w_width = width;
    w_height = height;
}

void window_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(action == GLFW_RELEASE) {
        if(currentProgram == program1)
            currentProgram = program2;
        else if(currentProgram == program2)
            currentProgram = program3;
        else
            currentProgram = program1;
    }
}