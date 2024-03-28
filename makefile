CC := g++
CCFLAGS := -Wall -o3
SRC := *.cpp
LDLIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET := 2dgame
all:
	$(CC) $(SRC) $(CCFLAGS) -o $(TARGET) $(LDLIBS)
.PHONY: clean
clean:
	rm $(TARGET)