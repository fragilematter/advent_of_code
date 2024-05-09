#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define WINNING_NUMBERS 10
#define PLAYED_NUMBERS 25
#define GAME_HEADER_SKIP_CHARS 10

struct number {
  char number[3]; // (f)scanf will null-terminate strings
  // I won't getc this stuff again, so we're stuck with 1 extra byte
};

int main(void) {
  FILE *f = fopen("input", "r");

  if (!f)
    return -1; // could not open file
  if (feof(f)) {
    fclose(f);
    return -2; // file is empty
  }

  uint8_t i, j;
  uint32_t game_points;
  uint32_t total_points = 0;
  struct number winning_numbers[WINNING_NUMBERS];
  char played_number[3];

  do {
    fseek(f, GAME_HEADER_SKIP_CHARS, SEEK_CUR);

    for (i = 0; i < WINNING_NUMBERS; i++) {
      if (fscanf(f, "%2s", winning_numbers[i].number) < 1)
        memcpy(winning_numbers[i].number, " 0", 2);
#ifdef DEBUG
      printf("Read winning number %s\n", winning_numbers[i].number);
#endif
    }

    game_points = 0;
    fseek(f, 2, SEEK_CUR); // skip | separator
    for (i = 0; i < PLAYED_NUMBERS; i++) {
      if (fscanf(f, "%2s", played_number) < 1)
        break;

#ifdef DEBUG
      printf("Read played number %s\n", played_number);
#endif
      for (j = 0; j < WINNING_NUMBERS; j++) {
        if (strcmp(played_number, winning_numbers[j].number) == 0) {
#ifdef DEBUG
          printf("Number is winning!\n");
#endif
          if (game_points == 0) {
            game_points = 1;
            break;
          }
          game_points <<= 1;
          break;
        }
      }
    }
#ifdef DEBUG
    printf("Game points: %d\n\n", game_points);
#endif
    total_points += game_points;
  } while (!feof(f));

  fclose(f);

  printf("Calculated %d\n", total_points);
}
