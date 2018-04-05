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

#include "utils.hpp"

#define WIDTH 800
#define HEIGHT 800
#define SCALE 0.8
#define MIN_INTENSITY 0.3
#define MAX_INTENSITY 1.0
#define TRANSLATION_MODE 1
#define ROTATION_MODE 2
#define SCALE_MODE 4
#define SCALE_RELATIVE_FACTOR 0.01
#define ROTATE_RELATIVE_DEGREE 0.1

int x_last, y_last;
Model model;

// My functions
void init_display(const char *model_path);
void display(void);
void wire_frame(void);
void z_buffer(void);
void toggle_perspective(bool z_buffer_mode);
void refresh(bool z_buffer_mode);
void draw_line(int x1, int y1, int x2, int y2);

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
  read_model(model_path, &model);
  normalize(model.vertices, HEIGHT, WIDTH, SCALE);
  wire_frame();

  glutSwapBuffers();
}

void display(void) {}

// Connect all vertices with lines define in each face
// Assume that the format uses 1-based indices
void wire_frame(void) {
  const std::vector<Face> &faces = model.faces;

  for (const Face &face : faces) {
    for (unsigned i = 0; i < face.size(); i++) {
      const Vertex &before = *face[i].vertex;
      const Vertex &after = *face[(i + 1) % face.size()].vertex;
      draw_line(before.x, before.y, after.x, after.y);
    }
  }
}

void z_buffer(void) {
  static double depth_buffer[HEIGHT][WIDTH];
  // Initialize depth_buffer to zero
  std::fill((double *)depth_buffer,
            (double *)depth_buffer + sizeof(depth_buffer) / sizeof(double),
            -INFINITY);

  double color = MIN_INTENSITY,
         color_inc = (MAX_INTENSITY - MIN_INTENSITY) / model.faces.size();

  std::unordered_map<int, std::vector<EdgeInfo>> edge_table;
  std::vector<EdgeInfo> ael;

  for (const Face &face : model.faces) {
    double min_y = std::numeric_limits<double>::max(),
           max_y = std::numeric_limits<double>::min();

    // This call will clear edge_table and
    // build a new one using the given face
    build_edge_table(face, edge_table);

    // Get min and max y
    for (const FaceElement &el : face) {
      min_y = el.vertex->y < min_y ? el.vertex->y : min_y;
      max_y = el.vertex->y > max_y ? el.vertex->y : max_y;
    }

    // For each scanline s
    int start_y = (int)round(min_y), end_y = (int)round(max_y);
    for (int s = start_y; s <= end_y; s++) {
      // Add edge_table[s] to the ael
      ael.insert(ael.end(), edge_table[s].begin(), edge_table[s].end());

      if (!ael.empty()) {
        auto x_comparitor = [](const EdgeInfo &info1, const EdgeInfo &info2) {
          return info1.x_cur < info2.x_cur;
        };
        std::sort(ael.begin(), ael.end(), x_comparitor);

        // Fill pixels between pairs of edges
        for (unsigned pair_i = 0; pair_i < ael.size(); pair_i += 2) {
          const EdgeInfo &edge1 = ael[pair_i];
          const EdgeInfo &edge2 = ael[pair_i + 1];
          int start_x = (int)round(edge1.x_cur),
              end_x = (int)round(edge2.x_cur);
          double cur_z = edge1.z_cur,
                 inc_z = (edge1.z_cur - edge2.z_cur) / (start_x - end_x);

          for (int x = start_x; x <= end_x; x++) {
            if (s >= 0 && s < HEIGHT && x >= 0 && x < WIDTH &&
                cur_z > depth_buffer[s][x]) {
              write_pixel(x, s, color);
              depth_buffer[s][x] = cur_z;
            }

            cur_z += inc_z;
          }
        }

        // Delete finished edges
        auto is_finished = [s](const EdgeInfo &edge) {
          return edge.y_upper == s + 1;
        };
        ael.erase(std::remove_if(ael.begin(), ael.end(), is_finished),
                  ael.end());

        // Update x and z values for edges in ael
        auto update = [](EdgeInfo &edge) {
          edge.x_cur += edge.x_inc;
          edge.z_cur += edge.z_inc;
        };
        std::for_each(ael.begin(), ael.end(), update);
      }
    }

    ael.clear();
    color += color_inc;
  }
}

void toggle_perspective(bool z_buffer_mode) {
  static bool perspective_mode = false;
  static std::vector<Vertex> prev;
  std::vector<Vertex> &vertices = model.vertices;
  double d = -0.5;

  if (!perspective_mode) {
    // Store current vertices
    prev = vertices;

    Vertex max = get_max(vertices), min = get_min(vertices);
    auto min_max_norm_fn = [&max, &min](Vertex &v) {
      v.z = (v.z - min.z) / (max.z - min.z);
    };
    auto perspective_fn = [d](Vertex &v) {
      v.x *= d / v.z;
      v.y *= d / v.z;
    };

    // Do a min-max normalization to be in between 0 and 1
    std::for_each(vertices.begin(), vertices.end(), min_max_norm_fn);
    // Move the objects to be a little bit far away
    // from the camera
    move(vertices, {0, 0, -2});
    // Do perspective view transformation
    std::for_each(vertices.begin(), vertices.end(), perspective_fn);
    // Normalize it so that it fits the screen
    normalize(vertices, HEIGHT, WIDTH, SCALE);
  } else
    vertices = prev;

  perspective_mode ^= 1;

  refresh(z_buffer_mode);
}

void interactive_handler(bool z_buffer_mode, unsigned char mode,
                         unsigned char key) {
  Vertex avg = get_avg(model.vertices);
  switch (mode) {
    case TRANSLATION_MODE:
      switch (key) {
        case 'a':
          avg.x--;
          break;
        case 'w':
          avg.y++;
          break;
        case 's':
          avg.y--;
          break;
        case 'd':
          avg.x++;
          break;
      }
      move(model.vertices, avg);
      break;
    case ROTATION_MODE:
      switch (key) {
        case 'a':
          rotate(model.vertices, Axis::y, -ROTATE_RELATIVE_DEGREE);
          break;
        case 'd':
          rotate(model.vertices, Axis::y, ROTATE_RELATIVE_DEGREE);
          break;
        case 'w':
          rotate(model.vertices, Axis::x, ROTATE_RELATIVE_DEGREE);
          break;
        case 's':
          rotate(model.vertices, Axis::x, -ROTATE_RELATIVE_DEGREE);
          break;
      }
      break;
    case SCALE_MODE:
      switch (key) {
        case 'a':
        case 'd':
          scale(model.vertices, 1 + SCALE_RELATIVE_FACTOR);
          break;
        case 'w':
        case 's':
          scale(model.vertices, 1 - SCALE_RELATIVE_FACTOR);
          break;
      }
  }
  refresh(z_buffer_mode);
}

void refresh(bool z_buffer_mode) {
  void (*fn)();
  if (z_buffer_mode)
    fn = z_buffer;
  else
    fn = wire_frame;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  fn();
  glutSwapBuffers();
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
  static unsigned char mode = 0;
  static bool z_buffer_mode = false;
  switch (key) {
    case 27:    // When Escape Is Pressed...
      exit(0);  // Exit The Program
      break;
    case '1':  // stub for new screen
      printf("New screen\n");
      break;
    case 'z':
      if (!z_buffer_mode) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        z_buffer();
        glutSwapBuffers();
      } else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        wire_frame();
        glutSwapBuffers();
      }
      z_buffer_mode ^= 1;
      break;
    case 'v':
      toggle_perspective(z_buffer_mode);
      break;
    case 't':
      mode = TRANSLATION_MODE;
      break;
    case 'r':
      mode = ROTATION_MODE;
      break;
    case 'e':
      mode = SCALE_MODE;
      break;
    case 'a':
    case 'w':
    case 's':
    case 'd':
      interactive_handler(z_buffer_mode, mode, key);
      break;
    default:
      break;
  }
}
