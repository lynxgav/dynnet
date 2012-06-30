include Makefile.inc
dynnet: main.cc *h
	$(CC) -o dynnet main.cc $(LDFLAGS) $(DEBUGFLAGS)


run:	dynnet
	./dynnet $(SEED) > out


bak:

clean:
	rm -f dynnet

	
