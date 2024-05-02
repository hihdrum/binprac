TARGET = jnl_ascii_dump

CXX = g++
CXXFLAGS += -Wall -MMD
CXXFLAGS += -g3
CXXFLAGS += -O0

$(TARGET) : jnl.o jnl_ascii_dump.o
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY : clean
clean:
	rm -f a.out $(TARGET) *.d *.o gmon.out *~

-include *.d
