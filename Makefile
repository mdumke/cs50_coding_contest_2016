wait_for_it: wait_for_it.c
	clang -ggdb3 -O0 -std=c11 -Wall wait_for_it.c -o wait_for_it cs50.o

airport_arbitrage: airport_arbitrage.c
	clang -ggdb3 -O0 -std=c11 airport_arbitrage.c -o airport_arbitrage cs50.o -lm

maze_runner: maze_runner.c
	clang -ggdb3 -O0 -std=c11 maze_runner.c -o maze_runner cs50.o

x: x.c
	clang -ggdb3 -O0 -std=c11 x.c -o x cs50.o

word_reverse: word_reverse.c
	clang -ggdb3 -O0 -std=c11 word_reverse.c -o word_reverse cs50.o

money: money.c
	clang -ggdb3 -O0 -std=c11 money.c -o money cs50.o -lm

one_day_more: one_day_more.c
	clang -ggdb3 -O0 -std=c11 one_day_more.c -o one_day_more cs50.o

name_your_cat: name_your_cat.c
	clang -ggdb3 -O0 -std=c11 name_your_cat.c -o name_your_cat cs50.o

