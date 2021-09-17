/**
 * global.h
 * @brief Contains declarations of all of the function and variables used to init and run glut/opengl
 * @author Elias Mawa
 * @date 10-16-2019
 * Author:		Elias Mawa
 * Created on: 	10-16-2019
 * Last Edit:	12-03-2019
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

#include <iostream>

///////////////////////////////////////////////////////
//
// Variables
//
///////////////////////////////////////////////////////

/** Number of objects in program */
extern unsigned int objects;
/** Flag for slowmode */
extern bool slowmode;
/** thread kill switch */
extern bool kill;

/** Refrence to the main window */
extern int main_window;
/** Window width */
extern int window_width;
/** Window height */
extern int window_height;

/** array of RGB values */
extern int rgb[];
/** Alpha value */
extern float alpha;
/** Pixel size */
extern int pixel_size;
/** Colorspace */
extern const float COLORS;

/** HSV hue */
extern unsigned int hsv_hue;
/** HSV saturation */
extern float hsv_saturation;
/** HSV value */
extern float hsv_value;
/** Flag to show on-screen help */
extern bool show_help;

/** Left mouse button flag (Erase) */
extern bool mouse_left;
/** Right mouse button flag (Draw*/
extern bool mouse_right;
/** Flag for a mouse key being activated */
extern bool mouse_active;
/** X key flag */
extern bool x_key;

/** Mouse coordinate location */
extern float mouse_x, mouse_y;

///////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////

/**
 * Initializes glut/opengl.
 * @param argc Number of command line args
 * @param argv Array of command line args
 */
void init(int argc, char** argv);

/** 
 * Draws objects to the screen
 */
void draw();

/**
 * Performs the logic for each object
 */
void update();

/**
 * Captures special keyboard keys
 * @param key Key pressed
 * @param x Mouse x.
 * @param y Mouse y.
 */
void kb_s(int key, int x, int y);

/**
 * Captures normal keyboard keys
 * @param key Key pressed.
 * @param x Mouse x.
 * @param y Mouse y.
 */
void kb(unsigned char key, int x, int y);

/**
 * Captures normal keyboard keys.
 * @param key Key pressed
 * @param x Mouse x.
 * @param y Mouse y.
 */
void kb_up(unsigned char key, int x, int y);

/**
 * Captures mouse buttons.
 * @param button Mouse button press.
 * @param state The state of the button pressed.
 * @param x Mouse x.
 * @param y Mouse y.
 */
void mouse(int button, int state, int x, int y);

/**
 * Handles motion wheel events.
 * @param button Mouse button press.
 * @param dir Direction of scrolling.
 * @param x Mouse x.
 * @param y Mouse y.
 */
void mouse_w(int button, int dir, int x, int y);

/**
 * Captures mouse movement.
 * @param x Mouse x.
 * @param y Mouse y.
 */
void mouse_m(int x, int y);

/**
 * Captures passive mouse movement (When no other keys are being pressed).
 * @param x Mouse x.
 * @param y Mouse y.
 */
void mouse_p(int x, int y);

/**
 * Function that handles pixel drawing events.
 */
void place();

/**
 * Main loop of program.
 */
void main_loop();

/**
 * Function for converting HSV values into rgb values.
 * @param hue Hue value (0-360).
 * @param output Array where rgb values will be output too.
 * @param saturation Color saturation value.
 * @param saturation Color shading value.
 */
void HSVtoRGB(int hue, int output[3], double saturation = 1, double value = 1);

/**
 * Draw text on screen at pixel coordinates.
 * @param text String to draw.
 * @param x Screen x coordinate (pixels from left).
 * @param y Screen y coordinate (pixels from bottom).
 * @param font GLUT bitmap font to use.
 * @param r Red color (0-1).
 * @param g Green color (0-1).
 * @param b Blue color (0-1).
 */
void drawText(const char* text, float x, float y, void* font, float r = 1.0f, float g = 1.0f, float b = 1.0f);

/**
 * Draw a filled rectangle in pixel coordinates.
 * @param x Left coordinate.
 * @param y Bottom coordinate.
 * @param w Width.
 * @param h Height.
 * @param r Red (0-1).
 * @param g Green (0-1).
 * @param b Blue (0-1).
 * @param a Alpha (0-1).
 */
void drawRect(float x, float y, float w, float h, float r, float g, float b, float a = 1.0f);

/**
 * Set up orthographic projection in pixel coordinates for UI drawing.
 */
void setPixelProjection();

/**
 * Restore previous projection after UI drawing.
 */
void restoreProjection();

/**
 * Draw the on-screen UI (info panel, color preview, help overlay).
 */
void drawUI();

#endif // GLOBAL_H
