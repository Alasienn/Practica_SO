ejecutarS:exeS
	reset
	./exeS

exeS:p2-dogServer.c
	gcc -o exe p2-dogServer.c -lpthread

ejecutarC:exeC
	reset
	./exeC

exeC:p2-dogClient.c
	gcc -o exe p2-dogClient.c -lpthread

clear:
	rm -f exe*
