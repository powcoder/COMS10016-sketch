https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
// -----------------------------------------------------------------
// Testing Framework for the Sketch Assignment
//
// DO NOT CHANGE ANY OF THIS FILE SO YOU CAN SEE EXACTLY WHERE YOUR
// PROGRAM FAILS, WE WILL USE A FRESH COPY OF THIS FOR OUR TESTING.
// -----------------------------------------------------------------

#include "displayfull.h"
#include "sketch.h"

// display object needed for a managing a graphics window
struct display {
  char *file;
  char **calls;
  int width;
  int height;
  int n;
  char call[100];
};

// Forward declarations of findTest and fail, which are at the end of this file.
static char **findTest(char *file);
static void fail(display *d, char *format);

// A replacement for the library assert function.
static void assert(int line, bool b) {
  if (b) return;
  fprintf(stderr, "ERROR: The test on line %d in test.c fails.\n", line);
  exit(1);
}

// Tests 1-10 on Basic Opcode Extraction
static void testBasicOpcodes() {
  assert(__LINE__, getOpcode(0x80) == TOOL);
  assert(__LINE__, getOpcode(0x81) == TOOL);
  assert(__LINE__, getOpcode(0x40) == DY);
  assert(__LINE__, getOpcode(0x5F) == DY);
  assert(__LINE__, getOpcode(0x60) == DY);
  assert(__LINE__, getOpcode(0x7F) == DY);
  assert(__LINE__, getOpcode(0x00) == DX);
  assert(__LINE__, getOpcode(0x1F) == DX);
  assert(__LINE__, getOpcode(0x20) == DX);
  assert(__LINE__, getOpcode(0x3F) == DX);
  printf("Opcode Tests OK.\n");
}

// Tests 11-20 on Basic Operand Extraction
static void testBasicOperands() {
  assert(__LINE__, getOperand(0x00) == 0);
  assert(__LINE__, getOperand(0x1F) == 31);
  assert(__LINE__, getOperand(0x40) == 0);
  assert(__LINE__, getOperand(0x5F) == 31);
  assert(__LINE__, getOperand(0x20) == -32);
  assert(__LINE__, getOperand(0x3F) == -1);
  assert(__LINE__, getOperand(0x60) == -32);
  assert(__LINE__, getOperand(0x7F) == -1);
  assert(__LINE__, getOperand(0x80) == NONE);
  assert(__LINE__, getOperand(0x81) == LINE);
  printf("Operand Tests OK.\n");
}

// Tests 21-35 on State Initialisation and Basic Command Sequence
static void testBasicCommand() {
  display *d = newDisplay("testBasicCommands", 200, 200);
  state *s = newState();
  assert(__LINE__, s->start == 0 && s->end == false);
  assert(__LINE__, s->data == 0 && s->tool == LINE);
  assert(__LINE__, s->x == 0);
  assert(__LINE__, s->y == 0);
  assert(__LINE__, s->tx == 0);
  assert(__LINE__, s->ty == 0);
  obey(d, s, 0x1e);
  assert(__LINE__, s->tx == 30);
  obey(d, s, 0x5e);
  assert(__LINE__, s->tx == 30 && s->ty == 30);
  assert(__LINE__, s->x == 30 && s->y == 30);
  obey(d, s, 0x80);
  assert(__LINE__, s->tool == NONE);
  obey(d, s, 0x1e);
  obey(d, s, 0x7F);
  assert(__LINE__, s->tx == 60 && s->ty == 29);
  assert(__LINE__, s->x == 60 && s->y == 29);
  obey(d, s, 0x81);
  assert(__LINE__, s->tool == LINE);
  obey(d, s, 0x5e);
  assert(__LINE__, s->tx == 60 && s->ty == 59);
  assert(__LINE__, s->x == 60 && s->y == 59);
  show(d);
  freeState(s);
  freeDisplay(d);
}

// Running the FULL Testing Framework
static void doTesting() {
  // testing the module locally
  testBasicOpcodes();
  testBasicOperands();
  testBasicCommand();
  // testing the module on sketch files via testing proxy
  char filename[12];
  for (int i = 0; i < 10; i++) {
    sprintf(filename, "sketch%02d.sk",i);
    view(filename);
    if (i == 4) printf("ALL BASIC TESTS PASS.\n");
    if (i == 7) printf("ALL INTERMEDIATE TESTS PASS.\n");
    if (i == 9) printf("ALL ADVANCED TESTS PASS.\n");
  }
}

// Check an actual call against the next expected call.
static void check(display *d) {
  char *expect;
  expect = d->calls[d->n];
  static char *last = "";
  if (strcmp("", expect) == 0) fail(d, "       Unexpected extra call %s\n");
  else if (strcmp(d->call, expect) != 0) {
    if (strcmp(expect, "freeDisplay(d)") == 0) {
      d->n = 0;
      expect = d->calls[d->n];
    }
    if ((strcmp(last, "processSketchReturn") == 0) &&
        (strcmp(d->call, "freeDisplay(d)") == 0));
    else if (strcmp(d->call, expect) != 0) fail(d, "       Found call: %s\n       But expected: %s\n       Failure at call number: %d\n");
  }
  last = expect;
  d->n = d->n + 1;
}

void show(display *d) {
  sprintf(d->call, "show(d)");
  check(d);
}

int getWidth(display *d) {
  return d->width;
}

int getHeight(display *d) {
  return d->height;
}

char *getName(display *d) {
  return d->file;
}

void line(display *d, int x0, int y0, int x1, int y1) {
  sprintf(d->call, "line(d,%d,%d,%d,%d)", x0, y0, x1, y1);
  check(d);
}

void block(display *d, int x, int y, int w, int h) {
  sprintf(d->call, "block(d,%d,%d,%d,%d)", x, y, w, h);
  check(d);
}

void pause(display *d, int ms) {
  sprintf(d->call, "pause(d,%d)", ms);
  check(d);
}

void colour(display *d, int rgba) {
  sprintf(d->call, "colour(d,0x%08x)", (unsigned int)rgba);
  check(d);
}

display *newDisplay(char *name, int width, int height) {
  display *d = malloc(sizeof(display));
  *d = (struct display) { name, findTest(name), width, height, 0 };
  return d;
}

void run(display *d, void *data, bool action(display *, void*, const char)) {
  bool quit = false;
  char key = 0;
  while (!quit) {
    quit = action(d, data, key);
    quit = quit && (key>26);
    sprintf(d->call, "processSketchReturn");
    check(d);
    key++;
  }
}

void freeDisplay(display *d) {
  sprintf(d->call, "freeDisplay(d)");
  check(d);
  printf("Sketch Test OK: %s\n", d->file);
  free(d);
}

//Drawing sequence for BasicCommands Tests
static char *testBasicCommands[] = {
  "line(d,0,0,30,30)", "line(d,60,29,60,59)",
  "show(d)", "freeDisplay(d)", ""
};

//Drawing sequence for sketch00.sk - single line (positive operands)
//Note that "processSketchReturn" is not a command but indicates
//a point at which your processSketch(...) function has returned.
//Also note that "" is not an expected call and is used to check for
//right termination of call sequences.
static char *sketch00[] = {
  "line(d,0,0,30,30)", "show(d)", "processSketchReturn", "freeDisplay(d)", ""
};

//Drawing sequence for sketch01.sk - many lines (positive operands)
static char *sketch01[] = {
  "line(d,0,0,22,22)",       "line(d,22,22,22,53)",     "line(d,22,53,53,53)",
  "line(d,53,53,84,84)",     "line(d,84,84,115,84)",    "line(d,115,84,115,115)",
  "line(d,115,115,146,146)", "line(d,146,146,146,177)", "line(d,146,177,177,177)",
  "line(d,177,177,199,199)", "show(d)",  "processSketchReturn", "freeDisplay(d)", ""
};

//Drawing sequence for sketch02.sk - shifted square (tool toggle, positive and negative operands)
static char *sketch02[] = {
  "line(d,30,30,60,30)", "line(d,60,30,60,60)", "line(d,60,60,30,60)",
  "line(d,30,60,30,30)", "show(d)",  "processSketchReturn", "freeDisplay(d)", ""
};

//Drawing sequence for sketch03.sk - house (tool toggle, positive and negative operands, multiple shifts)
static char *sketch03[] = {
  "line(d,100,30,70,60)", "line(d,70,60,70,90)", "line(d,70,90,130,90)",
  "line(d,130,90,130,60)", "line(d,130,60,100,30)",
  "show(d)",  "processSketchReturn", "freeDisplay(d)", ""
};

//Drawing sequence for sketch04.sk - star (tool toggle, positive and negative operands, multiple shifts)
static char *sketch04[] = {
  "line(d,100,100,70,100)", "line(d,100,100,130,100)", "line(d,100,100,100,70)",
  "line(d,100,100,100,130)", "line(d,100,100,120,120)", "line(d,100,100,120,80)",
  "line(d,100,100,80,80)", "line(d,100,100,80,120)",
  "show(d)", "processSketchReturn", "freeDisplay(d)", ""
};

//Drawing sequence for sketch05.sk - blue (data prefetching, blocks, colours)
static char *sketch05[] = {
  "colour(d,0x0000ffff)", "block(d,0,0,199,199)",
  "show(d)", "processSketchReturn", "freeDisplay(d)", ""
};

//Drawing sequence for sketch06.sk - blocks (data prefetching, blocks, colours)
static char *sketch06[] = {
  "colour(d,0x0000ffff)", "block(d,0,0,199,199)", "colour(d,0x00ff00ff)",
  "block(d,0,139,199,60)", "colour(d,0xff0000ff)", "block(d,30,60,105,132)",
  "show(d)",  "processSketchReturn", "freeDisplay(d)", ""
};

//Drawing sequence for sketch07.sk - purple star (data prefetching, blocks, lines, colours)
static char *sketch07[] = {
  "colour(d,0x0000ffff)", "block(d,0,0,199,199)", "colour(d,0xff00ffff)",
  "line(d,100,100,70,100)", "line(d,100,100,130,100)", "line(d,100,100,100,70)",
  "line(d,100,100,100,130)", "line(d,100,100,120,120)", "line(d,100,100,120,80)",
  "line(d,100,100,80,80)", "line(d,100,100,80,120)",
  "show(d)", "processSketchReturn", "freeDisplay(d)", ""
};

//Drawing sequence for sketch08.sk - flicker (show, pause, data prefetching, blocks, colours)
static char *sketch08[] = {
  "colour(d,0xff00ffff)", "block(d,0,0,199,199)", "show(d)", "pause(d,192)",
  "colour(d,0x0000ffff)", "block(d,0,0,199,199)", "show(d)", "pause(d,192)",
  "show(d)", "processSketchReturn", "freeDisplay(d)", ""
};

//Drawing sequence for sketch09.sk - multiframe (frames, show, pause, data prefetching, blocks, colours)
static char *sketch09[] = {
  "pause(d,192)", "colour(d,0x0000ffff)", "block(d,0,0,199,199)", "show(d)",
  "processSketchReturn", "pause(d,192)",
  "colour(d,0xff00ffff)", "block(d,0,0,199,199)",
  "show(d)", "processSketchReturn", "pause(d,192)", "colour(d,0xffffffff)",
  "block(d,0,0,199,199)", "show(d)", "processSketchReturn", "freeDisplay(d)", ""
};

struct sketch { char *name, *file, **test; };
static struct sketch sketches[] = {
    {"testBasicCommands", ""  , testBasicCommands},
    {"sketch00", "sketch00.sk", sketch00},
    {"sketch01", "sketch01.sk", sketch01},
    {"sketch02", "sketch02.sk", sketch02},
    {"sketch03", "sketch03.sk", sketch03},
    {"sketch04", "sketch04.sk", sketch04},
    {"sketch05", "sketch05.sk", sketch05},
    {"sketch06", "sketch06.sk", sketch06},
    {"sketch07", "sketch07.sk", sketch07},
    {"sketch08", "sketch08.sk", sketch08},
    {"sketch09", "sketch09.sk", sketch09}
  };

// Find the right test for the given sketch filename.
static char **findTest(char *file) {
    int n = sizeof(sketches) / sizeof(struct sketch);
    for (int i = 0; i < n; i++) {
        if (strcmp(file, sketches[i].name) == 0) return sketches[i].test;
        if (strcmp(file, sketches[i].file) == 0) return sketches[i].test;
    }
    fprintf(stderr, "Can't find test for %s\n", file);
    exit(1);
    return NULL;
}

// Report failure and exit.
static void fail(display *d, char *format) {
    fprintf(stderr, "ERROR: Failure in drawing sketch file %s\n", d->file);
    fprintf(stderr, format, d->call, d->calls[d->n], d->n + 1);
    exit(1);
}

#ifdef TESTING
int main(int n, char *args[n]) {
  if (n == 1) { // if no arguments then run tests
    doTesting();
    return 0;
  }
  if (n != 2) { // return usage hint if not exactly one argument
    printf("Use ./test file\n");
    exit(1);
  } else view(args[1]); // otherwise test sketch file in argument
  return 0;
}
#endif
