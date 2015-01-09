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

#include "pizza.cpp"

typedef struct Point {
    int x, y, z;
} Point;

void decodeWithState(const char* filename);
void drawTriangle(Point *points, Image *img);
int triangleArea(Point p1, Point p2, Point p3);

int main(void) {
  printf("Enter the points of the triangle (x y z): ");
  
  Point points[3];
  int *val = (int *)points;
  for(int i = 0; i < 9; i ++) {
//    scanf("%d", val++);
  }
  points[0].x = 100;
  points[0].y = 100;
  points[1].x = 400;
  points[1].y = 150;
  points[2].x = 250;
  points[2].y = 300;

  // make a 640x480 image (allocates buffer on the heap)
  Image img(640, 480);

  drawTriangle(points, &img);
  
  // write the targa file to disk
  img.WriteTga((char *)"awesome.tga", true); 
  // true to scale to max color, false to clamp to 1.0

}

void drawTriangle(Point *points, Image *img) {
  float area = (float) triangleArea(points[0], points[1], points[2]);
  
  int min_x = points[0].x, max_x = points[0].x;
  int min_y = points[0].y, max_y = points[0].y;
  for(int i = 1; i < 3; i ++) {
    if(points[i].x < min_x) min_x = points[i].x;
    if(points[i].x > max_x) max_x = points[i].x;
    if(points[i].y < min_y) min_y = points[i].y;
    if(points[i].y > max_y) max_y = points[i].y;
  }
  
  int width = max_x - min_x;
  int height = max_y - min_y;
  
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
  
  int vertices[3][2];
  float uv[2];
  vertices[0][0] = 0;
  vertices[0][1] = 0;
  vertices[1][0] = 0;
  vertices[1][1] = 1;
  vertices[2][0] = 1;
  vertices[2][1] = 0;
  
  Point point;
  for (int i = min_x; i <= max_x; i ++) {
    point.x = i;
    for (int j = min_y; j <= max_y; j ++) {
      point.y = j;
      
      float vertexProportions[3];
      vertexProportions[0] = triangleArea(point, points[1], points[2]);
      if(vertexProportions[0] < 0) continue;
      vertexProportions[1] = triangleArea(point, points[2], points[0]);
      if(vertexProportions[1] < 0) continue;
      vertexProportions[2] = triangleArea(point, points[0], points[1]);
      if(vertexProportions[2] < 0) continue;
      
      clr.r = clr.g = clr.b = uv[0] = uv[1] = 0;
      for(int p = 0; p < 3; p ++) {
//        clr.r += vertexProportions[p] * vertexColors[p].r / area;
//        clr.g += vertexProportions[p] * vertexColors[p].g / area;
//        clr.b += vertexProportions[p] * vertexColors[p].b / area;
        uv[0] = vertexProportions[p] * vertices[p][0] / area;
        uv[1] = vertexProportions[p] * vertices[p][1] / area;
      }
      
      int pixel = gimp_image.width / 2 + gimp_image.height / 2 * gimp_image.width;
      pixel = ((int) (uv[0] * gimp_image.width) % width) + ((int) (uv[1] * gimp_image.height) % height) * gimp_image.width;
      clr.r = gimp_image.pixel_data[pixel * 3 + 0];
      clr.g = gimp_image.pixel_data[pixel * 3 + 1];
      clr.b = gimp_image.pixel_data[pixel * 3 + 2];
  
      clr.r = (float) clr.r / 255;
      clr.g = (float) clr.g / 255;
      clr.b = (float) clr.b / 255;
      
      img->pixel(i, j, clr);
    }
  }
}

int triangleArea(Point p1, Point p2, Point p3) {
  return (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);
}
