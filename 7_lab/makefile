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

DEBUGFLAGS = -pg

default: build

# Unity testing
.PHONY: libtest
libtest: test/*.c src/*.h 
	@echo Running vanilla tests
	@$(CC) $(CFLAGS) src/*lib.c src/*.h test/vendor/unity.c test/*.c -o tests.out $(LIBS)
	@./tests.out

.PHONY: libtest-mem
libtest-mem: test/*.c src/*.c src/*.h
	@echo Running ASAN 
	@$(CC) $(ASANFLAGS) $(CFLAGS) src/*lib.c test/vendor/unity.c test/*.c -o asan.out $(LIBS)
	@ASAN_OPTIONS=detect_leaks=1 ./asan.out
	@echo "ASAN passed"
	@echo Running MSAN
	@$(CC) $(MSANFLAGS) $(CLAGS) src/*lib.c test/vendor/unity.c test/*.c -o msan.out $(LIBS)
	@./msan.out
	@echo "MSAN passed"

# Full testing
.PHONY: sanitize
sanitize: src/*.c src/*.h
	@echo Running ASAN 
	@$(CC) $(ASANFLAGS) $(CFLAGS) src/*.c -o asan.out $(LIBS)
	@ASAN_OPTIONS=detect_leaks=1 ../tools/run_tests.sh asan.out
	@echo "ASAN passed"
	@echo Running MSAN
	@$(CC) $(MSANFLAGS) $(CFLAGS) src/*.c -o msan.out $(LIBS)
	@../tools/run_tests.sh msan.out
	@echo "MSAN passed"
	@$(CC) $(UBSANFLAGS) $(CFLAGS) src/*.c -o ubsan.out $(LIBS)
	@../tools/run_tests.sh ubsan.out
	@echo "UBSAN passed"

.PHONY: build
build: src/*.c src/*.h
	@echo Building
	#@$(CC) $(ASANFLAGS) $(CFLAGS) $(DEBUGFLAGS) src/*.c -o task.out $(LIBS) 
	@$(CC) $(CFLAGS) $(DEBUGFLAGS) src/*.c -o task.out $(LIBS) 

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

.PHONY: valgrind
valgrind: task.out
	@bash ../tools/run_valgrind.sh

.PHONY: afl-build
afl-build: src/*.c src/*.h
	@AFL_HARDEN=1 afl-clang-fast $(AFLFLAGS) $(CFLAGS) src/*.c -o fuzz.out $(LIBS)

fuzz.out: afl-build

.PHONY: afl-start
afl-start: fuzz.out
	@AFL_PRELOAD=/home/bakunin/Tools/AFL/libdislocator/libdislocator.so afl-fuzz -i fuzzing/in -x fuzzing/dict -o fuzzing/sync -M fuzz1 -- ./fuzz.out
