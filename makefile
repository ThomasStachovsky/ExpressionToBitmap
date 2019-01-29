CFLAGS=-std=c11  -Wall  -Wextra  -Werror
all: exptoimg
exptoimg: main.o io.o bitmap.o tree.o utilities.o
	gcc $(CFLAGS) -o exptoimg $^
main.o: io.h
io.o: io.h bitmap.h
bitmap.o: bitmap.h tree.h
tree.o: tree.h utilities.h
utilities.o: utilities.h

rmo: exptoimg
	rm -f *.o

clean: 
	rm -f *.o
	rm -f exptoimg

run: exptoimg
	./exptoimg

