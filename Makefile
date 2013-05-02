#CFLAGS = 
#INCLUDES = 
GTKFLAGS = `pkg-config --cflags --libs gtk+-2.0 gmodule-2.0`

FILES = rag.c \
        ragGui.c

EXECUTABLE = rag

all: rag

rag:; gcc $(FILES) -g -o $(EXECUTABLE) $(CFLAGS) $(INCLUDES) $(GTKFLAGS)
clean:; rm -f rag

