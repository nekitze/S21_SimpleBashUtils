CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11

s21_cat: s21_cat.c s21_cat.h
	$(CC) -c $(CFLAGS) s21_cat.c s21_cat.h
	$(CC) -g $(CFLAGS) *.o -o s21_cat
	make clean
test:
	sh s21_cat_test.sh
style:
	cp ../../materials/linters/.clang-format ./
	clang-format -n *.c
	clang-format -n *.h
clean:
	rm -rf *.o
	rm -rf *.gch
