#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <stack>
#include <glm/ext.hpp>
using namespace std;

#include "renderer.h"
#include "main.h"

// Uniform and attribute locations
#define A_POSITION 0
#define A_COLOR 1
#define A_NORMAL 2

// Shader loader used by programs
GLuint LoadShaders(const char *vertFilePath, const char *geomFilePath, const char *fragFilePath);

Program *Program3D = NULL;
GLuint Program3D_uWinScale, Program3D_uProj, Program3D_uModel, Program3D_uView;
GLuint Program3D_uLightPos, Program3D_uAColor, Program3D_uDColor;
GLuint Program3D_uSColor, Program3D_uShine;
GLuint Program3D_aPosition, Program3D_aNormal;

GLuint *createBuffers(int num) {
    GLuint *buffers = (GLuint *) malloc(sizeof(GLuint) * num);
    
    glGenBuffers(num, buffers);
    
    return buffers;
}

// Program constructor/destructor
Renderer::Renderer(int numBuffers) : numBuffers(numBuffers), buffers(createBuffers(numBuffers)), elements(0) {};
Renderer::~Renderer() {
    glDeleteBuffers(numBuffers, buffers);
}

glm::mat4 Projection, View;
glm::mat4 currentMVP;
std::stack<glm::mat4> MatrixStack;

void Renderer::setView(glm::mat4 view) {
    View = view;
}

void Renderer::pushMatrix(glm::mat4 matrix) {
    MatrixStack.push(currentMVP);
    
    currentMVP *= matrix;
}

void Renderer::popMatrix() {
    currentMVP = MatrixStack.top();
    
    MatrixStack.pop();
}

void setMaterial(Material mat, GLuint uDColor, GLuint uSColor, GLuint uAColor, GLuint uShine) {
    switch(mat) {
        case MATERIAL_METAL:
            glUniform3f(Program3D_uAColor, 0.15, 0.15, 0.15);
            glUniform3f(Program3D_uDColor, 0.4, 0.4, 0.4);
            glUniform3f(Program3D_uSColor, 0.14, 0.14, 0.14);
            glUniform1f(Program3D_uShine, 76.8);
            break;
        case MATERIAL_RUBBER:
            glUniform3f(Program3D_uAColor, 0, 0, 0);//2, 0.02, 0.02);
            glUniform3f(Program3D_uDColor, 0, 0, 0);//.01, 0.01, 0.01);
            glUniform3f(Program3D_uSColor, 0.4, 0.4, 0.4);
            glUniform1f(Program3D_uShine, -100.0);
            break;
        default:
            std::cerr << "Error: Material " << mat << " not found" << std::endl;
    }
}

void setUniforms(GLuint uWinScale, GLuint uPerspective, GLuint uView, GLuint uModel, glm::mat4 MVP) {
    if(w_width > w_height)
        glUniform2f(uWinScale, (float) w_height / w_width, 1);
    else
        glUniform2f(uWinScale, 1, (float) w_width / w_height);
    
    // Send camera projection
    MVP = currentMVP * MVP;
    
    glUniformMatrix4fv(uModel, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(uView, 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(uPerspective, 1, GL_FALSE, &Projection[0][0]);
}

void shaders_init() {
    Projection = glm::perspective(45.0f, (float) w_width / w_height, 0.01f, 100.0f);
    currentMVP = glm::mat4(1.0f);
    MatrixStack.empty();
    
    // ----------------- 3D MODEL SHADER -------------------------
    Program3D = (Program *) malloc(sizeof(Program));
    
    Program3D->programID = LoadShaders("shaders/3DVertex.glsl", "shaders/3DGeometry.glsl", "shaders/3DFragment.glsl");
    Program3D_uWinScale = glGetUniformLocation(Program3D->programID, "windowScale");
    Program3D_uProj = glGetUniformLocation(Program3D->programID, "uProjMatrix");
    Program3D_uModel = glGetUniformLocation(Program3D->programID, "uModelMatrix");
    Program3D_uView = glGetUniformLocation(Program3D->programID, "uViewMatrix");
    Program3D_uAColor = glGetUniformLocation(Program3D->programID, "UaColor");
    Program3D_uDColor = glGetUniformLocation(Program3D->programID, "UdColor");
    Program3D_uSColor = glGetUniformLocation(Program3D->programID, "UsColor");
    Program3D_uLightPos = glGetUniformLocation(Program3D->programID, "uLightPos");
    Program3D_uShine = glGetUniformLocation(Program3D->programID, "uShine");
    Program3D_aNormal = glGetAttribLocation(Program3D->programID, "aNormal");
    Program3D_aPosition = glGetAttribLocation(Program3D->programID, "aPosition");
    
    Program3D->create = [] () {
        Renderer *prog = new Renderer(3);
        prog->program = Program3D;
        prog->mat = MATERIAL_METAL;
        
        return (Renderer *)prog;
    };
    
    Program3D->bufferData = [] (Renderer *p, int type, long num, void *data) {
        size_t scalar;
        GLuint bufType;
        
        if(type == VERTEX_BUFFER) {
            bufType = GL_ARRAY_BUFFER;
            scalar = sizeof(float);
            
            glBindBuffer(bufType, p->getBuffer(0));
        }
        else if(type == NORMAL_BUFFER) {
            bufType = GL_ARRAY_BUFFER;
            scalar = sizeof(float);
            
            glBindBuffer(bufType, p->getBuffer(1));
        }
        else if(type == INDICES_BUFFER) {
            bufType = GL_ELEMENT_ARRAY_BUFFER;
            scalar = sizeof(unsigned int);
            
            glBindBuffer(bufType, p->getBuffer(2));
        }
        
        glBufferData(bufType, scalar * num, data, GL_STATIC_DRAW);
    };
    
    Program3D->render = [] (Renderer *p, glm::mat4 Model) {
        glUseProgram(Program3D->programID);
        
        // Send window scale
        setUniforms(Program3D_uWinScale, Program3D_uProj, Program3D_uView, Program3D_uModel, Model);
        
        setMaterial(p->mat, Program3D_uDColor, Program3D_uSColor, Program3D_uAColor, Program3D_uShine);
        
        glUniform3f(Program3D_uLightPos, 5, 2, 33);
        
        // Bind attributes...
        // XYZ Position
        glEnableVertexAttribArray(Program3D_aPosition);
        glBindBuffer(GL_ARRAY_BUFFER, p->getBuffer(0));
        glVertexAttribPointer(Program3D_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        // integer color
        glEnableVertexAttribArray(Program3D_aNormal);
        glBindBuffer(GL_ARRAY_BUFFER, p->getBuffer(1));
        glVertexAttribPointer(Program3D_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->getBuffer(2));
        glDrawElements(GL_TRIANGLES, p->getNumElements(), GL_UNSIGNED_INT, 0);
        
        if(p->getNumElements() == 0)
            std::cout << "WARNING: Rendering a sprite with 0 elements" << std::endl;
        // Cleanup
        glDisableVertexAttribArray(Program3D_aPosition);
        glDisableVertexAttribArray(Program3D_aNormal);
    };
}

// ----------------- LOAD SHADERS -----------------------------

GLuint compileShader(const char *filePath, GLenum shaderType) {
    GLuint shaderID = glCreateShader(shaderType);
    
    // Read the Shader code from the file
    string shaderCode;
    ifstream shaderStream(filePath, ios::in);
    if(shaderStream.is_open()){
        string line = "";
        while(getline(shaderStream, line))
            shaderCode += "\n" + line;
        shaderStream.close();
    }
    
    GLint result = GL_FALSE;
    int infoLogLength;
    
    // Compile Shader
    printf("Compiling shader : %s\n", filePath);
    const char *sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer , NULL);
    glCompileShader(shaderID);
    
    // Check Shader
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0){
        vector<char> errorMessage(infoLogLength+1);
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, &errorMessage[0]);
        printf("%s\n", &errorMessage[0]);
    }
    
    return shaderID;
}

GLuint LoadShaders(const char *vertFilePath, const char *geomFilePath, const char *fragFilePath) {
    GLint result = GL_FALSE;
    int infoLogLength;
    
    // Create shaders
    GLuint vertexShader = compileShader(vertFilePath, GL_VERTEX_SHADER);
//    GLuint geomShader = compileShader(geomFilePath, GL_GEOMETRY_SHADER);
    GLuint fragShader = compileShader(fragFilePath, GL_FRAGMENT_SHADER);
    
    // Link the program
    printf("Linking program\n");
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    //    glAttachShader(programID, geomShader);
    glAttachShader(programID, fragShader);
    glLinkProgram(programID);
    
    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0){
        vector<char> errorMessage(infoLogLength+1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &errorMessage[0]);
        printf("%s\n", &errorMessage[0]);
        
        exit(1);
    }
    
    glDeleteShader(vertexShader);
    //    glDeleteShader(geomShader);
    glDeleteShader(fragShader);
    
    return programID;
}


