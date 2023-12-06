CFLAGS=-std=c11 -g -static -Wimplicit-function-declaration \
											-Wformat-security

9cc : 9cc.o object.o token.o parse.o gen.o
	gcc 9cc.o object.o token.o parse.o gen.o -o 9cc

9cc.o : 9cc.c object.h token.h parse.h
	gcc -c 9cc.c

object.o : object.c object.h
	gcc -c object.c

token.o : token.c object.h token.h
	gcc -c token.c

parse.o : parse.c object.h token.h parse.h
	gcc -c parse.c

gen.o : gen.c gen.h object.h parse.h
	gcc -c gen.c

test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean

