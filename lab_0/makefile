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

.PHONY: build 
default: build

.PHONY: test
test: tests.out
	@./tests.out

.PHONY: autotest
autotest: autotests.out
	@./autotests.out

.PHONY: memcheck
memcheck: test/*.c src/*.c src/*.h
	@echo Running memcheck
	@$(CC) $(ASANFLAGS) $(CFLAGS) src/*lib.c test/vendor/unity.c test/*.c -o memcheck.out $(LIBS)
	@./memcheck.out
	@echo "Memory check passed"

tests.out: test/*.c src/*.h
	@echo Building tests 
	@$(CC) $(CFLAGS) src/*lib.c src/*.h test/vendor/unity.c test/*.c -o tests.out $(LIBS)

test/autotest/autotest.c: test/autotest/generate_test.py test/autotest/template.jinja
	@echo Generating autotest.c
	cd test/autotest && python generate_test.py

autotests.out: test/autotest/autotest.c
	@echo Compiling autotest.c
	@$(CC) $(CFLAGS) src/*lib.c src/*.h test/vendor/unity.c test/autotest/*.c -o autotests.out $(LIBS)

.PHONY: build
build: src/*.c src/*.h
	@echo Building
	@$(CC) $(CFLAGS) src/*.c -o task.out $(LIBS) 

.PHONY: clean
clean:
	@echo Deleting *.out *.o autotests
	-@rm *.out 2>/dev/null || true
	-@rm *.o 2>/dev/null || true
	-@rm test/autotest/autotest.c 2>/dev/null || true

.PHONY: valgrind
valgrind: task.out
	cat test/testcases/testcase1 | valgrind --leak-check=full \
         --show-leak-kinds=all --track-origins=yes --verbose \
         --log-file=logs/valgrind-out.txt ./task.out
