#ifndef ____shader__
#define ____shader__

#include <string>
#include <glm/glm.hpp>

const int VERTEX_BUFFER = 0;
const int COLOR_BUFFER = 1;
const int NORMAL_BUFFER = 2;
const int INDICES_BUFFER = 3;

typedef unsigned int Material;
const Material MATERIAL_RUBBER = 0;
const Material MATERIAL_METAL = 1;

void shaders_init();

unsigned int LoadShaders(const char *vertFilePath, const char *geomFilePath, const char *fragFilePath);

class Renderer;

// Shader programs

typedef struct Program {
    int programID;
    Renderer *(* create)();
    void(* bufferData)(Renderer *p, int type, long elements, void *data);
    void(* render)(Renderer *p, glm::mat4 Model);
} Program;

extern Program *Program3D;

// Renderers

class Renderer {
private:
    unsigned int *buffers;
    unsigned int elements;
    
    int numBuffers;
    
public:
    Renderer(int numBuffers);
    ~Renderer();
    
    unsigned int getNumElements() { return elements; }
    void setNumElements(unsigned long num) { elements = num; }
    
    unsigned int getBuffer(int num) { return buffers[num]; }
    
    /* Rendering functions */
    Program *program;
    void bufferData(int type, long num, void *data) { program->bufferData(this, type, num, data); }
    void render(glm::mat4 Model) { program->render(this, Model); }
    
    static void pushMatrix(glm::mat4 matrix);
    static void popMatrix();
    
    Material mat;
    glm::vec3 bend;
};

#endif
