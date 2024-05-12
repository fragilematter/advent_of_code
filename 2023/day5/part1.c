#include <stdint.h>
#include <stdio.h>

#define NUMBER_OF_SEEDS 20
#define NUMBER_OF_MAPS 7

struct number_info {
  uint64_t number;
  uint8_t step;
};

void read_seeds(FILE *f, struct number_info *seed_info) {
  for (uint8_t i = 0; i < NUMBER_OF_SEEDS; i++) {
    if (fscanf(f, "%lu", &seed_info[i].number) != 1) {
      fprintf(stderr, "Failure reading seed number %d\n", i);
    }
#ifdef DEBUG
    printf("Read seed %lu\n", seed_info[i].number);
#endif
  }
}

void fskipline(FILE *f) {
  char c;
  do
    c = fgetc(f);
  while (!feof(f) && c != '\n');
}

void parse_map(FILE *f, struct number_info *numbers, uint8_t step) {
  uint64_t dest_range_start, source_range_start, range_length;
  uint64_t source_range_end;

  while (fscanf(f, "%lu %lu %lu", &dest_range_start, &source_range_start,
                &range_length) == 3) {
    source_range_end = source_range_start + range_length;

    for (uint8_t i = 0; i < NUMBER_OF_SEEDS; i++) {
      if (numbers[i].step == step)
        continue;
      if (numbers[i].number < source_range_start ||
          numbers[i].number > source_range_end)
        continue;

#ifdef DEBUG
      printf("Number %lu found in source %lu range %lu, destination %lu\n",
             numbers[i].number, source_range_start, range_length,
             dest_range_start);
#endif

      numbers[i].number =
          dest_range_start + (numbers[i].number - source_range_start);
      numbers[i].step = step;
#ifdef DEBUG
      printf("New number %lu\n", numbers[i].number);
#endif
    }
  }
}

void get_min_location(struct number_info *numbers, uint64_t *min_location) {
  *min_location = numbers[0].number;

  for (uint8_t i = 1; i < NUMBER_OF_SEEDS; i++) {
    if (*min_location > numbers[i].number)
      *min_location = numbers[i].number;
  }
}

int main(void) {
  FILE *f = fopen("input", "r");

  if (!f)
    return -1; // could not open file
  if (feof(f)) {
    fclose(f);
    return -2; // file is empty
  }

  struct number_info numbers[20];

  fseek(f, 7, SEEK_SET); // skip "seeds: " header
  read_seeds(f, numbers);

  fseek(f, 2, SEEK_CUR); // skip blank lines

  for (uint8_t i = 1; i <= NUMBER_OF_MAPS; i++) {
#ifdef DEBUG
    printf("\nParsing map %d\n", i);
#endif
    fskipline(f); // skip map name line

    parse_map(f, numbers, i);
  }

  fclose(f);

  uint64_t min_location;
  get_min_location(numbers, &min_location);

  printf("Lowest location number found is %lu\n", min_location);
}
