TARGET=ir-verbs

CC=gcc
WARN=-Wall -Wextra -Wpedantic
PTHREAD=-pthread
CCFLAGS=$(WARN) $(PTHREAD) -pipe -std=c99

ifdef DEBUG
	CCFLAGS+=-g -O0
endif

LD=gcc
LDFLAGS=$(PTHREAD)

OBJS=main.o verb.o util.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

main.o: src/main.c src/verb.h src/util.h src/color.h
	$(CC) -c $(CCFLAGS) src/main.c -o main.o

verb.o: src/verb.c src/verb.h src/util.h
	$(CC) -c $(CCFLAGS) src/verb.c -o verb.o

util.o: src/util.c src/util.h
	$(CC) -c $(CCFLAGS) src/util.c -o util.o

clean:
	rm -f *.o $(TARGET)
