
LEX=lex
YACC=yacc
CC=gcc

LEXX=clex
YACCX=cyacc

TESTFD=tests
TESTFL=$(wildcard $(TESTFD)/*.in)

YACCFLAGS=-d -v --debug
CFLAGS=-ansi -Wall -O2 -Wno-unused-function
LDLIBS=-lfl

DEPS=yacc.h tree.h tree.print.h tree.check.h macro.h 

.phony: run all clean print-header %-yacc %-lex
.default: run

run: print-header all all-lex all-yacc

# -------------------------------------------------------------------------

all: clex cyacc

lex.c: src.lex
	$(LEX) -t $< > $@

yacc.c yacc.h: src.yacc
	$(YACC) $(YACCFLAGS) $<
	mv y.tab.c yacc.c
	mv y.tab.h yacc.h
	mv y.output yacc.log

$(LEXX): main.lex.o lex.o
	$(CC) -o $@ $^ $(LDLIBS)

$(YACCX): main.yacc.o yacc.o lex.o tree.o tree.print.o tree.check.o
	$(CC) -o $@ $^ $(LDLIBS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f clex cyacc lex.* yacc.* *.o $(TESTFD)/*.out $(TESTFD)/*.err

# -------------------------------------------------------------------------

print-header:
	@echo "----------------------------------------"
	@echo "PUC-Rio / Compiladores / Linguagem Monga" 
	@echo "Julio Ribeiro da Silva - 0911409" 
	@echo "----------------------------------------"

%-lex: $(LEXX) $(TESTFD)/%.in
	@./$^

%-yacc: $(YACCX) $(TESTFD)/%.in
	@./$^

all-lex: $(LEXX) $(TESTFL) 
	@echo 
	@$(foreach f, $(TESTFL), \
		echo -n "Testing lex ("; \
		echo -n $(f); \
		echo -n ")... "; \
		./$< $(f) > $(f:.in=.lex.out); \
		diff $(f:.in=.lex.out) $(f:.in=.lex.gab) >/dev/null; \
		if [ $$? -eq 0 ]; then \
			echo "OK!"; \
		else \
			echo "ERROR!"; \
		fi; \
	)

all-yacc: $(YACCX) $(TESTFL) 
	@echo 
	@$(foreach f, $(TESTFL), \
		echo -n "Testing yacc ("; \
		echo -n $(f); \
		echo -n ")... "; \
		./$< $(f) > $(f:.in=.yacc.out); \
		diff $(f:.in=.yacc.out) $(f:.in=.yacc.gab) >/dev/null; \
		if [ $$? -eq 0 ]; then \
			echo "OK!"; \
		else \
			echo "ERROR!"; \
		fi; \
	)

