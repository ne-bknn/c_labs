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

DEBUGFLAGS = -g

default: prod 

.PHONY: prod
prod: 
	@echo Building
	@$(CC) $(CFLAGS) $(DEBUGFLAGS) *.c ../tuilib.c -o test.out $(LIBS) 

.PHONY: build
build: *.c *.h
	@echo Building
	@$(CC) $(ASANFLAGS) $(CFLAGS) -Werror $(DEBUGFLAGS) *.c ../tuilib.c -o test.out $(LIBS) 

.PHONY: clean
clean:
	@echo Deleting *.out *.o autotests logs core.* fuzzing/sync/* *.dot
	-@rm *.out 2>/dev/null || true
	-@rm *.o 2>/dev/null || true
	-@rm test/autotest/autotest.c 2>/dev/null || true
	-@rm logs/* 2>/dev/null || true
	-@rm core.* 2>/dev/null || true
	-@rm -r fuzzing/sync/* 2>/dev/null || true
	-@rm *.dot 2>/dev/null || true
