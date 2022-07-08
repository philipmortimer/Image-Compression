// Basic program skeleton for a Sketch File (.sk) Viewer
#include "displayfull.h"
#include "sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Allocate memory for a drawing state and initialise it
state *newState() {
  state *stateCreated = (state *) malloc(sizeof(state));
  *stateCreated = (state){0, 0, 0, 0, LINE, 0, 0, false};
  return stateCreated;
}

// Release all memory associated with the drawing state
void freeState(state *s) {
  free(s);
}

// Extract an opcode from a byte (two most significant bits).
int getOpcode(byte b) {
  const byte MASK = 192;
  byte op = MASK & b;
  op = op >> 6;
  return (int) op;
}

// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b) {
  const byte MASK = 63;
  byte op = MASK & b;
  int ans = op > 31? (int)(op) - 64 : (int) op;
  return ans;
}

int getAbs (int x){
  return x < 0? x * -1 : x;
}

void handleDy(display *d, state *s, int operand){
  s->ty = s->ty + operand;
  if(s->tool == LINE) line(d, s->x, s->y, s->tx, s->ty);
  else if(s->tool == BLOCK) block(d, s->x, s->y, getAbs(s->x - s->tx), getAbs(s->y - s->ty));
  s->x = s->tx;
  s->y = s->ty;
}

void handleDx(display *d, state *s, int operand){
  s->tx = s->tx + operand;
}

void handleTool(display *d, state *s, int operand){
  if(operand == COLOUR){
    colour(d, (int)s->data);
  }else if(operand == TARGETX){
    s->tx = s->data;
  }else if(operand == TARGETY){
    s->ty = s->data;
  }else if(operand == SHOW){
    show(d);
  }else if(operand == PAUSE){
    pause(d,s->data);
  }else if(operand == NEXTFRAME){
    s->end = true;
  }else{
    s->tool = operand >= 0? (unsigned char) operand : (unsigned char) (operand + 64);
  }
  s->data = 0;
}

void handleData(display *d, state *s, int operand){
  s->data = s->data << 6;
  unsigned int uOperand = operand >= 0? (unsigned int) operand : (unsigned int) (operand + 64);
  s->data = s->data | uOperand; //potential for error here via casting, check this.
}

// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op) {
  int operand = getOperand(op);
  int opcode = getOpcode(op);
  switch (opcode){
    case DX:
      handleDx(d, s, operand);
      break;
    case DY:
      handleDy(d, s, operand);
      break;
    case TOOL:
      handleTool(d, s, operand);
      break;
    case DATA:
      handleData(d, s, operand);
      break;
  }
}

void resetStateBarStartVar(state*s){
  s->x = 0;
  s->y = 0;
  s->tx = 0;
  s->ty = 0;
  s->tool = LINE;
  s->data = 0;
  s->end = false;
}

// Draw a frame of the sketch file. For basic and intermediate sketch files
// this means drawing the full sketch whenever this function is called.
// For advanced sketch files this means drawing the current frame whenever
// this function is called.
bool processSketch(display *d, void *data, const char pressedKey) {

    //TO DO: OPEN, PROCESS/DRAW A SKETCH FILE BYTE BY BYTE, THEN CLOSE IT
    //NOTE: CHECK DATA HAS BEEN INITIALISED... if (data == NULL) return (pressedKey == 27);
    //NOTE: TO GET ACCESS TO THE DRAWING STATE USE... state *s = (state*) data;
    //NOTE: TO GET THE FILENAME... char *filename = getName(d);
    //NOTE: DO NOT FORGET TO CALL show(d); AND TO RESET THE DRAWING STATE APART FROM
    //      THE 'START' FIELD AFTER CLOSING THE FILE
    if(data == NULL) return (pressedKey == 27);
    FILE *in = fopen(getName(d),"rb");
    fseek(in, 0, SEEK_END);
    int length = (int) ftell(in);
    state *s = (state*)data;
    s->start = s->start % length;
    fseek (in, s->start, SEEK_SET);
    byte ch = fgetc(in);
    while (!feof(in) && !s->end){
      obey(d, s, ch);
      s->start = s->start + 1;
      ch = fgetc(in);
    }
    fclose(in);
    show(d);
    resetStateBarStartVar(s);
    return (pressedKey == 27);
}

// View a sketch file in a 200x200 pixel window given the filename
void view(char *filename) {
  display *d = newDisplay(filename, 200, 200);
  state *s = newState();
  run(d, s, processSketch);
  freeState(s);
  freeDisplay(d);
}

// Include a main function only if we are not testing (make sketch),
// otherwise use the main function of the test.c file (make test).
#ifndef TESTING
int main(int n, char *args[n]) {
  if (n != 2) { // return usage hint if not exactly one argument
    printf("Use ./sketch file\n");
    exit(1);
  } else view(args[1]); // otherwise view sketch file in argument
  return 0;
}
#endif
