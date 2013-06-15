
LEX=lex
YACC=yacc
CC=gcc

YACCX=cyacc

TESTFD=tests
TESTFL=$(wildcard $(TESTFD)/*.in)

YACCFLAGS=-d -v --debug
CFLAGS=-ansi -Wall -O2 -Wno-unused-function
LDLIBS=-lfl

DEPS=yacc.h tree.h tree.print.h tree.check.h macro.h 

.phony: run all clean again print-header test-%
.default: run

run: print-header all testall

# -------------------------------------------------------------------------

all: $(YACCX)

lex.c: src.lex
	$(LEX) -t $< > $@

yacc.c yacc.h: src.yacc
	$(YACC) $(YACCFLAGS) $<
	mv y.tab.c yacc.c
	mv y.tab.h yacc.h
	mv y.output yacc.log

$(YACCX): main.yacc.o yacc.o lex.o tree.o tree.print.o tree.check.o
	$(CC) -o $@ $^ $(LDLIBS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(YACCX) lex.* yacc.* *.o $(TESTFD)/*.out $(TESTFD)/*.err

again: clean all

# -------------------------------------------------------------------------

print-header:
	@echo "----------------------------------------"
	@echo "PUC-Rio / Compiladores / Linguagem Monga" 
	@echo "Julio Ribeiro da Silva - 0911409" 
	@echo "----------------------------------------"

test-%: $(YACCX) $(TESTFD)/%.in
	@./$+

testall: $(YACCX) 
	@echo 
	@$(foreach f, $(TESTFL), \
		echo -n "Testing file '"; \
		echo -n $(f); \
		echo -n "'... "; \
		./$< $(f) 1>$(f:.in=.out) 2>$(f:.in=.err); \
		diff $(f:.in=.out) $(f:.in=.gab) >/dev/null; \
		if [[ ( $$? -eq 0 ) && ( ! -s  $(f:.in=.err) ) ]]; then \
			echo "OK!"; \
			rm $(f:.in=.out) $(f:.in=.err); \
		else \
			echo "ERROR!"; \
		fi; \
	)

