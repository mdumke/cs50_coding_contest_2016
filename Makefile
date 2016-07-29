wait_for_it: wait_for_it.c
	clang -ggdb3 -O0 -std=c11 -Wall wait_for_it.c -o wait_for_it cs50.o

