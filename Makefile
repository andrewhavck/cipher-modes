CC=gcc
CFLAGS=-I.

modes: modes.c rijndael-alg-fst.c
	$(CC) -o modes modes.c rijndael-alg-fst.c