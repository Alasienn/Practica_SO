ejecutar:exe
	reset
	./exe

exe:Mutex.c
	gcc -o exe p2-dogProgram.c

clear:
	rm -f exe


