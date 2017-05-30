MYLIBDIR =../mylib
MYLIB = $(MYLIBDIR)/mylib.a
CFLAGS = -I$(MYLIBDIR)
OBJS1 = server.o sessionman.o
OBJS2 = client.o session.o

all: bin bin/s bin/c

bin:
	mkdir $@

bin/s: $(OBJS1)
	$(CC) -g -o $@ $^ $(MYLIB) -lncurses

bin/c: $(OBJS2)
	$(CC) -g -o $@ $^ $(MYLIB) -lncurses

server.o: race.h sessionman.h

client.o: race.h session.h

data_ed.o checker.o session.o sessionman.o: race.h

clean:
	$(RM) bin/s bin/c *.o *~