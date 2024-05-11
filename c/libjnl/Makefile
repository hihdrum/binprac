TARGET = libjnl.a

all : $(TARGET)

CC = gcc
CFLAGS += -Wall
CFLAGS += -MMD
CFLAGS += -g3
CFLAGS += -O0

jnl.o : jnl.c
	$(CC) $(CFLAGS) -c $< -o $@

libjnl.a : jnl.o
	ar rcs libjnl.a jnl.o

.PHONY : clean
clean:
	rm -f $(TARGET) *.d *.o

-include *.d
