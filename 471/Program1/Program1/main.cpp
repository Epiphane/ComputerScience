#include <iostream>
#include <stdio.h>
#include <cassert>
#include "tiny_obj_loader.h"
#include "Image.h"

#include "cage.cpp"

class Point {
public:
    Point() {};

    double xyz[3];
    double getX() { return xyz[0]; }
    double getY() { return xyz[1]; }
    double getZ() { return xyz[2]; }
};

class Camera {
public:
    void transformPointToScreen(Point *point) {
        point->xyz[0] = (point->xyz[0] - left) * min_size / 2;
        point->xyz[1] = (point->xyz[1] - bottom) * min_size / 2;
        point->xyz[2] = (point->xyz[2] - back) / (front - back);
    }
    
    void setWindowSize(int width, int height) {
        if(width > height) {
            right = (double) width / height;
            top = 1;
            min_size = height - 1;
        }
        else {
            right = 1;
            top = (double) height / width;
            min_size = width - 1;
        }
        left = right * -1;
        bottom = top * -1;
        
        front = 1;
        back = -1;
        
        this->height = height;
        this->width = width;
    }
    
    int min_size, height, width;
    double left, right, top, bottom;
    double front, back;
};

std::ostream &operator<< (std::ostream &out, Point point) {
    out << "{" << point.xyz[0] << ", " << point.xyz[1] << ", " << point.xyz[2] << "}";
    return out;
}

void decodeWithState(const char* filename);
void drawTriangle(tinyobj::mesh_t mesh, size_t face, Camera *cam, Image *img, int coloring, double *zBuffer);
int triangleArea(Point p1, Point p2, Point p3);
void resize_obj(std::vector<tinyobj::shape_t> &shapes);

int main( int argc, const char* argv[] )
{
    if(argc < 2) {
        std::cerr << "Please provide an input file argument" << std::endl;
        exit(1);
    }
    
	std::string inputfile = argv[1];
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, inputfile.c_str());

	if (!err.empty()) {
		std::cerr << err << std::endl;
		exit(1);
	}

    Camera *cam = new Camera();
    
    resize_obj(shapes);

    // make a 640x480 image (allocates buffer on the heap)
    int coloring, w_width, w_height;
    std::cout << "Enter window size (width height): ";
    std::cin >> w_width >> w_height;
    std::cout << "Enter gradient type (1: z-depth, 2: x-value, 3: both)";
    std::cin >> coloring;
    
    Image img(w_width, w_height);
    double zBuffer[img.width() * img.height()];
    cam->setWindowSize(img.width(), img.height());
    
    color_t clr;
    for (int i = 0; i < img.width(); i ++) {
        for(int j = 0; j < img.height(); j ++) {
            clr.r = gimp_image.pixel_data[((i % gimp_image.width) + (j % gimp_image.height) * gimp_image.width) * 3 + 0];
            clr.g = gimp_image.pixel_data[((i % gimp_image.width) + (j % gimp_image.height) * gimp_image.width) * 3 + 1];
            clr.b = gimp_image.pixel_data[((i % gimp_image.width) + (j % gimp_image.height) * gimp_image.width) * 3 + 2];
            
            clr.r = (float) clr.r / 255;
            clr.g = (float) clr.g / 255;
            clr.b = (float) clr.b / 255;
            
            img.pixel(i, img.height() - j - 1, clr);
        }
    }
    
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
            drawTriangle(shapes[i].mesh, f, cam, &img, coloring, zBuffer);
        }
    }
    
	// write the targa file to disk
	// true to scale to max color, false to clamp to 1.0
	img.WriteTga((char *)"awesome.tga", true); 
}

void drawTriangle(tinyobj::mesh_t mesh, size_t face, Camera *cam, Image *img, int coloring, double *zBuffer) {
    std::vector<Point> points(3);
    for(int i = 0; i < 3; i ++) {
        for(int j = 0; j < 3; j ++) {
            points[i].xyz[j] = mesh.positions[3 * mesh.indices[3 * face + i] + j];
        }
        cam->transformPointToScreen(&points[i]);
    }
    
    double area = (double) triangleArea(points[0], points[1], points[2]);
    
    int min_x = points[0].getX(), max_x = points[0].getX();
    int min_y = points[0].getY(), max_y = points[0].getY();
    for(int i = 1; i < 3; i ++) {
        if(points[i].getX() < min_x) min_x = points[i].getX();
        if(points[i].getX() > max_x) max_x = points[i].getX();
        if(points[i].getY() < min_y) min_y = points[i].getY();
        if(points[i].getY() > max_y) max_y = points[i].getY();
    }
    
    // make a color
    color_t clr;
    
    Point point;
    for (int i = min_x; i <= max_x; i ++) {
        point.xyz[0] = i;
        for (int j = min_y; j <= max_y; j ++) {
            point.xyz[1] = j;
            
            double vertexProportions[3];
            vertexProportions[0] = triangleArea(point, points[1], points[2]);
            if(vertexProportions[0] < 0) continue;
            vertexProportions[1] = triangleArea(point, points[2], points[0]);
            if(vertexProportions[1] < 0) continue;
            vertexProportions[2] = triangleArea(point, points[0], points[1]);
            if(vertexProportions[2] < 0) continue;
            
            double z = 0;
            for(int p = 0; p < 3; p ++) {
                z += vertexProportions[p] * points[p].xyz[2] / area;
            }
            
            clr.r = clr.g = clr.b = 0;
            if(coloring % 2 == 1) {
                clr.r = z;
            }
            if(coloring > 1) {
                clr.b = (double) i / img->width();
                clr.g = 1.0 - clr.b;
            }
            
            if(zBuffer[i + j * img->width()] < z) {
                zBuffer[i + j * img->width()] = z;
                img->pixel(i, j, clr);
            }
        }
    }
}

int triangleArea(Point p1, Point p2, Point p3) {
    return (p2.getX() - p1.getX()) * (p3.getY() - p1.getY())
         - (p3.getX() - p1.getX()) * (p2.getY() - p1.getY());
}

//Given a vector of shapes which has already been read from an obj file
// resize all vertices to the range [-1, 1]
void resize_obj(std::vector<tinyobj::shape_t> &shapes) {
    double minX, minY, minZ;
    double maxX, maxY, maxZ;
    double scaleX, scaleY, scaleZ;
    double shiftX, shiftY, shiftZ;
    double epsilon = 0.001;
    
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
    double maxExtent, xExtent, yExtent, zExtent;
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