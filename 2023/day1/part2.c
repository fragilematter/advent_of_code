#include <stdio.h>

enum states {
  STATE_NONE,
  STATE_O,
  STATE_ON,
  STATE_T,
  STATE_TW,
  STATE_TH,
  STATE_THR,
  STATE_THRE,
  STATE_F,
  STATE_FO,
  STATE_FOU,
  STATE_FI,
  STATE_FIV,
  STATE_S,
  STATE_SI,
  STATE_SE,
  STATE_SEV,
  STATE_SEVE,
  STATE_E,
  STATE_EI,
  STATE_EIG,
  STATE_EIGH,
  STATE_N,
  STATE_NI,
  STATE_NIN
};

int letter_parser_machine(char c) {
  static enum states state = STATE_NONE;

  if (c == 0) {
    // reset state
    state = STATE_NONE;
    return -1;
  }

  if (c > 47 && c < 58) {
    // a number was read, just reset the state
    // and return the value

    state = STATE_NONE;
    return c - '0';
  }

  if (c < 91)
    c += 32; // convert upper case to lower case

  if (c < 97 || c > 122) {
    // we didn't read a lower case letter, reset state

    state = STATE_NONE;
    return -1;
  }

  switch (state) {
  case STATE_NONE:
    switch (c) {
    case 'o':
      state = STATE_O;
      break;
    case 't':
      state = STATE_T;
      break;
    case 'f':
      state = STATE_F;
      break;
    case 's':
      state = STATE_S;
      break;
    case 'e':
      state = STATE_E;
      break;
    case 'n':
      state = STATE_N;
      break;
    default:
      state = STATE_NONE;
      return -1;
    }

    return -2;

  case STATE_O:
    if (c == 'n') {
      state = STATE_ON;
      return -3;
    }

    state = STATE_NONE;
    return letter_parser_machine(c);

  case STATE_ON:
    state = STATE_NONE;
    if (c == 'e') {
      letter_parser_machine(c);
      return 1; // found 'one'
    }
    return letter_parser_machine(c);

  case STATE_T:
    switch (c) {
    case 'w':
      state = STATE_TW;
      break;
    case 'h':
      state = STATE_TH;
      break;
    default:
      state = STATE_NONE;
      return letter_parser_machine(c);
    }

    return -3;

  case STATE_TW:
    state = STATE_NONE;
    if (c == 'o') {
      letter_parser_machine(c);
      return 2; // found 'two'
    }
    return letter_parser_machine(c);

  case STATE_TH:
    if (c == 'r') {
      state = STATE_THR;
      return -4;
    }
    state = STATE_NONE;
    return letter_parser_machine(c);

  case STATE_THR:
    if (c == 'e') {
      state = STATE_THRE;
      return -5;
    }
    state = STATE_NONE;
    return letter_parser_machine(c);

  case STATE_THRE:
    state = STATE_NONE;
    if (c == 'e') {
      letter_parser_machine(c);
      return 3; // found 'three'
    }
    return letter_parser_machine(c);

  case STATE_F:
    switch (c) {
    case 'o':
      state = STATE_FO;
      break;
    case 'i':
      state = STATE_FI;
      break;
    default:
      state = STATE_NONE;
      return letter_parser_machine(c);
    }

    return -3;

  case STATE_FO:
    if (c == 'u') {
      state = STATE_FOU;
      return -4;
    }
    if (c == 'n') {
      // we have 'fo' and we receive 'n' -> 'on'
      state = STATE_ON;
      return -3;
    }
    state = STATE_NONE;
    return letter_parser_machine(c);

  case STATE_FOU:
    state = STATE_NONE;
    if (c == 'r')
      return 4; // we have 'four'
    return letter_parser_machine(c);

  case STATE_FI:
    if (c == 'v') {
      state = STATE_FIV;
      return -4;
    }
    state = STATE_NONE;
    return letter_parser_machine(c);

  case STATE_FIV:
    state = STATE_NONE;
    if (c == 'e') {
      letter_parser_machine(c);
      return 5; // we have 'five'
    }
    return letter_parser_machine(c);

  case STATE_S:
    switch (c) {
    case 'i':
      state = STATE_SI;
      break;
    case 'e':
      state = STATE_SE;
      break;
    default:
      state = STATE_NONE;
      return letter_parser_machine(c);
    }
    return -3;

  case STATE_SI:
    state = STATE_NONE;
    if (c == 'x')
      return 6; // we have 'six'
    return letter_parser_machine(c);

  case STATE_SE:
    if (c == 'v') {
      state = STATE_SEV;
      return -4;
    }
    state = STATE_NONE;
    return letter_parser_machine(c);

  case STATE_SEV:
    if (c == 'e') {
      state = STATE_SEVE;
      return -5;
    }
    state = STATE_NONE;
    return letter_parser_machine(c);

  case STATE_SEVE:
    state = STATE_NONE;
    if (c == 'n') {
      letter_parser_machine(c);
      return 7; // we have 'seven'
    }
    return letter_parser_machine(c);

  case STATE_E:
    if (c == 'i') {
      state = STATE_EI;
      return -3;
    }
    state = STATE_NONE;
    return letter_parser_machine(c);

  case STATE_EI:
    if (c == 'g') {
      state = STATE_EIG;
      return -4;
    }
    state = STATE_NONE;
    return letter_parser_machine(c);

  case STATE_EIG:
    if (c == 'h') {
      state = STATE_EIGH;
      return -5;
    }
    state = STATE_NONE;
    return letter_parser_machine(c);

  case STATE_EIGH:
    state = STATE_NONE;
    if (c == 't') {
      letter_parser_machine(c);
      return 8; // we have 'eight'
    }
    return letter_parser_machine(c);

  case STATE_N:
    if (c == 'i') {
      state = STATE_NI;
      return -3;
    }
    state = STATE_NONE;
    return letter_parser_machine(c);

  case STATE_NI:
    if (c == 'n') {
      state = STATE_NIN;
      return -4;
    }
    state = STATE_NONE;
    return letter_parser_machine(c);

  case STATE_NIN:
    state = STATE_NONE;
    if (c == 'e') {
      letter_parser_machine(c);
      return 9; // we have 'nine'
    }
    return letter_parser_machine(c);
  }

  state = STATE_NONE;
  return -1;
}

int main(void) {
  FILE *f = fopen("input", "r");
  char c;
  int total, first, last;
  int eof_reached;
  int parsed_value;
#ifdef DEBUG
  int line_num;
#endif

  if (!f)
    return -1; // could not open file
  if (feof(f)) {
    fclose(f);
    return -2; // file is empty
  }

  total = 0;
  first = 0;
  last = 0;
#ifdef DEBUG
  line_num = 0;
#endif

  do {
    c = fgetc(f);
    eof_reached = feof(f);

    if (c == 10 || c == 13 || eof_reached) {
      // end of line or eof was reached
#ifdef DEBUG
      line_num++;
      printf("line %d - %d %d\n", line_num, first, last);
#endif
      total += first * 10 + last;

      first = 0;
      last = 0;
      letter_parser_machine(0); // reset the state
    } else {
      parsed_value = letter_parser_machine(c);
#ifdef DEBUG
      printf("parsed %c, returned %d\n", c, parsed_value);
#endif

      if (parsed_value > -1) {
        last = parsed_value;
        if (!first)
          first = last;
      }
    }
  } while (!eof_reached);

  fclose(f);

  printf("Calculated %d\n", total);
}
