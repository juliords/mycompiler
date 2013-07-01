
CC=gcc
CFLAGS=

SRCS=var_local.c var_constant.c

.SECONDARY:
.phony: clean

all: $(SRCS:.c=.bin)

%.s: %.c
	$(CC) $(CFLAGS) -S $<

%.bin: %.s
	$(CC) $(CFLAGS) -o $@ $< main.c

clean:  
	rm -rf *.s *.bin

