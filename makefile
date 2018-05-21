ejecutar:exe 3535
	reset
	./exe

exe: p2-dogServer.c
	gcc p2-dogServer.c -o exe -lpthread

ejecutarC:exeC
	reset
	./exeC
	
exeC: p2-dogClient.c
	gcc p2-dogClient.c -o exeC -lpthread

clear:
	rm -f exe
