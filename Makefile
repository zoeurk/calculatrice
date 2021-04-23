CC=gcc
ARG=-g -Wall -Wpadded -Wextra

all:libparser.so
	 $(CC) $(ARG) calcule.c libcalcule.c -o calcule -lm -L. -lparsearg

libparser.so:
	$(CC) $(ARG) -fPIC -c parsearg.c
	$(CC) $(ARG) -o libparsearg.so -shared parsearg.o

clean:
	rm -fv libparsearg.so parsearg.o calcule
