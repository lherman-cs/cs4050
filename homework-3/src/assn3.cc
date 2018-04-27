//
//		          Programming Assignment #1
//
//			        Victor Zordan
//
//
//
/***************************************************************************/

/**
 * Name			: Lukas Herman
 * Homework # 	: 1
 *
 * Conventions	:
 * 	- "_": means previous value. Ex: x_last_ -> previous value of x_last
 */

/* Include needed files */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>  // The GL Utility Toolkit (Glut) Header

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <stack>
#include <unordered_map>
#include <vector>

#include "color.hpp"
#include "model.hpp"
#include "utils.hpp"

#define WIDTH 800
#define HEIGHT 800
#define BUMP_MAP_FILE "bump_map.pgm"
#define SMOOTH 1
#define BUMPY 2

int x_last, y_last;
Model *model;
double bm[HEIGHT][WIDTH] = {0};
static const Coordinate eye = {0.0, 0.0, 2.0};
static const Coordinate light = {0.0, 0.0, 2.0};
static const Color background = Color(1.0);
static const Color ambient = get_ambient();

// My functions
void init_display(const char *model_path);
void display(void);
void render(int mode);
void set_bump_map(double bm[][WIDTH]);

// Your functions
void init_window(void);
void write_pixel(int x, int y, const Color &color);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);

/***************************************************************************/

int main(int argc, char *argv[]) {
  /* This main function sets up the main loop of the program and continues the
loop until the end of the data is reached.  Then the window can be closed
using the escape key.						  */
  if (argc != 2) {
    printf("Usage: ./assn2 [model_path]\n");
    return 1;
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Computer Graphics");
  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);

  init_window();  // create_window

  init_display(argv[1]);
  glutMainLoop();  // Initialize The Main Loop
}

// My functions start here
void init_display(const char *model_path) {
  model = new Model(model_path);
  set_bump_map(bm);
  render(0);
  glutSwapBuffers();
}

void render(int mode) {
  int row, col;
  double n_row, n_col;  // Normalized row and col
  bool intersected;
  Coordinate normal, screen_pixel, direction, intersected_point, to_source,
      to_viewer, shadow_ray_eye, shadow_ray_direction;
  Color illumination;
  Triangle closest;

  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH; col++) {
      n_row = (double)row / (HEIGHT - 1) - 0.5;
      n_col = (double)col / (WIDTH - 1) - 0.5;

      screen_pixel = Coordinate(n_col, n_row, 1.5);
      direction = (screen_pixel - eye).normalize();
      intersected_point = Coordinate();
      closest = Triangle();

      // Determine the closest intersected triangle
      switch (mode & 1) {
        case 0:  // Flat shading
        default:
          intersected = model->find_closest_intersection(
              eye, direction, &closest, &intersected_point, nullptr);
          normal = closest.normal;
          break;
        case 1:  // Smooth shading
          intersected = model->find_closest_intersection(
              eye, direction, &closest, &intersected_point, &normal);
          break;
      }

      if (mode >> 1) normal = normal * bm[row][col];

      // Calculate Phong Shading
      if (intersected) {
        Color illumination = ambient;
        to_source = (light - intersected_point).normalize();
        to_viewer = (eye - intersected_point).normalize();
        shadow_ray_eye = intersected_point;
        shadow_ray_direction = (light - shadow_ray_eye).normalize();

        intersected =
            model->is_shadowed(shadow_ray_eye, shadow_ray_direction, closest);

        if (!intersected)
          illumination = illumination + get_diffuse(to_source, normal) +
                         get_specular(to_source, normal, to_viewer);

        write_pixel(col, row, illumination);
      } else
        write_pixel(col, row, background);
    }
  }
}

void set_bump_map(double bm[][WIDTH]) {
  std::ifstream fin(BUMP_MAP_FILE);

  std::string type;
  int width, height, max_bit;

  fin >> type >> width >> height >> max_bit;
  if (!(type == "P2" || type == "P5"))
    std::cerr << "The image format is not in PGM\n";

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      fin >> bm[row][col];
      bm[row][col] /= max_bit;
    }
  }

  fin.close();
}

void display(void) {}

/***************************************************************************/

void init_window()
/* Clear the image area, and set up the coordinate system */
{
  /* Clear the window */
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glOrtho(0, WIDTH, 0, HEIGHT, -1.0, 1.0);
}

/***************************************************************************/

void write_pixel(int x, int y, const Color &color)
/* Turn on the pixel found at x,y */
{
  glColor3f(color.red, color.blue, color.green);
  glBegin(GL_POINTS);
  glVertex3i(x, y, 0);
  glEnd();
}

/***************************************************************************/
void mouse(int button, int state, int x, int y) {
  /* This function I finessed a bit, the value of the printed x,y should
match the screen, also it remembers where the old value was to avoid multiple
readings from the same mouse click.  This can cause problems when trying to
start a line or curve where the last one ended */
  static int oldx = 0;
  static int oldy = 0;
  int mag;

  y *= -1;   // align y with mouse
  y += 500;  // ignore
  mag = (oldx - x) * (oldx - x) + (oldy - y) * (oldy - y);
  if (mag > 20) {
    printf(" x,y is (%d,%d)\n", x, y);
  }

  oldx = x;
  oldy = y;
  x_last = x;
  y_last = y;
}

/***************************************************************************/
void keyboard(unsigned char key, int x, int y)  // Create Keyboard Function
{
  static int mode = 0;
  switch (key) {
    case 27:    // When Escape Is Pressed...
      exit(0);  // Exit The Program
      break;
    case '1':  // stub for new screen
      printf("New screen\n");
      break;
    case 'x':
      mode = (mode & 2) | ((mode & 1) ^ 1);
      render(mode);
      glutSwapBuffers();
      break;
    case 'b':
      mode = ((mode & 2) ^ 2) | (mode & 1);
      render(mode);
      glutSwapBuffers();
      break;
  }
}
