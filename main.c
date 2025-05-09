#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define LENGTH 30000
#define STACK_SIZE 1024

struct termios oldt, newt;

uint8_t turing[LENGTH] = {0};
int loop_stack[STACK_SIZE];
int stack_ptr = -1;

void raw_mode() {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    cfmakeraw(&newt);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void reset_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
};

int main(int argc, char* argv[]) {
    FILE *fp = fopen(argv[1], "r");

    if (!fp) {
        perror("Failed to open file");
        return 1;
    }
    
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    rewind(fp);

    char* code = malloc(size + 1);
    fread(code, 1, size, fp);
    code[size] = '\0';

    size_t pc = 0;
    int current_cell = 0;
    int loop_start;

    raw_mode();

    for (int i = 0; i < size; i++) {
        switch (code[i]) {
            case '>':
                if (current_cell >= 29999) {
                    current_cell = 0;
                } else {
                    current_cell++;
                }
                break;
            
            case '<':
                if (current_cell <= 0) {
                    current_cell = 29999;
                } else {
                    current_cell--;
                }
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
                break;
            }

            case '[':
                if (turing[current_cell]) {
                    loop_stack[++stack_ptr] = i;
                } else {
                    int open = 1;
                    while (open) {
                        i++;
                        if (code[i] == '[') {
                            open++;
                        }
                        if (code[i] == ']') {
                            open--;
                        }
                    }
                }
                break;
            
            case ']':
                if (turing[current_cell]) {
                    i = loop_stack[stack_ptr];
                } else {
                    stack_ptr--;
                }
                break;
        }
    }

    free(code);
    fclose(fp);
    reset_terminal();

    return 0;
}