CC := gcc
flags := -std=c11 -Wall -g

all: scheduler

utils.o: utils.c
	$(CC) $(flags) utils.c -c -o utils.o

FIFO.o: FIFO.c
	$(CC) $(flags) FIFO.c -c -o FIFO.o

RR.o: RR.c
	$(CC) $(flags) RR.c -c -o RR.o

SJF.o: SJF.c
	$(CC) $(flags) SJF.c -c -o SJF.o

PSJF.o: PSJF.c
	$(CC) $(flags) PSJF.c -c -o PSJF.o

scheduler: scheduler.c scheduler.h utils.o FIFO.o RR.o SJF.o PSJF.o
	$(CC) $(flags) scheduler.c utils.o FIFO.o RR.o SJF.o PSJF.o -o scheduler

clean:
	rm -f scheduler utils.o FIFO.o RR.o SJF.o PSJF.o