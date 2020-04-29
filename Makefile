CFLAG = -DDEBUG -Wall -std=c99

main: driver.o schedpolicy.o process.o
	gcc $(CFLAG) driver.o schedpolicy.o process.o -o main
main.o: driver.c Makefile
	gcc $(CFLAG) driver.c -c
schedpolicy.o: schedpolicy.c schedpolicy.h Makefile
	gcc $(CFLAG) schedpolicy.c -c
process.o: process.c process.h Makefile
	gcc $(CFLAG) process.c -c
clean:
	rm -rf *o
run:
	sudo ./main
