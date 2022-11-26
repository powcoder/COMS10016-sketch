https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
// This display module provides basic graphics support for drawing (built on SDL2).
// ------------------------------------------------------------------------------
// A user does not have to understand how the functions are implemented in display.c.
// To use the module, first create a display via newDisplay().
// Then create your own drawing function that uses mainly the functions
// colour, line, pixel, block, pause, and show. Your function must have a particular
// signature: bool action(display*, void*, const char)
// Thus, your function should take a pointer to the created display, a void pointer
// to whatever custom data your function needs to represent persistent state
// (which can be cast by your funtion to the data structure you expect),
// and a char giving your function information about the currently pressed key.
// Then call run() with the display, your data, and your function as arguments.
// Then your function is called repeatedly until it returns true, then run() returns.
// Finally free your data and call freeDisplay() to shut down the graphics.

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// A display structure needs to be created by calling newDisplay,
// and then needs to be passed to each of the graphics functions.
// Once obsolete it should be freed with freeDisplay.
struct display;
typedef struct display display;

// Returns a pointer to a display object representing a plain black window of a given size.
// (For the sketch assignment the title MUST be the filename of the sketch file to be displayed.)
display *newDisplay(char *name, int width, int height);

// Free all memory allocated by the display and shut down.
void freeDisplay(display *d);

// Returns the width of the display object in pixels.
int getWidth(display *d);

// Returns the height of the display object in pixels.
int getHeight(display *d);

// Get the title of the graphics window.
// (For the sketch assignment this also retrieves the filename of the displayed sketch file.)
char *getName(display *d);

// Pauses processing for ms milliseconds
void pause(display *d, int ms);

// Make all recent changes appear on screen.
void show(display *d);

// Draw a line from (x0,y0) to (x1,y1) with current drawing colour. (must call show to make it appear)
void line(display *d, int x0, int y0, int x1, int y1);

// Draw a filled rectangle at (x,y) of size (w,h) with current drawing colour. (must call show to make it appear)
void block(display *d, int x, int y, int w, int h);

// Change the current drawing colour to rgba. Colour is represented as a packed int,
// where red, green, blue, and opp have unsigned single byte values packed into the int
// from the most to the least significant byte. (Default is white)
void colour(display *d, int rgba);

// Runs the (drawing) function action repeatedly until the display is closed or action returns true.
// The function action is provided with a pointer to the display, a pointer to the data,
// and a char representing the currently pressed key on the keyboard.
void run(display *d, void *data, bool action(display*, void*, const char));
