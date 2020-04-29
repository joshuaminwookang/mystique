// Target code to be translated to assembly.
#include "circ.h"

int DrawCircle(int r, int i, pair display[]) // circuit input: only r
{
  int e = -r;
  int x = r;
  int y = 0;
  while (x >= y) {           // line 4
    display[i].x = x;       // these
    display[i++].y = y;     //    two lines: circuit displays x,y
    e = e+2*y+1;
    y++;
    if (e >= 0) {
      e = e+1-2*x;
      x--;
    }
  }
  return i;                 // assembly only
}
