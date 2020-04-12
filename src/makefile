CC=@ gcc
CFLAGS=--std=c99 --pedantic -Wall -Wextra -Wmissing-prototypes -O3 
LD=@ gcc
LDFLAGS=

# Files
EXEC=slimming
@MODULES=mainSlimming.c PNM.c slimming.c
OBJECTS=mainSlimming.o PNM.o slimming.o


## Rules

all: $(EXEC)

slimming: $(OBJECTS)
	$(LD) -o $(EXEC) $(OBJECTS) $(LDFLAGS)

mainSlimming.o: mainSlimming.c
	$(CC) -c mainSlimming.c -o mainSlimming.o $(CFLAGS)

PNM.o: PNM.c
	$(CC) -c PNM.c -o PNM.o $(CFLAGS)

slimming.o: slimming.c
	$(CC) -c slimming.c -o slimming.o $(CFLAGS)

clean:
	rm -f *.o $(EXEC) *~
