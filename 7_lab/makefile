LIBS = -lm

CC = clang

CFLAGS = -std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
#CFLAGS += -Werror
CFLAGS += -pedantic
CFLAGS += -Wmissing-declarations
CFLAGS += -fstack-protector-all

ASANFLAGS = -fsanitize=address
ASANFLAGS += -fno-common
ASANFLAGS += -fno-omit-frame-pointer

DEBUGFLAGS = -g

#OPTFLAGS += -Ofast

default: test debug

test: src/llrb.c src/test.c 
	$(CC) $(CFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(ASANFLAGS) src/llrb.c src/tuilib.c src/test.c -o test.out $(LIBS)

debug: src/llrb.c src/main.c
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(ASANFLAGS) src/interface.c src/llrb.c src/main.c src/tuilib.c -o debug.out $(LIBS)

clean:
	rm *.out
	rm *.dot
