/*
  CPE 471 Lab 1 
  Base code for Rasterizer
  Example code using B. Somers' image code - writes out a sample tga
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include "Image.h"
#include "types.h"
#include "lodepng.h"

#include "cage.cpp"

typedef struct Point {
    int x, y, z;
} Point;

void decodeWithState(const char* filename);

int main(void) {
  
  Point points[3];
  printf("Enter the points of the triangle (x y z): ");
  
  int *val = (int *)points;
  for(int i = 0; i < 9; i ++) {
//    scanf("%d", val++);
    *val++ = 50 * (i + 1);
  }
  
  int min_x = std::min({points[0].x, points[1].x, points[2].x});
  int min_y = std::min({points[0].y, points[1].y, points[2].y});
  int max_x = std::max({points[0].x, points[1].x, points[2].x});
  int max_y = std::max({points[0].y, points[1].y, points[2].y});
  
  // make a color
  color_t clr;

  clr.r = 0.5;
  clr.g = 0.5;
  clr.b = 0.9;

  // make a 640x480 image (allocates buffer on the heap)
  Image img(640, 480);

  // set a square to be the color above
  for (int i = min_x; i <= max_x; i ++) {
    for (int j = min_y; j <= max_y; j ++) {
      clr.r = gimp_image.pixel_data[((i % gimp_image.width) + (j % gimp_image.height) * gimp_image.width) * 3 + 0];
      clr.g = gimp_image.pixel_data[((i % gimp_image.width) + (j % gimp_image.height) * gimp_image.width) * 3 + 1];
      clr.b = gimp_image.pixel_data[((i % gimp_image.width) + (j % gimp_image.height) * gimp_image.width) * 3 + 2];
      
      clr.r = (float) clr.r / 255;
      clr.g = (float) clr.g / 255;
      clr.b = (float) clr.b / 255;
      
      if(i == min_x && j == min_y) {
        std::cout << clr.r << std::endl;
        std::cout << clr.g << std::endl;
        std::cout << clr.b << std::endl;
      }
      
      img.pixel(i, j, clr);
    }
  }

  // write the targa file to disk
  img.WriteTga((char *)"awesome.tga", true); 
  // true to scale to max color, false to clamp to 1.0

}
