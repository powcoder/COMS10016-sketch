https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
// -----------------------------------------------------------------
// Basic header skeleton for a Sketch File (.sk) Viewer
// -----------------------------------------------------------------

// Operations (DO NOT CHANGE)
enum { DX = 0, DY = 1, TOOL = 2, // basic
       DATA = 3 // intermediate
     };

// Tool Types (DO NOT CHANGE)
enum { NONE = 0, LINE = 1, // basic
       BLOCK = 2, COLOUR = 3, TARGETX = 4, TARGETY = 5, // intermediate
       SHOW = 6, PAUSE = 7, NEXTFRAME = 8 // advanced
     };

// Data structure holding the drawing state (DO NOT CHANGE)
typedef struct state { int x, y, tx, ty; unsigned char tool; unsigned int start, data; bool end;} state;

// -----------------------------------------------------------------
// DO NOT CHANGE ANY OF THE DECLARATIONS BELOW
// -----------------------------------------------------------------

// A byte is defined as an unsigned 8bit value
typedef unsigned char byte;

// Allocate memory for a drawing state and initialise it
state *newState();

// Release all memory associated with the drawing state
void freeState(state *s);

// Extract an opcode from a byte (two most significant bits).
int getOpcode(byte b);

// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b);

// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op);

// Draw a frame of the sketch file. For basic and intermediate sketch files
// this means drawing a static picture when this function is first called.
// For advanced sketch files this means drawing the current frame whenever
// this function is called.
bool processSketch(display *d, void *data, const char pressedKey);

// View a sketch file in a 200x200 pixel window given the filename
void view(char *filename);
