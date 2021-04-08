LIBS = -lm

CC = clang

CFLAGS = -std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -Wmissing-declarations
CFLAGS += -fstack-protector-all

ASANFLAGS = -fsanitize=address
ASANFLAGS += -fno-common
ASANFLAGS += -fno-omit-frame-pointer

DEBUGFLAGS = -g

VECTORFLAGS = -fopenmp-simd
VECTORFLAGS += -Rpass="loop|vect"
VECTORFLAGS += -Rpass-missed="loop|vect"
VECTORFLAGS += -Rpass-analysis="loop|vect"

OPTFLAGS = -ffast-math
OPTFLAGS += -march=native
OPTFLAGS += -O3

UNSAFEOPTS = -mllvm -unroll-allow-partial -mllvm -unroll-runtime -funsafe-math-optimizations

default: debug build

debug: strassen.c test_strassen.c
	$(CC) $(CFLAGS) $(VECTORFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(ASANFLAGS) strassen.c test_strassen.c -o debug.out $(LIBS)

build: strassen.c test_strassen.c
	$(CC) $(CFLAGS) $(VECTORFLAGS) $(OPTFLAGS) $(UNSAFEOPTS) strassen.c test_strassen.c -o bench.out $(LIBS)

clean:
	rm *.out
