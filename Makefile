all: execute

execute: cpu.o execute.o

execute.o: execute.c cpu.h
cpu.o: cpu.c cpu.h

clean:
	rm *.o execute
