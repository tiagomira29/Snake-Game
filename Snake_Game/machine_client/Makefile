SOURCES := $(wildcard *.c)
HEADERS := $(wildcard *.h)
EXECUTABLE = machine-client

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	gcc $^ -o $@ -g -lzmq -lcurses

clean:
	rm -f $(EXECUTABLE)