CC := g++
CCFLAGS := -Wall -o3
SRC := main.cpp
LDLIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET := 2dgame
all:
	clang-format $(SRC) -i
	$(CC) $(SRC) $(CCFLAGS) -o $(TARGET) $(LDLIBS)
.PHONY: clean
clean:
	rm $(TARGET)