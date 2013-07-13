
CC=gcc
CFLAGS=-m32 -O0

SRCS=var_local.c var_constant.c var_global.c loop_for.c \
	func.c if.c local_char.c param.c malloc.c
ASM_FILTER=.text .file .cfi .LF .size .ident .align .section

TEMPF=temp.s

# avoid exclusion of intermediary files
.SECONDARY:

.phony: all alls clean again

all: $(SRCS:.c=.bin)

alls: $(SRCS:.c=.s)

%.s: %.c
	$(CC) $(CFLAGS) -S $< -o $@
	@cat $@ $(foreach w, $(ASM_FILTER), | sed '/$(w)/d') > $(TEMPF)
	@mv $(TEMPF) $@

%.bin: %.s main.s
	$(CC) $(CFLAGS) -o $@ $^

clean:  
	rm -rf *.s *.bin

