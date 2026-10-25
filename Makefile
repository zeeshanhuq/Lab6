CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES= main.c vectorops.c vectorstore.c vectorui.c vectorutil.c vectorfile.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=vect
all: $(SOURCES) $(EXECUTABLE)
-include $(OBJECTS:.o=.d)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
.c.o:
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > $*.d
clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) *.d