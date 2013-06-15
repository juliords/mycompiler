
LEX=lex
YACC=yacc
CC=gcc

YACCFLAGS=-d -v --debug
CFLAGS=-ansi -Wall -O0 -Wno-unused-function -g
LDLIBS=-lfl

LEXS=src.lex
YACCS=src.yacc

DEPS=yacc.h tree.h tree.check.h tree.print.h macro.h 
SRCS=main.c lex.c yacc.c tree.c tree.check.c tree.print.c

YACCX=exec

TESTFD=tests
TESTFL=$(wildcard $(TESTFD)/*.in)

# -------------------------------------------------------------------------

.phony: run all clean again print-header test-%
.default: run

run: print-header all test-all

all: $(YACCX)

lex.c: $(LEXS)
	$(LEX) -t $< > $@

yacc.c yacc.h: $(YACCS)
	$(YACC) $(YACCFLAGS) $<
	mv y.tab.c yacc.c
	mv y.tab.h yacc.h
	mv y.output yacc.log

$(YACCX): $(SRCS:.c=.o)
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
	./$+

test-all: $(YACCX) 
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

