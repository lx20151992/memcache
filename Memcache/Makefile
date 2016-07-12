CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
SOURCES=main.cpp Packet.cpp Server.cpp Cache.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=memcache

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

