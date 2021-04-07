LIBS = -lm

CC = clang

CFLAGS = -std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
#CFLAGS += -Werror
CFLAGS += -Wmissing-declarations
CFLAGS += -fstack-protector-all

ASANFLAGS = -fsanitize=address
ASANFLAGS += -fno-common
ASANFLAGS += -fno-omit-frame-pointer

MSANFLAGS = -fsanitize=memory
MSANFLAGS += -fno-omit-frame-pointer
MSANFLAGS += -fsanitize-memory-use-after-dtor
MSANFLAGS += -fsanitize-memory-track-origins=2

UBSANFLAGS = -fsanitize=undefined

UNITYFLAGS = -DUNITY_SUPPORT_64

AFLFLAGS = -Ofast

DEBUGFLAGS = -g

VECTORFLAGS = -fopenmp-simd
VECTORFLAGS += -ffast-math
VECTORFLAGS += -march=native
VECTORFLAGS += -Rpass="loop|vect"
VECTORFLAGS += -Rpass-missed="loop|vect"
VECTORFLAGS += -Rpass-analysis="loop|vect"
VECTORFLAGS += -O3 

default: debug

.PHONY: debug
debug: src/*/*.c src/*/*.h
	@echo 'Building for debug'
	@$(CC) $(CFLAGS) $(ASANFLAGS) $(DEBUGFLAGS) $(VECTORFLAGS) src/matrix/strassen.c src/matrix/test_strassen.c -o strassen.out $(LIBS)

.PHONY: build
build: src/*/*.c src/*/*.h
	@echo Building
	#@$(CC) $(ASANFLAGS) $(CFLAGS) $(DEBUGFLAGS) src/*.c -o task.out $(LIBS) 
	@$(CC) $(CFLAGS) -O3 src/matrix/strassen.c src/matrix/test_strassen.c -o strassen.out $(LIBS) 

.PHONY: python
python: src/*/*.c src/*/*.h
	@echo "Building&Installing python module"
	@cd src/pystrassen && python setup.py install --user

task.out: build

.PHONY: clean
clean:
	@echo Deleting *.out *.o autotests logs core.* fuzzing/sync/*
	-@rm *.out 2>/dev/null || true
	-@rm *.o 2>/dev/null || true
	-@rm test/autotest/autotest.c 2>/dev/null || true
	-@rm logs/* 2>/dev/null || true
	-@rm core.* 2>/dev/null || true
	-@rm -r fuzzing/sync/* 2>/dev/null || true

.PHONY: afl-build
afl-build: src/*.c src/*.h
	@AFL_HARDEN=1 afl-clang-fast $(AFLFLAGS) $(CFLAGS) src/matrix/strassen.c src/matrix/test_strassen.c -o fuzz.out $(LIBS)

fuzz.out: afl-build

.PHONY: afl-start
afl-start: fuzz.out
	@AFL_PRELOAD=/home/bakunin/Tools/AFL/libdislocator/libdislocator.so afl-fuzz -i fuzzing/in -x fuzzing/dict -o fuzzing/sync -M fuzz1 -- ./fuzz.out
