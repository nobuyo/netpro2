MYLIBDIR =../mylib
MYLIB = $(MYLIBDIR)/mylib.a
CFLAGS = -I$(MYLIBDIR)

all: bin bin/s bin/c

bin:
	mkdir $@

bin/s: server.o goban.o
	$(CC) -o $@ $^ $(MYLIB)

bin/c: client.o goban.o
	$(CC) -o $@ $^ $(MYLIB)

server.o client.o: goban.h

clean:
	$(RM) bin/s bin/c *.o *~