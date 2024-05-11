TARGET = jnl_ascii_dump

CC = gcc
CFLAGS += -Wall
CFLAGS += -g3
CFLAGS += -O0

$(TARGET) : jnl_ascii_dump.o asciiDump_v01.o jnl.o
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET)_v2 : jnl_ascii_dump.o asciiDump_v02.o jnl.o
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET)_v3 : jnl_ascii_dump.o asciiDump_v03.o jnl.o
	$(CC) $(CFLAGS) $^ -o $@

.PHONY : clean
clean:
	rm -f a.out $(TARGET) *.o gmon.out $(TARGET)_v2 $(TARGET)_v3
