TARGET = make_dummy_data

CC = gcc

CFLAGE += -Wall
CFLAGE += -MMD
CFLAGE += -g3 -O0

all : $(TARGET)

raw_jnl_header.o : raw_jnl_header.c
	$(CC) $(CFLAGE) -c $< -o $@

raw_jnl_record.o : raw_jnl_record.c
	$(CC) $(CFLAGE) -c $< -o $@

$(TARGET).o : $(TARGET).c
	$(CC) $(CFLAGE) -c $< -o $@

$(TARGET) : $(TARGET).o raw_jnl_header.o raw_jnl_record.o
	$(CC) $(CFLAGE) $^ -o $@

run : $(TARGET)
	./$(TARGET)

.PHONY : clean
clean :
	rm -f *.o *.d a.out $(TARGET) data???

-include *.d
