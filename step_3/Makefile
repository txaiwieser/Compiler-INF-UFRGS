# Makefile

# etapa1: lex.yy.o hash.o main.o
#	gcc lex.yy.o hash.o main.o -o etapa1

etapa2: y.tab.o lex.yy.o hash.o main.o
	gcc -o etapa2 y.tab.o lex.yy.o hash.o main.o

main.o: main.c
	gcc -c main.c

hash.o: hash.c
	gcc -c hash.c

y.tab.o: y.tab.c lex.yy.c
	gcc -c y.tab.c

y.tab.c: parser.y
	yacc -d -v parser.y

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

lex.yy.c: scanner.l
	lex --header-file=lex.yy.h scanner.l

clean:
	rm *.o etapa2 lex.yy.h lex.yy.c y.tab.c y.tab.h y.output
