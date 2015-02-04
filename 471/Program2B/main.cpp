// Program 2A - Thomas Steinke

#include <iostream>
#include <stdlib.h>

// Include graphics libraries
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>

#include "GLSL.h"
#include "shader.hpp"
#include "tiny_obj_loader.h"

// Globals
GLFWwindow *window;
using namespace std;

int width = 1;
int height = 1;

vector<tinyobj::shape_t> shapes;
vector<tinyobj::material_t> materials;
GLuint posBufObj = 0;
GLuint norBufObj = 0;
GLuint indBufObj = 0;
GLint aPos = 0;
GLint aNor = 0;
GLint uMV = 0;
GLint uP = 0;

void loadShapes(const string &objFile)
{
    string err = tinyobj::LoadObj(shapes, materials, objFile.c_str());
    if(!err.empty()) {
        cerr << err << endl;
    }
}

void initGL()
{
    // Set the background color
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
    // Enable Z-buffer test
    glEnable(GL_DEPTH_TEST);
    
    // Send the position array to the GPU
    const vector<float> &posBuf = shapes[0].mesh.positions;
    glGenBuffers(1, &posBufObj);
    glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
    
    // Send the normal array to the GPU
    const vector<float> &norBuf = shapes[0].mesh.normals;
    glGenBuffers(1, &norBufObj);
    glBindBuffer(GL_ARRAY_BUFFER, norBufObj);
    glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
    
    // Send the index array to the GPU
    const vector<unsigned int> &indBuf = shapes[0].mesh.indices;
    glGenBuffers(1, &indBufObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);
    
    // Unbind the arrays
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    GLSL::checkVersion();
    assert(glGetError() == GL_NO_ERROR);
}

void drawGL()
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use our GLSL program
    glUseProgram(prog);
    
    // Enable and bind position array for drawing
    GLSL::enableVertexAttribArray(aPos);
    glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
    glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Enable and bind normal array for drawing
    GLSL::enableVertexAttribArray(aNor);
    glBindBuffer(GL_ARRAY_BUFFER, norBufObj);
    glVertexAttribPointer(aNor, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Bind index array for drawing
    int nIndices = (int)shapes[0].mesh.indices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObj);
    
    // Compute and send the projection matrix
    float P[16];
    createPerspectiveMat(P, 45.0f, (float)width/height, 0.01f, 100.0f);
    glUniformMatrix4fv(uP, 1, GL_FALSE, P);
    
    glUniformMatrix4fv(uMV, 1, GL_FALSE, Final);
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
    
    
    // Disable and unbind
    GLSL::disableVertexAttribArray(aPos);
    GLSL::disableVertexAttribArray(aNor);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);
    
}

int main(void) {
    // Initialize GLFW
    if(!glfwInit()) {
        cerr << "Failed to initialize GLFW." << endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    // Open window and create OpenGL Context
    window = glfwCreateWindow(1024, 768, "Attractive Points", NULL, NULL);
    if(window == NULL) {
        cerr << "Failed to open GLFW window." << endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    if(glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW." << endl;
        return -1;
    }
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable (GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Initialize program
    GLuint programID   = LoadShaders("VertexShader.glsl", "FragmentShader.glsl");
    
    GLuint vertexPositionID = glGetAttribLocation(programID, "vertexPosition");
    GLuint vertexColorID = glGetAttribLocation(programID, "vertexColor");
    
    loadShapes("sphere.obj");
    
    GLfloat time = 0;
    
    do {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // ------ FOREGROUND --------------
        
        glUseProgram(programID);
        
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