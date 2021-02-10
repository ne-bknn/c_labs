LIBS = -lm

CFLAGS = -std=c99
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -Werror
CFLAGS += -Wmissing-declarations
CFLAGS += -DUNITY_SUPPORT_64

ASANFLAGS = -fsanitize=address
ASANFLAGS += -fno-common
ASANFLAGS += -fno-omit-frame-pointer

.PHONY: test
default: build

test: tests.out
	@./tests.out

.PHONY: memcheck
memcheck: test/*.c src/*.c src/*.h
	@echo Running memcheck
	@$(CC) $(ASANFLAGS) $(CFLAGS) src/*_lib.c test/vendor/unity.c test/*.c -o memcheck.out $(LIBS)
	@./memcheck.out
	@echo "Memory check passed"

tests.out: test/*.c src/*.h
	@echo Building tests 
	@$(CC) $(CFLAGS) src/*_lib.c src/*.h test/vendor/unity.c test/*.c -o tests.out $(LIBS)

.PHONY: build
build: src/*.c src/*.h
	@echo Building
	@$(CC) $(CFLAGS) src/*.c -o task.out $(LIBS) 

.PHONY: clean
clean:
	@echo Deleting *.out *.o
	-@rm *.out 2>/dev/null || true
	-@rm *.o 2>/dev/null || true
