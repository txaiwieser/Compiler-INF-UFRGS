# Makefile

all: y.tab.o lex.yy.o hash.o astree.o semantic.o tac.o compiler.o main.o
	gcc -o etapa6 y.tab.o lex.yy.o hash.o astree.o semantic.o tac.o compiler.o main.o

main.o: main.c
	gcc -c main.c

astree.o: astree.c
	gcc -c astree.c

semantic.o: semantic.c
	gcc -c semantic.c

tac.o: tac.c
	gcc -c tac.c

hash.o: hash.c
	gcc -c hash.c

compiler.o: compiler.c
	gcc -c compiler.c

y.tab.o: y.tab.c lex.yy.c
	gcc -c y.tab.c

y.tab.c: parser.y
	yacc -d -v parser.y

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

lex.yy.c: scanner.l
	lex --header-file=lex.yy.h scanner.l

clean:
	rm *.o etapa6 lex.yy.h lex.yy.c y.tab.c y.tab.h y.output out.txt
