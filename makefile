
LEX=lex
YACC=yacc
CC=gcc

LEXFLAGS=
YACCFLAGS=-d -v --debug
CFLAGS=-ansi -Wall -g -O2
LDLIBS=-lfl

.phony: all clean
.default: all

all: clex cyacc

lex.c: src.lex yacc.h
	$(LEX) -t src.lex > $@

yacc.%: src.yacc
	$(YACC) $(YACCFLAGS) $<
	mv y.tab.c $(@:.h=.c)
	mv y.tab.h $(@:.c=.h)
	mv y.output yacc.log

clex: main.lex.c lex.c tree.c
	$(CC) main.lex.c lex.c tree.c -o $@ $(LDLIBS)

cyacc: main.yacc.c yacc.c lex.c tree.c 
	$(CC) main.yacc.c yacc.c lex.c tree.c -o $@ $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f clex cyacc lex.* yacc.*

