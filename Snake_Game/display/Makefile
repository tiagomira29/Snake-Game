SOURCES := $(wildcard *.c)
HEADERS := $(wildcard *.h)
EXECUTABLE = display

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	gcc $^ -o $@ -g -lzmq -lcurses

clean:
	rm -f $(EXECUTABLE)