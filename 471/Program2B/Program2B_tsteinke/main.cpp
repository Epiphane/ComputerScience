// Program 2B
// Thomas Steinke (tsteinke)
// CPE 471
// Do you want to build a snowman?

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "GLSL.h"
#include "tiny_obj_loader.h"

#define NUM_POINTS 40

GLFWwindow* window;
using namespace std;

vector<tinyobj::shape_t> shapes;
vector<tinyobj::material_t> materials;
GLuint prog;
GLuint posBufObj = 0;
GLuint norBufObj = 0;
GLuint indBufObj = 0;
GLint aPos = 0;
GLint aNor = 0;
GLint uMaterial = 0;
GLint uMV = 0;
GLint uP = 0;

GLuint snowProg;
GLuint snowBufObj = 0;
GLint aSnowPos = 0;
GLint uTime = 0;
GLint uSpeed = 0;

int LAB5_DEBUG = 0;
int width = 1;
int height = 1;

float rotation = 0.0f;
int r_time = 0;
bool keysDown[] = { false, false };

void window_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    bool down = action == GLFW_PRESS || action == GLFW_REPEAT;
    if(key == GLFW_KEY_A)
        keysDown[0] = down;
    else if(key == GLFW_KEY_D)
        keysDown[1] = down;
}

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
	glClearColor(0.7f, 0.7f, 0.9f, 1.0f);
    // Enable Z-buffer test
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
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
    
    // Send the snow array to the GPU
    GLfloat *points = (GLfloat *)calloc(NUM_POINTS * 3, sizeof(GLfloat));
    
    for(int i = 0; i < NUM_POINTS; i ++) {
        points[3*i+0] = float(rand() % 60) / 30;// - 1.0f;
        points[3*i+1] = float(rand() % 60) / 60;// - 1.0f;
        points[3*1+2] = 0.1f;
    }
    
    glGenBuffers(1, &snowBufObj);
    glBindBuffer(GL_ARRAY_BUFFER, snowBufObj);
    glBufferData(GL_ARRAY_BUFFER, 3* NUM_POINTS * sizeof(GLfloat), points, GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
}

bool installShaders(const string &vShaderName, const string &fShaderName,
                    const string &vSnowShaderName, const string &fSnowShaderName)
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
	prog = glCreateProgram();
	glAttachShader(prog, VS);
	glAttachShader(prog, FS);
	glLinkProgram(prog);
	GLSL::printError();
	glGetProgramiv(prog, GL_LINK_STATUS, &rc);
	GLSL::printProgramInfoLog(prog);
	if(!rc) {
		printf("Error linking shaders %s and %s\n", vShaderName.c_str(), fShaderName.c_str());
		return false;
	}
	
	// Set up the shader variables
	aPos = GLSL::getAttribLocation(prog, "aPos");
	aNor = GLSL::getAttribLocation(prog, "aNor");
	uMV = GLSL::getUniformLocation(prog, "MV");
	uP = GLSL::getUniformLocation(prog, "P");
    uMaterial = GLSL::getUniformLocation(prog, "material");
	
	assert(glGetError() == GL_NO_ERROR);
    
    // Create shader handles
    VS = glCreateShader(GL_VERTEX_SHADER);
    FS = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read shader sources
    const char *vsnowshader = GLSL::textFileRead(vSnowShaderName.c_str());
    const char *fsnowshader = GLSL::textFileRead(fSnowShaderName.c_str());
    glShaderSource(VS, 1, &vsnowshader, NULL);
    glShaderSource(FS, 1, &fsnowshader, NULL);
    
    // Compile vertex shader
    glCompileShader(VS);
    GLSL::printError();
    glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
    GLSL::printShaderInfoLog(VS);
    if(!rc) {
        printf("Error compiling vertex shader %s\n", vSnowShaderName.c_str());
        return false;
    }
    
    // Compile fragment shader
    glCompileShader(FS);
    GLSL::printError();
    glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
    GLSL::printShaderInfoLog(FS);
    if(!rc) {
        printf("Error compiling fragment shader %s\n", fSnowShaderName.c_str());
        return false;
    }
    
    // Create the program and link
    snowProg = glCreateProgram();
    glAttachShader(snowProg, VS);
    glAttachShader(snowProg, FS);
    glLinkProgram(snowProg);
    GLSL::printError();
    glGetProgramiv(snowProg, GL_LINK_STATUS, &rc);
    GLSL::printProgramInfoLog(prog);
    if(!rc) {
        printf("Error linking shaders %s and %s\n", vSnowShaderName.c_str(), fSnowShaderName.c_str());
        return false;
    }
    
    // Set up the shader variables
    aSnowPos = GLSL::getAttribLocation(snowProg, "aPos");
    uTime = GLSL::getUniformLocation(snowProg, "time");
    uSpeed = GLSL::getUniformLocation(snowProg, "speed");
    
    assert(glGetError() == GL_NO_ERROR);
    
	return true;
}

void drawSnowman(long nIndices, GLfloat *matrix) {
    glUniformMatrix4fv(uMV, 1, GL_FALSE, matrix);
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
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
    glm::mat4 perspective = glm::perspective(45.0f, (float)width/height, 0.01f, 100.0f);
    glUniformMatrix4fv(uP, 1, GL_FALSE, &perspective[0][0]);
    
    // Draw snowglobe
    glm::mat4 Snowglobe = glm::translate(0.0f, 0.0f, -2.0f) *
                          glm::rotate(rotation, 0.0f, 1.0f, 0.0f);
    
    glUniform1i(uMaterial, 0);
    glUniformMatrix4fv(uMV, 1, GL_FALSE, &Snowglobe[0][0]);
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
	
    // Draw snowman
    glm::mat4 SnowmanBody = Snowglobe * glm::scale(0.2f, 0.17f, 0.2f);
    glm::mat4 SnowmanButt = SnowmanBody * glm::translate(0.0f, -1.4f, 0.0f) * glm::scale(1.3f, 1.3f, 1.3f);
    glm::mat4 SnowmanHead = SnowmanBody * glm::translate(0.0f, 1.2f, 0.0f) * glm::scale(0.8f, 0.8f, 0.8f);
    
    glUniform1i(uMaterial, 1);
    drawSnowman(nIndices, &SnowmanBody[0][0]);
    drawSnowman(nIndices, &SnowmanButt[0][0]);
    drawSnowman(nIndices, &SnowmanHead[0][0]);
    
    glm::mat4 SnowmanEye1 = SnowmanHead * glm::translate(-0.45f, 0.0f, 0.8f) * glm::scale(0.1f, 0.1f, 0.1f);
    glm::mat4 SnowmanEye2 = SnowmanHead * glm::translate(0.45f, 0.0f, 0.8f) * glm::scale(0.1f, 0.1f, 0.1f);
    
    float rot = r_time % 60;
    if(rot > 30) rot = 60 - rot;
    glm::mat4 armDance = glm::rotate(15.0f, 0.0f, 0.0f, 1.0f) * glm::rotate(rot * 5 - 130.0f, 0.0f, 1.0f, 0.0f);
    glm::mat4 SnowmanArm1 = SnowmanBody * glm::translate(0.6f, 0.0f, 0.3f) * armDance * glm::translate(1.0f, 0.0f, 0.0f) * glm::scale(1.0f, 0.1f, 0.1f);
    glm::mat4 SnowmanArm2 = SnowmanBody * glm::translate(-0.6f, 0.3f, 0.3f) * glm::rotate(180.0f, 0.0f, 0.0f, 1.0f) * armDance * glm::translate(1.0f, 0.0f, 0.0f) * glm::scale(1.0f, 0.1f, 0.1f);
    
    glUniform1i(uMaterial, 2);
    drawSnowman(nIndices, &SnowmanEye1[0][0]);
    drawSnowman(nIndices, &SnowmanEye2[0][0]);
    drawSnowman(nIndices, &SnowmanArm1[0][0]);
    drawSnowman(nIndices, &SnowmanArm2[0][0]);
    
    // Disable and unbind
	GLSL::disableVertexAttribArray(aPos);
    GLSL::disableVertexAttribArray(aNor);
    
    // Snow!!
    glUseProgram(snowProg);
    
    glBindBuffer(GL_ARRAY_BUFFER, snowBufObj);
    
    GLSL::enableVertexAttribArray(aSnowPos);
    glVertexAttribPointer(aSnowPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glUniform1f(uTime, (float) r_time);
    glUniform1f(uSpeed, 0.01f);
    
    glDrawArrays(GL_POINTS, 0, NUM_POINTS);
    
    GLSL::disableVertexAttribArray(aSnowPos);
    
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
	assert(glGetError() == GL_NO_ERROR);
	
}

void reshapeGL(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	width = w;
	height = h;
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
   window = glfwCreateWindow( 768, 768, "Program 2B", NULL, NULL);
   if( window == NULL ){
      fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, window_key_callback);

   // Ensure we can capture the escape key being pressed below
   glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	loadShapes("sphere.obj");

   std::cout << "loaded the object " << endl;

   // Initialize glad
   if (!gladLoadGL()) {
      fprintf(stderr, "Unable to initialize glad");
      glfwDestroyWindow(window);
      glfwTerminate();
      return 1;
   }

	initGL();
	installShaders("vert.glsl", "frag.glsl", "vert_snow.glsl", "frag_snow.glsl");

    do{
        if(keysDown[0])
            rotation += 1.0f;
        if(keysDown[1])
            rotation -= 1.0f;
        
        while(rotation < 0)
            rotation += 360;
        while(rotation > 360)
            rotation -= 360;
        
        r_time += 1;
        
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
