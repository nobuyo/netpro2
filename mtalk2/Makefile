MYLIBDIR =../mylib
MYLIB = $(MYLIBDIR)/mylib.a
CFLAGS = -I$(MYLIBDIR)
OBJS1 = server.o sessionman.o
OBJS2 = client.o session.o

all: bin bin/s bin/c

serve:
	./bin/s

client:
	./bin/c

bin:
	mkdir $@

bin/s: $(OBJS1)
	$(CC) -g -o $@ $^ $(MYLIB) -lncurses

bin/c: $(OBJS2)
	$(CC) -g -o $@ $^ $(MYLIB) -lncurses

server.o: mtalk2.h sessionman.h

client.o: mtalk2.h session.h

sessionman.o session.o: mtalk2.h

clean:
	$(RM) bin/s bin/c *.o *~