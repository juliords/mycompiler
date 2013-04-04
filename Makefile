
CC=gcc
LEX=lex
YACC=yacc

LEXIN=lex.lex
LEXOUT=lex.c

LEXFLAGS=-t
YACCFLAGS=-ly
CFLAGS=-c
CEXTLINK=-lfl -lm
LFLAGS=-O2

SOURCES=$(LEXOUT)
OBJECTS=$(SOURCES:.c=.o)
TESTFILES=./tests/*.in
EXEC=compiler

all: $(LEXOUT) $(EXEC)

clean:
	rm $(LEXOUT) $(OBJECTS) $(EXEC)

$(LEXOUT): $(LEXIN)
	$(LEX) $(LEXFLAGS) $< >$@

$(EXEC): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(CEXTLINK)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

