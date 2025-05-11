CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC = src/main.c
OUT = bf

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OUT)