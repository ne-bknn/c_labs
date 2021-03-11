default: test

.PHONY: test
test: utils.h test.c
	clang -Werror -Wall -Wextra -o test.out test.c
	./test.out

.PHONY: memtest
memtest: utils.h test.c
	clang -Werror -Wall -Wextra -fsanitize=address -o test.out test.c
	./test.out


.PHONY: clean
clean:
	@-rm *.out
