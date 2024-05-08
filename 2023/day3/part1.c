#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>

#define LINE_LENGTH 141 // should be enough for any elf schematic

/*
 * Solver design for minimal memory usage:
 * 1. keep 3 consecutive lines from the file in buffers
 * 2. scan line: for each part, determine N, S, E, W numbers
 * 3. if N or S neighbor is not found, search for corresponding
 *        NE,NW or SE,SW neighbors.
 *
 * Is it possible that a number is neighbor to two parts?
 */

int clear_line(char *line) {
    return sprintf(line, "%.*s", LINE_LENGTH, ".");
}

int read_next_line(FILE *f, char *line) {
    if (feof(f) || fgets(line, LINE_LENGTH, f) == NULL) {
        clear_line(line);
        return -1;
    }

    return 0;
}

uint32_t parse_number_reverse(char *line, uint16_t position) {
    uint8_t number_of_digits = 0;
    uint32_t sum = 0;

    while (isdigit(line[position])) {
        sum += ((line[position] - '0') * 10 ^ number_of_digits);
        number_of_digits++;

        if (position == 0)
            break;
        position--;
    }

    return sum;
}

uint32_t parse_number_forward(char *line, uint16_t position) {
    uint32_t sum = 0;

    while (isdigit(line[position])) {
        sum += sum * 10 + (line[position] - '0');
        position++;
    }

    return sum;
}

uint32_t parse_number_from_center(char *line, uint16_t position) {
    uint32_t sum = 0;
   
    sum = parse_number_reverse(line, position);

    if(isdigit(line[position + 1])) {
        uint32_t right_part = 0;

        right_part = parse_number_forward(line, position + 1);
        
        if (right_part == 0)
            return sum * 10;

        sum = sum * pow(10, floor(log10(right_part)) + 1) + right_part;
    }

    return sum;
}

int parse_line(char *previous_line, char *current_line, char *next_line) {
    // iterate through line
    // if symbol found check E, W, N, S
    // if no values found in N, S, check NE, NW and SE, SW
    uint32_t sum = 0;

    int is_line_beginning;
    int is_line_end;

    for (uint16_t i = 0; i < LINE_LENGTH; i++) {
        if (current_line[i] == '.' || current_line[i] == '\0')
            continue;

        if (isdigit(current_line[i]))
            continue;

        is_line_beginning = (i == 0);
        is_line_end = (i == LINE_LENGTH - 1);

        // we have a token
        // check the East
        if (!is_line_beginning)
            sum += parse_number_reverse(current_line, i - 1);

        // chech the West
        if (!is_line_end)
            sum += parse_number_forward(current_line, i + 1);

        if (isdigit(previous_line[i])) {
            // get N neighbor
            sum += parse_number_from_center(current_line, i);
        } else {
            // get NE neighbor
            if (!is_line_beginning)
                sum += parse_number_reverse(previous_line, i - 1);

            // get NW neighbor
            if (!is_line_end)
                sum += parse_number_forward(previous_line, i + 1);
        }

        if (isdigit(next_line[i])) {
            // get S neighbor
            sum += parse_number_from_center(current_line, i);
        } else {
            // get SE neighbor
            if (!is_line_beginning)
                sum += parse_number_reverse(next_line, i - 1);

            // get SW neighbor
            if (!is_line_end)
                sum += parse_number_forward(next_line, i + 1);
        }
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
