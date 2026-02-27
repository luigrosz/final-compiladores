compiler: main.o lex.o synt.o gen.o symbol.o
	gcc -o compiler main.o lex.o synt.o gen.o symbol.o -I. -g

main.o: main.c
	gcc -c -g main.c

lex.o: lex.c
	gcc -c -g lex.c

synt.o: synt.c
	gcc -c -g synt.c

gen.o: gen.c
	gcc -c -g gen.c

symbol.o: symbol.c
	gcc -c -g symbol.c

clean:
	rm -f *.o compiler
