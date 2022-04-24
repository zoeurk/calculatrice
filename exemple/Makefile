CC=gcc
ARG=-g -Wall -Wpadded -Wextra

all:libparser.so liboperation.so
	$(CC) $(ARG) calcule.c calcule-data.c -o calcule -L. -lparsearg -loperation -lm
	$(CC) $(ARG) ntest.c -o ntest -L. -lparsearg

libparser.so:
	$(CC) $(ARG) -fPIC -c parsearg.c
	$(CC) $(ARG) -o libparsearg.so -shared parsearg.o
	
liboperation.so:
	$(CC) $(ARG) -fPIC -c operation.c
	$(CC) $(ARG) -o liboperation.so -shared operation.o

clean:
	rm -fv libparsearg.so parsearg.o liboperation.so operation.o calcule ntest
