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

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 500
#define HEIGHT 500
#define N_POINTS 4
#define N_SEGMENTS 100
#define DELAY 5000  // In microseconds
#define EPS 10
#define THICK 3

int x_last, y_last;

// My functions
void draw_point(int x, int y, int thick);
void draw_line(int x1, int y1, int x2, int y2);
void draw_curve(int points[N_POINTS][2], int segments);
double smoothstep(double x);
void animate(int points[N_POINTS][2], int chosen_point, int to_x, int to_y);
bool is_changed();
bool is_clicked(int point[]);
void display(void);

// Your functions
void init_window(void);
void write_pixel(int x, int y, double intensity);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);

/***************************************************************************/

int main(int argc, char *argv[]) {
  /* This main function sets up the main loop of the program and continues the
loop until the end of the data is reached.  Then the window can be closed
using the escape key.						  */

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Computer Graphics");
  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);

  init_window();  // create_window

  glutMainLoop();  // Initialize The Main Loop
}

// My functions start here
void draw_point(int x, int y, int thick) {
  int low_x = x - thick, high_x = x + thick;
  int low_y = y - thick, high_y = y + thick;

  for (int cur_x = low_x; cur_x <= high_x; cur_x++)
    for (int cur_y = low_y; cur_y <= high_y; cur_y++)
      write_pixel(cur_x, cur_y, 1.0);
}

void draw_line(int x_, int y_, int x, int y) {
  int dx = x - x_;
  int dy = y - y_;
  int steps = abs(dy) > abs(dx) ? abs(dy) : abs(dx);

  double x_plot = x_, y_plot = y_;
  double x_step = (double)dx / steps, y_step = (double)dy / steps;

  write_pixel(x_, y_, 1.0);

  for (int i = 0; i < steps; i++) {
    x_plot += x_step;
    y_plot += y_step;

    write_pixel((int)round(x_plot), (int)round(y_plot), 1.0);
  }
}

void draw_curve(int points[N_POINTS][2], int segments) {
  // u = 0 -> 1
  double u = 0.0;
  double x_ = points[0][0], y_ = points[0][1], x = points[0][0],
         y = points[0][1];

  for (int i = 0; i <= segments; i++) {
    u = i / (double)segments;
    x = pow((1 - u), 3) * points[0][0];
    x += 3 * u * pow((1 - u), 2) * points[1][0];
    x += 3 * pow(u, 2) * (1 - u) * points[2][0];
    x += pow(u, 3) * points[3][0];

    y = pow((1 - u), 3) * points[0][1];
    y += 3 * u * pow((1 - u), 2) * points[1][1];
    y += 3 * pow(u, 2) * (1 - u) * points[2][1];
    y += pow(u, 3) * points[3][1];

    draw_line(round(x_), round(y_), round(x), round(y));

    x_ = x;
    y_ = y;
  }
}

// This a smoothstep function from https://en.wikipedia.org/wiki/Smoothstep.
double smoothstep(double x) { return 3 * pow(x, 2) - 2 * pow(x, 3); }

void animate(int points[N_POINTS][2], int chosen_point, int to_x, int to_y) {
  int dx = to_x - points[chosen_point][0];
  int dy = to_y - points[chosen_point][1];
  int steps = abs(dy) > abs(dx) ? abs(dy) : abs(dx);

  double x_plot = points[chosen_point][0], y_plot = points[chosen_point][1];
  double x_step = (double)dx / steps, y_step = (double)dy / steps;

  for (int i = 0; i < steps; i++) {
    x_plot += x_step;
    y_plot += y_step;

    points[chosen_point][0] = x_plot;
    points[chosen_point][1] = y_plot;

    usleep(smoothstep(i / (double)(steps - 1)) * DELAY);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear Screen
    draw_curve(points, N_SEGMENTS);
    glutSwapBuffers();
  }
}

/**
 * This is a javascript style event helper to catch if
 * the user click on a different location
 */
bool is_changed() {
  static int x_last_ = 0, y_last_ = 0;

  if ((x_last != x_last_) && (y_last != y_last_)) {
    x_last_ = x_last;
    y_last_ = y_last;
    return true;
  }
  return false;
}

/**
 * This is a javascript style event helper to catch if
 * the user click on the given point
 */
bool is_clicked(int point[]) {
  return (point[0] - EPS <= x_last && x_last <= point[0] + EPS) &&
         (point[1] - EPS <= y_last && y_last <= point[1] + EPS);
}

/******************************************************************
 *                                                                *
 *           The flow of the program is the following:            *
 *                                                                *
 *              FIRST: User chooses 4 control points              *
 *                               |                                *
 *                               V                                *
 *        SECOND: Draw the curve using the 4 control points       *
 *                               |                                *
 *                               V                                *
 *      THIRD: User is not allowed to choose another point        *
 *             and only allowed to move one point to another      *
 *             coordinate at a time by clicking on a point and    *
 *             click on a new location                            *
 *                               |                                *
 *                               V                                *
 *      FOURTH: Give an animation of the transition to the new    *
 *              location.                                         *
 *                                                                *
 ******************************************************************/
void display(void) {
  static int cntr = 0;
  static int points[N_POINTS][2];
  static bool drawn = false;
  static bool chosen = false;
  static int chosen_point = -1;

  if (is_changed()) {
    if (!drawn) {
      // FIRST: User chooses 4 control points

      if (cntr == 0) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear Screen
        glutSwapBuffers();
      }

      draw_point(x_last, y_last, THICK);
      points[cntr][0] = x_last;
      points[cntr][1] = y_last;
      cntr++;
      glutSwapBuffers();

      if (cntr % N_POINTS == 0) {
        draw_curve(points, N_SEGMENTS);
        glutSwapBuffers();
        cntr = 0;
        drawn = true;
      }

    } else if (!chosen) {
      // SECOND: Draw the curve using the 4 control points

      chosen = true;

      if (is_clicked(points[0]))
        chosen_point = 0;

      else if (is_clicked(points[1]))
        chosen_point = 1;

      else if (is_clicked(points[2]))
        chosen_point = 2;

      else if (is_clicked(points[3]))
        chosen_point = 3;

      else
        chosen = false;

    } else {
      // THIRD: User is not allowed to choose another point
      //        and only allowed to move one point to another
      //        coordinate at a time by clicking on a point and
      //        click on a new location

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear Screen

      animate(points, chosen_point, x_last, y_last);

      // Draw the points
      for (int i = 0; i < N_POINTS; i++)
        draw_point(points[i][0], points[i][1], THICK);

      glutSwapBuffers();
      chosen = false;
    }
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

void write_pixel(int x, int y, double intensity)
/* Turn on the pixel found at x,y */
{
  glColor3f(intensity, intensity, intensity);
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
    default:
      break;
  }
}
