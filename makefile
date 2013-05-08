
LEX=lex
YACC=yacc
CC=gcc

LEXFLAGS=
YACCFLAGS=-d -v --debug
CFLAGS=-ansi -O2
LDLIBS=-lfl

DEPS=yacc.h tree.h

.phony: all clean
.default: all

all: clex cyacc

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lex.c: src.lex yacc.h
	$(LEX) -t $< > $@

yacc.c yacc.h: src.yacc
	$(YACC) $(YACCFLAGS) $<
	mv y.tab.c yacc.c
	mv y.tab.h yacc.h
	mv y.output yacc.log

clex: main.lex.o lex.o tree.o
	$(CC) -o $@ $^ $(LDLIBS)

cyacc: main.yacc.o yacc.o lex.o tree.o 
	$(CC) -o $@ $^ $(LDLIBS)

clean:
	rm -f clex cyacc lex.* yacc.* *.o

