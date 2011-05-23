CXXFLAGS=-Wall -O2
LDFLAGS=-lstdc++

TARGET=brainfuck

$(TARGET): main.cc
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: clean all
clean:
	$(RM) *.o $(TARGET)

all: $(TARGET)
