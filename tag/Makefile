MYLIBDIR =../mylib
MYLIB = $(MYLIBDIR)/mylib.a
CFLAGS = -I$(MYLIBDIR)

all: bin bin/s bin/c

bin:
	mkdir $@

bin/s: server.o session.o
	$(CC) -o $@ $^ $(MYLIB) -lncurses

bin/c: client.o session.o
	$(CC) -o $@ $^ $(MYLIB) -lncurses

server.o client.o: tag.h session.h

clean:
	$(RM) bin/s bin/c *.o *~