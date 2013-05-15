
LEX=lex
YACC=yacc
CC=gcc

YACCFLAGS=-d -v --debug
CFLAGS=-ansi -Wall -O2 -Wno-unused-function
LDLIBS=-lfl

DEPS=yacc.h tree.h tree_print.h tree_optimize.h

.phony: all clean
.default: all

all: clex cyacc

lex.c: src.lex
	$(LEX) -t $< > $@

yacc.c yacc.h: src.yacc
	$(YACC) $(YACCFLAGS) $<
	mv y.tab.c yacc.c
	mv y.tab.h yacc.h
	mv y.output yacc.log

clex: main.lex.o lex.o
	$(CC) -o $@ $^ $(LDLIBS)

cyacc: main.yacc.o yacc.o lex.o tree.o tree_print.o tree_optimize.o
	$(CC) -o $@ $^ $(LDLIBS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f clex cyacc lex.* yacc.* *.o

