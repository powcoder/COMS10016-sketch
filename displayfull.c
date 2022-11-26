https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
// This display module provides basic graphics support for drawing built on SDL2 using a single window.
// ----------------------------------------------------------------------------------------------------
// Full comments on how to use the module can be found in the header file.
#include "displayfull.h"
#define SDL_MAIN_HANDLED
#define FAILURE_CODE 1 // exit code at program failure

// display object needed for a managing a graphics window
struct display {
  SDL_Window *window;
  SDL_Renderer *renderer;
  char *name;
  int width;
  int height;
  Uint8 r, g, b, a;
};

// If SDL fails, print the SDL error message, and stop the program immediately.
static void fail() {
  fprintf(stderr, "Error: %s\n", SDL_GetError());
  SDL_Quit();
  exit(FAILURE_CODE);
}

// Check return values from SDL functions (Integer and Pointer versions).
// If return values are ok, just return these values. If there are errors
// call the fail function. These make error handling less intrusive.
static int safeI(int n) { if (n < 0) fail(); return n; }
static void *safeP(void *p) { if (p == NULL) fail(); return p; }

void pause(display *d, int ms) {
  SDL_Delay(ms);
}

int getWidth(display *d) {
  return d->width;
}

int getHeight(display *d) {
  return d->height;
}

char *getName(display *d) {
  return d->name;
}

void line(display *d, int x0, int y0, int x1, int y1) {
  safeI(SDL_RenderDrawLine(d->renderer, x0, y0, x1, y1));
}

void block(display *d, int x, int y, int w, int h) {
  SDL_Rect r = (SDL_Rect) {x, y, w, h};
  safeI(SDL_RenderFillRect(d->renderer, &r));
}

void pixel(display *d, int x, int y) {
  safeI(SDL_RenderDrawPoint(d->renderer, x, y));
}

void colour(display *d, int rgba) {
  d->r = (rgba >> 24) & 0xFF;
  d->g = (rgba >> 16) & 0xFF;
  d->b = (rgba >> 8) & 0xFF;
  d->a = rgba & 0xFF;
  safeI(SDL_SetRenderDrawColor(d->renderer, d->r, d->g, d->b, d->a));
}

void show(display *d) {
  SDL_RenderPresent(d->renderer);
  SDL_Delay(10);
  safeI(SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 0xFF));
  block(d, 0, 0, d->width, d->height);
  safeI(SDL_SetRenderDrawColor(d->renderer, d->r, d->g, d->b, d->a));
}

display *newDisplay(char *name, int width, int height) {
  setbuf(stdout, NULL);
  display *d = malloc(sizeof(display));
  safeI(SDL_Init(SDL_INIT_VIDEO));
  d->name = name;
  d->width = width;
  d->height = height;
  d->window = safeP(SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED,
                 SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN));
  d->renderer = safeP(SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED));
  safeI(SDL_RenderClear(d->renderer));
  colour(d,0xFF);
  block(d, 0, 0, width, height);
  colour(d,0xFFFFFFFF);
  show(d);
  return d;
}

void run(display *d, void *data, bool action(display *, void*, const char)) {
  bool quit = false;
  char key = 0;
  SDL_Event e;
  while (!quit) {
    quit = action(d, data, key);
    key = 0;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_KEYDOWN) key = (char) e.key.keysym.sym;
      if (e.type == SDL_QUIT) quit = true;
    }
  }
}

void freeDisplay(display *d) {
  SDL_DestroyRenderer(d->renderer);
  SDL_DestroyWindow(d->window);
  SDL_Quit();
  free(d);
}
