CPP=gcc
CFLAGS=-g -Wall
LDFLAGS=
SOURCES=main.c fsnd_utilities.c fsnd_client.c fsnd_server.c socket.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=fsnd

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CPP) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c Makefile
	$(CPP) $(CFLAGS) -c -o $@ $<

%.o: %.c %.h Makefile
	$(CPP) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o fsnd
