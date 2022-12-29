CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11

s21_grep: s21_grep.c s21_grep.h
	$(CC) -c $(CFLAGS) s21_grep.c s21_grep.h
	$(CC) $(CFLAGS) *.o -o s21_grep
	make clean
test:
	sh s21_grep_test.sh
style:
	cp ../../materials/linters/.clang-format ./
	clang-format -n *.c
	clang-format -n *.h
clean:
	rm -rf *.o
	rm -rf *.gch
