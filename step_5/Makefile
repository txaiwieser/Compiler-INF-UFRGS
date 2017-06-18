# Makefile

all: y.tab.o lex.yy.o hash.o astree.o semantic.o main.o
	gcc -o etapa4 y.tab.o lex.yy.o hash.o astree.o semantic.o main.o

main.o: main.c
	gcc -c main.c

astree.o: astree.c
	gcc -c astree.c

semantic.o: semantic.c
	gcc -c semantic.c

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
	rm *.o etapa4 lex.yy.h lex.yy.c y.tab.c y.tab.h y.output
