TARGET = jnl_ascii_dump

CC = gcc
CFLAGS += -Wall
CFLAGS += -MMD
CFLAGS += -g3
CFLAGS += -O0

$(TARGET) : jnl_ascii_dump.o jnl_ascii_dump_imp.o jnl.o
	$(CC) $(CFLAGS) $^ -o $@

jnl_ascii_dump_po0 : jnl_ascii_dump.c jnl_ascii_dump_imp.c jnl.c
	$(CC) -Wall -MMD -g3 -O0 -pg jnl_ascii_dump.c jnl_ascii_dump_imp.c jnl.c -o $@

jnl_ascii_dump_po2 : jnl_ascii_dump.c jnl_ascii_dump_imp.c jnl.c
	$(CC) -Wall -MMD -g3 -O2 -pg jnl_ascii_dump.c jnl_ascii_dump_imp.c jnl.c -o $@

.PHONY : clean
clean:
	rm -f a.out $(TARGET) *.o *.d gmon.out \
		jnl_ascii_dump_po0 jnl_ascii_dump_po2

-include *.d
