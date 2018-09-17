CDEBUGFLAGS = -O2 -Wall -g -DGTK_DISABLE_DEPRECATED=1 -DGDK_DISABLE_DEPRECATED=1 -DG_DISABLE_DEPRECATED=1
CFLAGS = ${CDEBUGFLAGS} `pkg-config --cflags gtk+-3.0`
LDLIBS = `pkg-config --libs gtk+-3.0`
LDFLAGS = -g

all : sachy 

sachy : sachy.o logika.o figurky.o
	$(CC) $(LDFLAGS) -o sachy sachy.o logika.o figurky.o $(LDLIBS)


clean :
	rm -f sachy.o sachy logika.o figurky.o
