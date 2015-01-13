#include <iostream>
#include <stdio.h>
#include <cassert>
#include "tiny_obj_loader.h"
#include "Image.h"

class Point {
public:
    Point() {};
    Point(float _x, float _y, float _z) : xyz{_x, _y, _z} {};
    
    void transformToScreen(int width, int height) {
        xyz[0] = (int) (xyz[0] + 1) * (width - 1) / 2;
        xyz[1] = (int) (xyz[1] + 1) * (height - 1) / 2;
    }

    float xyz[3];
    float getX() { return xyz[0]; }
    float getY() { return xyz[1]; }
    float getZ() { return xyz[2]; }
};

std::ostream &operator<< (std::ostream &out, Point point) {
    out << "{" << point.xyz[0] << ", " << point.xyz[1] << ", " << point.xyz[2] << "}";
    return out;
}

void decodeWithState(const char* filename);
void drawTriangle(tinyobj::mesh_t mesh, size_t face, Image *img);
int triangleArea(Point p1, Point p2, Point p3);

int main( int argc, const char* argv[] )
{
	std::string inputfile = "cube.obj";
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, inputfile.c_str());

	if (!err.empty()) {
		std::cerr << err << std::endl;
		exit(1);
	}

	std::cout << "# of shapes    : " << shapes.size() << std::endl;
	for (size_t i = 0; i < shapes.size(); i++) {
		printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
		assert((shapes[i].mesh.indices.size() % 3) == 0);
		for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
			printf("  triangle[%ld] = %d, %d, %d\n", f, shapes[i].mesh.indices[3*f+0], shapes[i].mesh.indices[3*f+1], shapes[i].mesh.indices[3*f+2]);
		}

		printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
		assert((shapes[i].mesh.positions.size() % 3) == 0);
		for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
			printf("  vertex[%ld] = (%f, %f, %f)\n", v,
				shapes[i].mesh.positions[3*v+0],
				shapes[i].mesh.positions[3*v+1],
				shapes[i].mesh.positions[3*v+2]);
		}
	}

	// make a color
	color_t clr;
	clr.r = 0.5;
	clr.g = 0.5;
	clr.b = 0.9;
 	// make a 640x480 image (allocates buffer on the heap)
	Image img(400, 400);
    
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
            drawTriangle(shapes[i].mesh, f, &img);
        }
    }
    
	// write the targa file to disk
	// true to scale to max color, false to clamp to 1.0
	img.WriteTga((char *)"awesome.tga", true); 
}

void drawTriangle(tinyobj::mesh_t mesh, size_t face, Image *img) {
    std::cout << std::endl;
    std::vector<Point> points(3);
    for(int i = 0; i < 3; i ++) {
        for(int j = 0; j < 3; j ++) {
            points[i].xyz[j] = mesh.positions[3 * mesh.indices[3 * face + i] + j];
        }
        points[i].transformToScreen(img->width(), img->height());
        std::cout << points[i] << std::endl;
    }
    
    float area = (float) triangleArea(points[0], points[1], points[2]);
    
    int min_x = points[0].getX(), max_x = points[0].getX();
    int min_y = points[0].getY(), max_y = points[0].getY();
    for(int i = 1; i < 3; i ++) {
        if(points[i].getX() < min_x) min_x = points[i].getX();
        if(points[i].getX() > max_x) max_x = points[i].getX();
        if(points[i].getY() < min_y) min_y = points[i].getY();
        if(points[i].getY() > max_y) max_y = points[i].getY();
    }
    
    // make a color
    color_t vertexColors[3], clr;
    vertexColors[0].r = 1;
    vertexColors[0].g = 0;
    vertexColors[0].b = 0;
    vertexColors[1].r = 0;
    vertexColors[1].g = 1;
    vertexColors[1].b = 0;
    vertexColors[2].r = 0;
    vertexColors[2].g = 0;
    vertexColors[2].b = 1;
    
    Point point;
    for (int i = min_x; i <= max_x; i ++) {
        point.xyz[0] = i;
        for (int j = min_y; j <= max_y; j ++) {
            point.xyz[1] = j;
            
            float vertexProportions[3];
            vertexProportions[0] = triangleArea(point, points[1], points[2]);
            if(vertexProportions[0] < 0) continue;
            vertexProportions[1] = triangleArea(point, points[2], points[0]);
            if(vertexProportions[1] < 0) continue;
            vertexProportions[2] = triangleArea(point, points[0], points[1]);
            if(vertexProportions[2] < 0) continue;
            
            clr.r = clr.g = clr.b = 0;
            for(int p = 0; p < 3; p ++) {
                clr.r += vertexProportions[p] * vertexColors[p].r / area;
                clr.g += vertexProportions[p] * vertexColors[p].g / area;
                clr.b += vertexProportions[p] * vertexColors[p].b / area;
            }
            
            img->pixel(i, j, clr);
        }
    }
}

int triangleArea(Point p1, Point p2, Point p3) {
    return (p2.getX() - p1.getX()) * (p3.getY() - p1.getY())
         - (p3.getX() - p1.getX()) * (p2.getY() - p1.getY());
}
