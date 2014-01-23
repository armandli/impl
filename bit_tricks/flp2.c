#include "btricks.h"

//finds the greatest power of 2 less and equal to x, found in Hacker's Delight
//14 instructions, no loop
unsigned flp2(unsigned x){
  x = x | (x >> 1);
  x = x | (x >> 2);
  x = x | (x >> 4);
  x = x | (x >> 8);
  x = x | (x >> 16);
  return x - (x >> 1);
}

//find the least power of 2 greater or equal to x
unsigned clp2(unsigned x){
  x = x - 1;
  x = x | (x >> 1);
  x = x | (x >> 2);
  x = x | (x >> 4);
  x = x | (x >> 8);
  x = x | (x >> 16);
  return x + 1;
}
