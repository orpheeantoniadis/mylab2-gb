CC=srec_cat
CFLAGS=-binary -o
OFLAGS=-C-Array

SRC=$(wildcard *.bmp)
OUT=$(SRC:.bmp=.c)

all: $(OUT)

%.c: %.bmp
	$(CC) $< $(CFLAGS) $@ $(OFLAGS)

clean:
	rm -rf *.c