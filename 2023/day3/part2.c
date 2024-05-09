#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

#define LINE_LENGTH 142 // should be enough for any elf schematic

/*
 * Solver design for minimal memory usage:
 * 1. keep 3 consecutive lines from the file in buffers
 * 2. scan line: for each part, determine N, S, E, W numbers
 * 3. if N or S neighbor is not found, search for corresponding
 *        NE,NW or SE,SW neighbors.
 *
 * Is it possible that a number is neighbor to two parts?
 */

void clear_line(char *line) {
    // memset(line, '.', LINE_LENGTH); // another include? I'd rather loop it
    for (uint8_t i = 0; i < LINE_LENGTH; i++)
        line[i] = '.';

    line[LINE_LENGTH - 1] = '\0';
}

int read_next_line(FILE *f, char *line) {
    if (feof(f) || fgets(line, LINE_LENGTH, f) == NULL) {
        clear_line(line);
        return -1;
    }

    return 0;
}

uint32_t poormanspow10(uint8_t power_of) {
    switch (power_of) {
        case 0:
            return 1;
        case 1:
            return 10;
        case 2:
            return 100;
        case 3:
            return 1000;
        case 4:
            return 10000;
        default:
            fprintf(stderr, "poormanspow10 called with invalid value %d\n", power_of);
            exit(1);
    }

    return 0;
}

uint32_t parse_number_reverse(char *line, uint16_t position) {
    uint8_t number_of_digits = 0;
    uint32_t sum = 0;

    while (isdigit(line[position])) {
        sum += ((line[position] - '0') * poormanspow10(number_of_digits));
        number_of_digits++;

        if (position == 0)
            break;
        position--;
    }

#ifdef DEBUG
    printf("parse_number_reverse found %d\n", sum);
#endif

    return sum;
}

uint32_t parse_number_forward(char *line, uint16_t position) {
    uint32_t sum = 0;

    while (isdigit(line[position])) {
        sum = sum * 10 + (line[position] - '0');
        position++;
    }

#ifdef DEBUG
    printf("parse_number_forward found %d\n", sum);
#endif

    return sum;
}

uint32_t parse_number_from_center(char *line, uint16_t position) {
    uint32_t sum = 0;
   
    sum = parse_number_reverse(line, position);

    position++;
    while (position < LINE_LENGTH && isdigit(line[position])) {
        sum = sum * 10 + (line[position] - '0');
        position++;
    }

#ifdef DEBUG
    printf("parse_number_from_center found %d\n", sum);
#endif

    return sum;
}

uint32_t parse_left_right(char *line, uint16_t position) {
    uint32_t sum = 0;

    if (isdigit(line[position])) {
#ifdef DEBUG
        printf("Look forward\n");
#endif
        return parse_number_from_center(line, position);
    } else {
        // get NE neighbor
        if (position > 0) {
#ifdef DEBUG
        printf("Look East\n");
#endif
          sum = parse_number_reverse(line, position - 1);
        }

        // get NW neighbor
        if (position < LINE_LENGTH) {
#ifdef DEBUG
        printf("Look West\n");
#endif
            uint32_t found_value = parse_number_forward(line, position + 1);
            if (found_value)
                sum = (sum ? sum : 1) * found_value;
        }
    }

    return sum;
}

uint8_t has_digit_before(char *line, uint16_t position) {
    if (position > 0 && isdigit(line[position - 1]))
        return 1;

    return 0;
}

uint8_t has_digit_after(char *line, uint16_t position) {
    if (position < LINE_LENGTH && isdigit(line[position + 1]))
        return 1;

    return 0;
}

uint8_t is_gear_ratio_valid(char *previous_line, char *current_line,
                            char *next_line, uint16_t position) {
    uint8_t gear_ratio_count = 0;

    gear_ratio_count += has_digit_before(current_line, position);
    gear_ratio_count += has_digit_after(current_line, position);

    if (isdigit(previous_line[position])) {
        gear_ratio_count++;
    } else {
        gear_ratio_count += has_digit_before(previous_line, position);
        gear_ratio_count += has_digit_after(previous_line, position);
    }

    if (gear_ratio_count > 2) return 0;

    if (isdigit(next_line[position])) {
        gear_ratio_count++;
    } else {
        gear_ratio_count += has_digit_before(next_line, position);
        gear_ratio_count += has_digit_after(next_line, position);
    }

    return (gear_ratio_count == 2);
}

uint32_t parse_line(char *previous_line, char *current_line, char *next_line) {
    // iterate through line
    // if symbol found check E, W, N, S
    // if no values found in N, S, check NE, NW and SE, SW
    uint32_t sum = 0;
    uint32_t gear_ratio;
    uint32_t found_value;

#ifdef DEBUG
    printf("\nparse line working set:\n");
    printf("prev: %s\n", previous_line);
    printf("curr: %s\n", current_line);
    printf("next: %s\n", next_line);
#endif

    for (uint16_t i = 0; i < LINE_LENGTH; i++) {
        if (current_line[i] < 32)
            break;

        if (current_line[i] != '*') 
            continue;

#ifdef DEBUG
        printf("\nFound symbol %c (%d)\n", current_line[i], current_line[i]);
#endif
        if (!is_gear_ratio_valid(previous_line, current_line, next_line, i))
            continue;

        gear_ratio = 1;

        // we have a token
        // check the East
        if (i > 0) {
#ifdef DEBUG
            printf("Look East\n");
#endif
            found_value = parse_number_reverse(current_line, i - 1);
            if (found_value)
                gear_ratio = found_value;
        }

        // chech the West
        if (i < LINE_LENGTH) {
#ifdef DEBUG
            printf("Look West\n");
#endif
            found_value = parse_number_forward(current_line, i + 1);
            if (found_value)
                gear_ratio *= found_value;
        }

#ifdef DEBUG
        printf("Look North\n");
#endif
        found_value = parse_left_right(previous_line, i);
        if (found_value)
            gear_ratio *= found_value;

#ifdef DEBUG
        printf("Look South\n");
#endif
        found_value = parse_left_right(next_line, i);
        if (found_value)
            gear_ratio *= found_value;

#ifdef DEBUG
        printf("Gear ratio for line is %d\n", gear_ratio);
#endif

        sum += gear_ratio;
    }

    return sum;
}

int parse_file(FILE *f) {
    char line0[LINE_LENGTH];
    char line1[LINE_LENGTH];
    char line2[LINE_LENGTH];

    char *lines[] = {line0, line1, line2};
    char *previous_line, *current_line, *next_line;

    uint8_t active_line = 0;
    uint32_t sum = 0;

    // handle first line case
    read_next_line(f, line0);
    read_next_line(f, line1);
    clear_line(line2);

    previous_line = line2;
    current_line = line0;
    next_line = line1;

#ifdef DEBUG
    printf("Initial configuration:\n");
    printf("Line 0: %s\n", line0);
    printf("Line 1: %s\n", line1);
    printf("Line 2: %s\n", line2);
#endif

    sum += parse_line(previous_line, current_line, next_line);

    while (!feof(f)) {
        active_line = (active_line + 1) % 3;
        previous_line = lines[(active_line + 2) % 3];
        current_line = lines[active_line];
        next_line = lines[(active_line + 1) % 3];

        read_next_line(f, next_line);
        
        sum += parse_line(previous_line, current_line, next_line);
    }
    
    // make sure we're not left with an unprocessed line
    clear_line(previous_line);
    sum += parse_line(current_line, next_line, previous_line);

    return sum;
}

int main(void) {
  FILE *f = fopen("input", "r");
  int sum;

  if (!f)
    return -1; // could not open file
  if (feof(f)) {
    fclose(f);
    return -2; // file is empty
  }

  sum = parse_file(f);

  fclose(f);

  printf("Calculated %d\n", sum);
}
