#CFLAGS = 
#INCLUDES = 
GTKFLAGS = `pkg-config --cflags --libs gtk+-2.0 gmodule-2.0`

all: rag

rag:; gcc -g $(CFLAGS) $(INCLUDES) $(GTKFLAGS) rag.c ragGui.c -o rag
clean:; rm -f rag

