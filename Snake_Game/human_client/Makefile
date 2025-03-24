CFLAGS = -g -lzmq -lcurses -pthread

SOURCES := $(wildcard *.c)
HEADERS := $(wildcard *.h)
EXECUTABLE = human-client

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	gcc $^ -o $@ $(CFLAGS)

clean:
	rm -f $(EXECUTABLE)