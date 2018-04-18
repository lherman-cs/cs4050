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
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <stack>
#include <unordered_map>
#include <vector>

#include "color.hpp"
#include "model.hpp"
#include "utils.hpp"

#define WIDTH 400
#define HEIGHT 400
#define SCALE 0.8

int x_last, y_last;
Model *model;

// My functions
void init_display(const char *model_path);
void display(void);
void draw_line(int x_, int y_, int x, int y);
void wire_frame();
void render();

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
  glutInitWindowSize(HEIGHT, WIDTH);
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
  normalize(model->vertices, HEIGHT, WIDTH, SCALE);
  render();

  // wire_frame();

  glutSwapBuffers();
}

void wire_frame(void) {
  const std::vector<Face> &faces = model->faces;

  for (const Face &face : faces) {
    for (unsigned i = 0; i < face.size(); i++) {
      const Coordinate &before = *face[i].vertex;
      const Coordinate &after = *face[(i + 1) % face.size()].vertex;
      draw_line(before.x, before.y, after.x, after.y);
      std::cerr << before.x << ',' << before.y << ',' << before.z << '\n';
    }
    std::cerr << "===================================\n";
  }
}

void render() {
  Coordinate min = get_min(model->vertices), max = get_max(model->vertices),
             avg = get_avg(model->vertices);
  move(model->vertices, {avg.x, avg.y, -4});

  Coordinate eye = {WIDTH >> 1, HEIGHT >> 1, 0};
  Coordinate light = {0, HEIGHT, 0};

  for (int row = 0; row < HEIGHT; row++) {
    for (int col = 0; col < WIDTH; col++) {
      Coordinate screen_pixel = Coordinate(col, row, -2);
      Coordinate direction = (screen_pixel - eye).normalize();
      Coordinate intersected_point = Coordinate();
      Triangle closest = Triangle();
      Color illumination = Color(1.0, 1.0, 1.0);

      // Determine the closest intersected triangle
      bool intersected = find_closest_intersection(
          model->faces, eye, direction, &closest, &intersected_point);

      // Calculate Phong Shading
      if (intersected) {
        // std::cerr << "intersected\n";
        Coordinate to_source = light - intersected_point;
        Coordinate to_viewer = eye - intersected_point;

        intersected = find_closest_intersection(model->faces, intersected_point,
                                                to_source.normalize(), &closest,
                                                &intersected_point);
        illumination = get_ambient();

        if (!intersected)
          illumination = illumination +
                         get_diffuse(to_source, closest.normal()) +
                         get_specular(to_source, closest.normal(), to_viewer);
      }
      write_pixel(col, row, illumination);
    }
  }
}

void display(void) {}

void draw_line(int x_, int y_, int x, int y) {
  int dx = x - x_;
  int dy = y - y_;
  int steps = abs(dy) > abs(dx) ? abs(dy) : abs(dx);

  double x_plot = x_, y_plot = y_;
  double x_step = (double)dx / steps, y_step = (double)dy / steps;

  write_pixel(x_, y_, Color(1.0));

  for (int i = 0; i < steps; i++) {
    x_plot += x_step;
    y_plot += y_step;

    write_pixel((int)round(x_plot), (int)round(y_plot), Color(1.0));
  }
}

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
  switch (key) {
    case 27:    // When Escape Is Pressed...
      exit(0);  // Exit The Program
      break;
    case '1':  // stub for new screen
      printf("New screen\n");
      break;
  }
}
