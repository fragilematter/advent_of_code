#include <stdint.h>
#include <stdio.h>

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
  uint32_t winning_number_count;
  uint32_t total_cards = 0;
  struct number winning_numbers[WINNING_NUMBERS];
  char played_number[3];
  uint16_t additional_cards[UINT8_MAX] = {0};
  uint8_t card_index = 0;
  do {
    fseek(f, GAME_HEADER_SKIP_CHARS, SEEK_CUR);

    for (i = 0; i < WINNING_NUMBERS; i++) {
      if (fscanf(f, "%2s", winning_numbers[i].number) < 1) {
          goto no_number;
      }
#ifdef DEBUG
      printf("Read winning number %s\n", winning_numbers[i].number);
#endif
    }

    winning_number_count = 0;
    fseek(f, 2, SEEK_CUR); // skip | separator
    for (i = 0; i < PLAYED_NUMBERS; i++) {
      if (fscanf(f, "%2s", played_number) < 1)
        break;

#ifdef DEBUG
      printf("Read played number %s\n", played_number);
#endif
      for (j = 0; j < WINNING_NUMBERS; j++) {
        if (played_number[0] == winning_numbers[j].number[0] &&
            played_number[1] == winning_numbers[j].number[1]) {
#ifdef DEBUG
          printf("Number is winning!\n");
#endif
          winning_number_count++;
          additional_cards[card_index + winning_number_count]++;
          break;
        }
      }
    }
#ifdef DEBUG
    printf("Winning number count: %d\n\n", winning_number_count);
#endif

    total_cards += 1 + additional_cards[card_index];
    card_index++;
no_number:
  } while (!feof(f));

  fclose(f);

  printf("Calculated %d\n", total_cards);
}
