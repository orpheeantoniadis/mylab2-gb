CC = gcc
INCDIR = inc
SRCDIR = src

XFLAGS = -Wall -Wextra
CFLAGS = -g -I$(INCDIR) $(XFLAGS) -std=gnu99 -O3
LDFLAGS = -lSDL2
DFLAGS = -D__UNIX

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:.c=.o)
OBJECTS = $(OBJ:$(SRCDIR)/%=%)
EXEC = gameboy

all: $(EXEC) mostlyclean

$(EXEC): $(OBJ)
	$(CC) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

%o: %c
	$(CC) $(CFLAGS) $(DFLAGS) -c $^

run: $(EXEC)
	./$<

mostlyclean:
	rm -rf *.o *~ *.dSYM

clean:
	rm -rf $(EXEC) *.o *~ *.dSYM

rebuild: clean all