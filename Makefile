#CFLAGS = 
#INCLUDES = 
GTKFLAGS = `pkg-config --cflags --libs gtk+-2.0 gmodule-2.0`

all: ra

ra:; gcc -g $(CFLAGS) $(INCLUDES) $(GTKFLAGS) ra.c raGui.c -o ra
clean:; rm -f ra

