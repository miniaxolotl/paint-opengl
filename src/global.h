/**
 * global.h
 * Contains declarations of all of the function and variables used to init and run glut/opengl.
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
#include <thread>
#include <regex>

///////////////////////////////////////////////////////
//
// Variables
//
///////////////////////////////////////////////////////

/** Refrence to the main window */
extern int main_window;
/** Window width */
extern int window_width;
/** Window height */
extern int window_height;
/** Number */
extern unsigned int objects;
/** Thread for handling terminal inputs */
extern std::thread terminal_thread;
/** RGB values */
extern unsigned int r, g, b;


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
 * @param x 
 * @param y
 */
void kb_s(int key, int x, int y);

/**
 * Captures normal keyboard keys
 * @param key
 * @param x 
 * @param y
 */
void kb(unsigned char key, int x, int y);

/**
 * Captures mouse buttons
 * @param button 
 * @param s
 * @param x
 * @param y
 */
void mouse(int button, int s, int x, int y);

/**
 * 
 */
void mouse_w(int button, int dir, int x, int y);

/**
 * Captures mouse movement
 * @param x
 * @param y
 */
void mouse_m(int x, int y);

/**
 * Places down pixels
 */
void place(bool clear_flag);

/**
 * Main loop of program
 */
void main_loop();

/**
 * Thread for handling terminal inputs
 */
void terminal_thread_func();

#endif // GLOBAL_H
