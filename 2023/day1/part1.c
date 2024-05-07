#include <stdio.h>

int main(void) {
  FILE *f = fopen("input", "r");
  char c;
  int total, first, last;
  int eof_reached;

  if (!f)
    return -1; // could not open file
  if (feof(f)) {
    fclose(f);
    return -2; // file is empty
  }

  total = 0;
  first = 0;
  last = 0;

  do {
    c = fgetc(f);
    eof_reached = feof(f);

    if (c > 47 && c < 58) {
      // a number was read

      last = c - '0';
      if (first == 0)
        first = last;
    } else if (c == 10 || c == 13 || eof_reached) {
      // end of line or eof was reached
      total += first * 10 + last;

      first = 0;
      last = 0;
    }
  } while (!eof_reached);

  fclose(f);

  printf("Calculated %d\n", total);
}
