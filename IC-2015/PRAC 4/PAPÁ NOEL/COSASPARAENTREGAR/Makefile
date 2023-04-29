CC=mpicc

main-p4: main-p4.o
	$(CC) -o main-p4 main-p4.o -g3 -ljpeg -lm
	
main-p4.o: main-p4.c
	$(CC) -c main-p4.c -o main-p4.o -g3 -ljpeg -lm

clean:
	rm -rf *.o main-p4
	rm -rf salida/*.jpg
