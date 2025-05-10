#include <stdint.h>
#include <stdio.h>
#include <termios.h>
#include "modes.h"

#define LENGTH 30000

uint8_t turing[LENGTH] = {0};

int main(int argc, char* argv[]) {
    FILE *fp = fopen(argv[1], "r");

    if (!fp) {
        perror("No file path provided");
        return 1;
    }

    int operand;
    int current_cell = 0;
    int loop_start;

    raw_mode();

    while ((operand = fgetc(fp)) != EOF) {
        switch ((char) operand) {
            case '>':
                current_cell++;
                break;
            
            case '<':
                current_cell--;
                break;

            case '+':
                if (turing[current_cell] == 255) {
                    turing[current_cell] = 0;
                } else {
                    turing[current_cell]++;
                }
                break;
            
            case '-':
                if (turing[current_cell] == 0) {
                    turing[current_cell] = 255;
                } else {
                    turing[current_cell]--;
                }
                break;

            case '.':
                printf("%c", (char) turing[current_cell]);
                break;

            case ',':
            {
                char ch;
                while (read(STDIN_FILENO, &ch, 1) == 1) {
                    if ((int) ch >= 0 && (int) ch <= 127) {
                        break;
                    }
                }
                turing[current_cell] = (int) ch;
            }

            case '[':
                break;
            
            case ']':
                break;
        }
    }

    return 0;
}