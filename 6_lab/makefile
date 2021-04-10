LIBS = -lm
LIBS += -lpthread

CC = clang

CFLAGS = -std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -Wmissing-declarations
CFLAGS += -fstack-protector-all
CFLAGS += -D_POSIX_C_SOURCE=199309L

ASANFLAGS = -fsanitize=address
ASANFLAGS += -fno-common
ASANFLAGS += -fno-omit-frame-pointer

DEBUGFLAGS = -g

VECTORFLAGS = -fopenmp-simd
#VECTORFLAGS += -Rpass="loop|vect"
#VECTORFLAGS += -Rpass-missed="loop|vect"
#VECTORFLAGS += -Rpass-analysis="loop|vect"

OPTFLAGS = -ffast-math
OPTFLAGS += -march=native
OPTFLAGS += -mtune=native
OPTFLAGS += -O3
OPTFLAGS += -Ofast

VECTFORCE = -mllvm -force-vector-width=8
VECTFORCE += -mllvm -force-vector-interleave=4

UNSAFEOPTS = -mllvm -unroll-allow-partial
UNSAFEOPTS += -mllvm -unroll-runtime 
UNSAFEOPTS += -funsafe-math-optimizations

.PHONY: default
default: debug build

.PHONY: debug
debug: src/strassen.c src/strassen_multithreaded.c src/test_strassen.c
	$(CC) $(CFLAGS) $(VECTORFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(ASANFLAGS) src/strassen.c src/test_strassen.c -o debug_vanilla.out $(LIBS)
	$(CC) $(CFLAGS) $(VECTORFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(ASANFLAGS) src/strassen_multithreaded.c src/test_strassen.c -o debug_pthread.out $(LIBS)

.PHONY: build
build: src/strassen.c src/strassen_multithreaded.c src/test_strassen.c
	$(CC) $(CFLAGS) $(VECTORFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) src/strassen.c src/bench_strassen.c -o bench_vanilla.out $(LIBS)
	$(CC) $(CFLAGS) $(VECTORFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) src/strassen_multithreaded.c src/bench_strassen.c -o bench_pthread.out $(LIBS)

	$(CC) $(CFLAGS) $(VECTORFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(ASANFLAGS) src/strassen.c src/test_strassen.c -o test_vanilla.out $(LIBS)
	$(CC) $(CFLAGS) $(VECTORFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(ASANFLAGS) src/strassen_multithreaded.c src/test_strassen.c -o test_pthread.out $(LIBS)

	$(CC) $(CFLAGS) $(VECTORFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(ASANFLAGS) src/strassen.c src/demo_strassen.c -o demo_vanilla.out $(LIBS)
	$(CC) $(CFLAGS) $(VECTORFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(ASANFLAGS) src/strassen_multithreaded.c src/demo_strassen.c -o demo_pthread.out $(LIBS)

bench_vanilla.out: build
bench_pthread.out: build
demo_vanilla.out: build
demo_pthread.out: build
test_vanilla.out: build
test_pthread.out: build

.PHONY: bytecode
bytecode: strassen.c test_strassen.c
	$(CC) -std=c99 $(VECTORFLAGS) $(OPTFLAGS) -emit-llvm strassen.c -S

run_vanilla: bench_vanilla.out demo_vanilla.out test_vanilla.out
	./demo_vanilla.out
	./test_vanilla.out
	./bench_vanilla.out

run_pthread: bench_pthread.out demo_pthread.out test_pthread.out
	./demo_pthread.out
	./test_pthread.out
	./bench_pthread.out


clean:
	rm *.out
