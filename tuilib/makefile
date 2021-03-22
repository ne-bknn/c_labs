default: build

.PHONY: build
build: test.c tuilib.h
	clang -Wextra -Wall -fsanitize=address -o test.out *.c

.PHONY: clean
clean:
	-rm *.out
