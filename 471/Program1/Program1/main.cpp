#include <iostream>
#include <stdio.h>
#include <cassert>
#include "tiny_obj_loader.h"
#include "Image.h"

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
	// set a square to be the color above
	for (int i=50; i < 100; i++) {
		for (int j=50; j < 100; j++) {
			img.pixel(i, j, clr);
		}
	}
	// write the targa file to disk
	// true to scale to max color, false to clamp to 1.0
	img.WriteTga((char *)"awesome.tga", true); 
}
