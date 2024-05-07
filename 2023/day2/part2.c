#include <stdio.h>
#include <stdint.h>

// struct to hold information about each game
struct game_t {
    uint8_t id;  // game id
    uint8_t red; // max number of each cube color
    uint8_t green;
    uint8_t blue;
};

enum file_field_types {
    FIELD_NONE,
    FIELD_GAME_ID,
    FIELD_RED,
    FIELD_GREEN,
    FIELD_BLUE
};

struct parsed_file_field_t {
    enum file_field_types field_type;
    uint8_t field_value;
};

uint8_t read_number(FILE *f, uint8_t initial_value) {
    char c = fgetc(f);

    while (c > 47 && c < 58) {
        initial_value = initial_value * 10 + (c - '0');
        c = fgetc(f);
    }

    return initial_value;
}

int get_next_field(FILE *f, struct parsed_file_field_t *file_field) {
    uint8_t read_value;
    char c = fgetc(f);
    while (!feof(f)) {
        switch (c) {
            case 'G':
                // reading a game
                fseek(f, 4, SEEK_CUR); // advance 4 positions, to read the game ID
                read_value = read_number(f, 0);

                file_field->field_type = FIELD_GAME_ID;
                file_field->field_value = read_value;
#ifdef DEBUG
                printf("get_text_field: game id %d\n", read_value);
#endif
                return 0;

                break;
            
            case 48 ... 57:
                // color
                read_value = c - '0';
                read_value = read_number(f, read_value);
                file_field->field_value = read_value;
#ifdef DEBUG
                printf("get_text_field: read value %d\n", read_value);
#endif

                c = fgetc(f);
#ifdef DEBUG
                printf("get_text_field: next char after number %c\n", c);
#endif
               switch (c) {  
                    case 'r':
                        file_field->field_type = FIELD_RED;
#ifdef DEBUG
                        printf("get_text_field: red %d\n", read_value);
#endif
                        return 0;

                    case 'g':
                        file_field->field_type = FIELD_GREEN;
#ifdef DEBUG
                        printf("get_text_field: green %d\n", read_value);
#endif
                       return 0;

                    case 'b':
                        file_field->field_type = FIELD_BLUE;
#ifdef DEBUG
                        printf("get_text_field: blue %d\n", read_value);
#endif
                       return 0;
                }
        }

        c = fgetc(f);
    }

    file_field->field_type = FIELD_NONE;
    file_field->field_value = 0;
    return 0;
}

uint8_t read_file(struct game_t *games) {
    FILE *f = fopen("input", "r");

    if (!f)
        return 0; // could not open file
    
    if (feof(f)) {
        fclose(f);
        return 0; // file is empty
    }

    struct parsed_file_field_t file_field;
    uint8_t game_count;
    uint8_t game_index;

    game_count = 0;
    game_index = 0;

    do {
        get_next_field(f, &file_field);

        switch (file_field.field_type) {
            case FIELD_GAME_ID:
                if (game_count == UINT8_MAX) {
                    // we have already processed UINT8_MAX worth of games
                    goto toomanygames;
                }

                game_count++;
                game_index = game_count - 1;

                games[game_index].id = file_field.field_value;
                games[game_index].red = 0;
                games[game_index].green = 0;
                games[game_index].blue = 0;
                break;

            case FIELD_RED:
                if (file_field.field_value > games[game_index].red)
                    games[game_index].red = file_field.field_value;
                break;

            case FIELD_GREEN:
                if (file_field.field_value > games[game_index].green)
                    games[game_index].green = file_field.field_value;
                break;

            case FIELD_BLUE:
                if (file_field.field_value > games[game_index].blue)
                    games[game_index].blue = file_field.field_value;
        }
    } while (!feof(f));

toomanygames:
    fclose(f);

    return game_count;
}

int main (void) {
    uint8_t game_count;
    struct game_t games[UINT8_MAX - 1]; // uint8 games -1 (255) should be enough
                                        // for all elves

    game_count = read_file(games);

    if (!game_count) {
        return -1; // could not read any games
    }

    printf("read %d games\n", game_count);

    uint32_t maximum_powah = 0;

    for (uint8_t i = 0; i < game_count; i++) {
#ifdef DEBUG
        printf("Game %d: red %d, green %d, blue %d\n", 
               games[i].id, games[i].red, games[i].green, games[i].blue);
#endif
    
        maximum_powah += games[i].red * games[i].green * games[i].blue;
    }

    printf("Sum of powah: %d\n", maximum_powah);
}
