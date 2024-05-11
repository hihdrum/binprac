TARGET = jnl_ascii_dump

CC = gcc
CFLAGS += -Wall
CFLAGS += -MMD
CFLAGS += -g3
CFLAGS += -O0

$(TARGET) : jnl_ascii_dump.o jnl_ascii_dump_imp.o jnl.o
	$(CC) $(CFLAGS) $^ -o $@

.PHONY : clean
clean:
	rm -f a.out $(TARGET) *.o *.d

-include *.d
