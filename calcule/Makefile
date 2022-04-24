CC=gcc
ARG=-g -Wall -Wpadded -Wextra

all:libparser.so liboperation.so
	$(CC) $(ARG) calcule/calcule.c calcule/calcule-data.c -o calcule-v1 -L. -lparsearg -loperation -lm
	$(CC) $(ARG) ntest/ntest.c -o ntest-v1 -L. -lparsearg

libparser.so:
	$(CC) $(ARG) -fPIC -c ./parsearg/parsearg.c
	$(CC) $(ARG) -o libparsearg.so -shared parsearg.o
	
liboperation.so:
	$(CC) $(ARG) -fPIC -c ./operation/operation.c
	$(CC) $(ARG) -o liboperation.so -shared operation.o

clean:
	rm -fv libparsearg.so parsearg.o liboperation.so operation.o ./calcule-v1 ./ntest-v1
