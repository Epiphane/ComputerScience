// Base code for program 3 - does not compile as is, needs other files
//and shaders but has skeleton for much of the data transfer for shading
//and traversal of the mesh for computing normals - you must compute normals

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <stdio.h>
#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

GLFWwindow* window;
using namespace std;

vector<tinyobj::shape_t> shapes;
vector<tinyobj::material_t> materials;

int g_SM = 0, g_SN = 0;
int g_width;
int g_height;
float g_Camtrans = -2.5;
float g_angle = 0, g_x_angle = 0;
int g_mat_id =0;
glm::vec3 g_trans(0, 0, 0);
glm::vec3 g_trans_2(3, 0, -2);
glm::vec3 g_trans_3(-3, 0, -2);
glm::vec3 g_light(2, 6, 6);

GLuint ShadeProg;
GLuint posBufObj = 0;
GLuint norBufObj = 0;
GLuint indBufObj = 0;

//Handles to the shader data
GLint h_aPosition;
GLint h_aNormal;
GLint h_uModelMatrix;
GLint h_uViewMatrix;
GLint h_uProjMatrix;
GLint h_uLightPos;
GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;
GLint h_uShadeM;
GLint h_uShowNorm;

/* helper function to make sure your matrix handle is correct */
inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
    if (handle >= 0)
        glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

/* helper function to send materials to the shader - you must create your own */
void SetMaterial(int i) {
    
    glUseProgram(ShadeProg);
    switch (i) {
        case 0: //shiny blue plastic
            glUniform3f(h_uMatAmb, 0.02, 0.02, 0.1);
            glUniform3f(h_uMatDif, 0.0, 0.08, 0.5);
            glUniform3f(h_uMatSpec, 0.14, 0.14, 0.4);
            glUniform1f(h_uMatShine, 120.0);
            break;
        case 1: // flat grey
            glUniform3f(h_uMatAmb, 0.13, 0.13, 0.14);
            glUniform3f(h_uMatDif, 0.3, 0.3, 0.4);
            glUniform3f(h_uMatSpec, 0.3, 0.3, 0.4);
            glUniform1f(h_uMatShine, 4.0);
            break;
        case 2: //gold
            glUniform3f(h_uMatAmb, 0.09, 0.07, 0.08);
            glUniform3f(h_uMatDif, 0.91, 0.782, 0.82);
            glUniform3f(h_uMatSpec, 1.0, 0.913, 0.8);
            glUniform1f(h_uMatShine, 200.0);
            break;
        case 3: // Jade
            glUniform3f(h_uMatAmb, 1, 1, 1);
            glUniform3f(h_uMatDif, -0.9, -0.75, -0.8);
            glUniform3f(h_uMatSpec, 1.0, 1, 1);
            glUniform1f(h_uMatShine, 1000.0);
            break;
    }
}

/* helper function to set projection matrix - don't touch */
void SetProjectionMatrix() {
    glm::mat4 Projection = glm::perspective(90.0f, (float)g_width/g_height, 0.1f, 100.f);
    safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(Projection));
}

/* camera controls - do not change beyond the current set up to rotate*/
void SetView() {
    glm::mat4 Trans = glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0, g_Camtrans));
    safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(Trans));
}

/* model transforms */
void SetModel(int model) {
    glm::mat4 com;
    if(model == 1) {
        glm::mat4 Trans = glm::translate( glm::mat4(1.0f), g_trans);
        glm::mat4 RotateY = glm::rotate( glm::mat4(1.0f), g_angle, glm::vec3(0.0f, 1, 0));
        glm::mat4 RotateX = glm::rotate( glm::mat4(1.0f), g_x_angle, glm::vec3(1.0f, 0, 0));
        com = Trans*RotateY*RotateX;
    }
    else if(model == 2) {
        glm::mat4 Trans = glm::translate( glm::mat4(1.0f), g_trans_2);
        glm::mat4 RotateY = glm::rotate( glm::mat4(1.0f), g_angle, glm::vec3(0.0f, 1, 0));
        glm::mat4 RotateX = glm::rotate( glm::mat4(1.0f), g_x_angle, glm::vec3(1.0f, 0, 0));
        com = Trans*RotateY*RotateX;
    }
    else {
        glm::mat4 Trans = glm::translate( glm::mat4(1.0f), g_trans_3);
        glm::mat4 RotateY = glm::rotate( glm::mat4(1.0f), g_angle, glm::vec3(0.0f, 1, 0));
        glm::mat4 RotateX = glm::rotate( glm::mat4(1.0f), g_x_angle, glm::vec3(1.0f, 0, 0));
        com = Trans*RotateY*RotateX;
    }
    safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(com));
}

//Given a vector of shapes which has already been read from an obj file
// resize all vertices to the range [-1, 1]
void resize_obj(std::vector<tinyobj::shape_t> &shapes){
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    float scaleX, scaleY, scaleZ;
    float shiftX, shiftY, shiftZ;
    float epsilon = 0.001;
    
    minX = minY = minZ = 1.1754E+38F;
    maxX = maxY = maxZ = -1.1754E+38F;
    
    //Go through all vertices to determine min and max of each dimension
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            if(shapes[i].mesh.positions[3*v+0] < minX) minX = shapes[i].mesh.positions[3*v+0];
            if(shapes[i].mesh.positions[3*v+0] > maxX) maxX = shapes[i].mesh.positions[3*v+0];
            
            if(shapes[i].mesh.positions[3*v+1] < minY) minY = shapes[i].mesh.positions[3*v+1];
            if(shapes[i].mesh.positions[3*v+1] > maxY) maxY = shapes[i].mesh.positions[3*v+1];
            
            if(shapes[i].mesh.positions[3*v+2] < minZ) minZ = shapes[i].mesh.positions[3*v+2];
            if(shapes[i].mesh.positions[3*v+2] > maxZ) maxZ = shapes[i].mesh.positions[3*v+2];
        }
    }
    //From min and max compute necessary scale and shift for each dimension
    float maxExtent, xExtent, yExtent, zExtent;
    xExtent = maxX-minX;
    yExtent = maxY-minY;
    zExtent = maxZ-minZ;
    if (xExtent >= yExtent && xExtent >= zExtent) {
        maxExtent = xExtent;
    }
    if (yExtent >= xExtent && yExtent >= zExtent) {
        maxExtent = yExtent;
    }
    if (zExtent >= xExtent && zExtent >= yExtent) {
        maxExtent = zExtent;
    }
    scaleX = 2.0 /maxExtent;
    shiftX = minX + (xExtent/ 2.0);
    scaleY = 2.0 / maxExtent;
    shiftY = minY + (yExtent / 2.0);
    scaleZ = 2.0/ maxExtent;
    shiftZ = minZ + (zExtent)/2.0;
    
    //Go through all verticies shift and scale them
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            shapes[i].mesh.positions[3*v+0] = (shapes[i].mesh.positions[3*v+0] - shiftX) * scaleX;
            assert(shapes[i].mesh.positions[3*v+0] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+0] <= 1.0 + epsilon);
            shapes[i].mesh.positions[3*v+1] = (shapes[i].mesh.positions[3*v+1] - shiftY) * scaleY;
            assert(shapes[i].mesh.positions[3*v+1] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+1] <= 1.0 + epsilon);
            shapes[i].mesh.positions[3*v+2] = (shapes[i].mesh.positions[3*v+2] - shiftZ) * scaleZ;
            assert(shapes[i].mesh.positions[3*v+2] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+2] <= 1.0 + epsilon);
        }
    }
}



void loadShapes(const string &objFile)
{
    string err = tinyobj::LoadObj(shapes, materials, objFile.c_str());
    if(!err.empty()) {
        cerr << err << endl;
    }
    resize_obj(shapes);
}

void initGL()
{
    // Set the background color
    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
    // Enable Z-buffer test
    glEnable(GL_DEPTH_TEST);
    glPointSize(18);
    
    // Send the position array to the GPU
    const vector<float> &posBuf = shapes[0].mesh.positions;
    glGenBuffers(1, &posBufObj);
    glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
    
    vector<float> norBuf;
    int idx1, idx2, idx3;
    glm::vec3 v1, v2, v3;
    //for every vertex initialize a normal to 0
    for (int j = 0; j < shapes[0].mesh.positions.size()/3; j++) {
        norBuf.push_back(0);
        norBuf.push_back(0);
        norBuf.push_back(0);
    }
    // DO work here to compute the normals for every face
    //then add its normal to its associated vertex
    for (int i = 0; i < shapes[0].mesh.indices.size()/3; i++) {
        idx1 = shapes[0].mesh.indices[3*i+0];
        idx2 = shapes[0].mesh.indices[3*i+1];
        idx3 = shapes[0].mesh.indices[3*i+2];
        v1 = glm::vec3(shapes[0].mesh.positions[3*idx1 +0],shapes[0].mesh.positions[3*idx1 +1], shapes[0].mesh.positions[3*idx1 +2]);
        v2 = glm::vec3(shapes[0].mesh.positions[3*idx2 +0],shapes[0].mesh.positions[3*idx2 +1], shapes[0].mesh.positions[3*idx2 +2]);
        v3 = glm::vec3(shapes[0].mesh.positions[3*idx3 +0],shapes[0].mesh.positions[3*idx3 +1], shapes[0].mesh.positions[3*idx3 +2]);
        
        glm::vec3 u, v;
        
        u = v2 - v1;
        v = v3 - v1;
        norBuf[3*idx1+0] += u.y * v.z - u.z * v.y;
        norBuf[3*idx1+1] += u.z * v.x - u.x * v.z;
        norBuf[3*idx1+2] += u.x * v.y - u.y * v.x;
        
        u = v3 - v2;
        v = v1 - v2;
        norBuf[3*idx2+0] += u.y * v.z - u.z * v.y;
        norBuf[3*idx2+1] += u.z * v.x - u.x * v.z;
        norBuf[3*idx2+2] += u.x * v.y - u.y * v.x;
        
        u = v1 - v3;
        v = v2 - v3;
        norBuf[3*idx3+0] += u.y * v.z - u.z * v.y;
        norBuf[3*idx3+1] += u.z * v.x - u.x * v.z;
        norBuf[3*idx3+2] += u.x * v.y - u.y * v.x;
    }
    
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

bool installShaders(const string &vShaderName, const string &fShaderName)
{
    GLint rc;
    
    // Create shader handles
    GLuint VS = glCreateShader(GL_VERTEX_SHADER);
    GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read shader sources
    const char *vshader = GLSL::textFileRead(vShaderName.c_str());
    const char *fshader = GLSL::textFileRead(fShaderName.c_str());
    glShaderSource(VS, 1, &vshader, NULL);
    glShaderSource(FS, 1, &fshader, NULL);
    
    // Compile vertex shader
    glCompileShader(VS);
    GLSL::printError();
    glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
    GLSL::printShaderInfoLog(VS);
    if(!rc) {
        printf("Error compiling vertex shader %s\n", vShaderName.c_str());
        return false;
    }
    
    // Compile fragment shader
    glCompileShader(FS);
    GLSL::printError();
    glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
    GLSL::printShaderInfoLog(FS);
    if(!rc) {
        printf("Error compiling fragment shader %s\n", fShaderName.c_str());
        return false;
    }
    
    // Create the program and link
	   ShadeProg = glCreateProgram();
	   glAttachShader(ShadeProg, VS);
	   glAttachShader(ShadeProg, FS);
	   glLinkProgram(ShadeProg);
    
	   GLSL::printError();
	   glGetProgramiv(ShadeProg, GL_LINK_STATUS, &rc);
	   GLSL::printProgramInfoLog(ShadeProg);
	   if(!rc) {
           printf("Error linking shaders %s and %s\n", vShaderName.c_str(), fShaderName.c_str());
           return false;
       }
    
    /* get handles to attribute data */
    h_aPosition = GLSL::getAttribLocation(ShadeProg, "aPosition");
    h_aNormal = GLSL::getAttribLocation(ShadeProg, "aNormal");
    h_uProjMatrix = GLSL::getUniformLocation(ShadeProg, "uProjMatrix");
    h_uViewMatrix = GLSL::getUniformLocation(ShadeProg, "uViewMatrix");
    h_uModelMatrix = GLSL::getUniformLocation(ShadeProg, "uModelMatrix");
    h_uLightPos = GLSL::getUniformLocation(ShadeProg, "uLightPos");
    h_uMatAmb = GLSL::getUniformLocation(ShadeProg, "UaColor");
    h_uMatDif = GLSL::getUniformLocation(ShadeProg, "UdColor");
    h_uMatSpec = GLSL::getUniformLocation(ShadeProg, "UsColor");
    h_uMatShine = GLSL::getUniformLocation(ShadeProg, "Ushine");
    h_uShadeM = GLSL::getUniformLocation(ShadeProg, "uShadeModel");
    h_uShowNorm = GLSL::getUniformLocation(ShadeProg, "uShowNormal");
    
    assert(glGetError() == GL_NO_ERROR);
    return true;
}


void drawGL()
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use our GLSL program
    glUseProgram(ShadeProg);
    
    SetProjectionMatrix();
    SetView();
    
    SetModel(1);
    SetMaterial(g_mat_id);
    glUniform3f(h_uLightPos, g_light.x, g_light.y, g_light.z);
    glUniform1i(h_uShadeM, g_SM);
    glUniform1i(h_uShowNorm, g_SN);
    
    // Enable and bind position array for drawing
    GLSL::enableVertexAttribArray(h_aPosition);
    glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
    glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Enable and bind normal array for drawing
    GLSL::enableVertexAttribArray(h_aNormal);
    glBindBuffer(GL_ARRAY_BUFFER, norBufObj);
    glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Bind index array for drawing
    int nIndices = (int)shapes[0].mesh.indices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObj);
    
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
    
    SetModel(2);
    
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
    
    SetModel(3);
    
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
    
    GLSL::disableVertexAttribArray(h_aPosition);
    GLSL::disableVertexAttribArray(h_aNormal);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // Disable and unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);
    
}


void window_size_callback(GLFWwindow* window, int w, int h){
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    g_width = w;
    g_height = h;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        g_angle += 10;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        g_angle -= 10;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        g_x_angle += 10;
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        g_x_angle -= 10;
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        g_mat_id = (g_mat_id+1)%4;
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        g_SM = !g_SM;
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
        g_SN = !g_SN;
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        g_light.x += 0.25;
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        g_light.x -= 0.25;
}

int main(int argc, char **argv)
{
    
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    // Open a window and create its OpenGL context
    g_width = 1024;
    g_height = 768;
    window = glfwCreateWindow( g_width, g_height, "P3 - shading", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    loadShapes("bunny.obj");
//    loadShapes("sphere.obj");
//    loadShapes("cube.obj");
    initGL();
    installShaders("vert.glsl", "frag.glsl");
    
    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
    
    
    do{
        drawGL();
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}