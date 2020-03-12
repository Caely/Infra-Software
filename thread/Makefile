all:
	gcc	serial_group2.c	-o	serial_group2.out
	gcc	pthreads_group2.c	-lpthread	-o	pthreads_group2.out
	gcc	-fopenmp	omp_group2.c	-o	omp_group2.out

run:
	./serial_group2.out	arquivo1.txt	arquivo2.txt
	./pthreads_group2.out	arquivo1.txt	arquivo2.txt
	./omp_group2.out	arquivo1.txt	arquivo2.txt

clean:
	rm	*.out
	rm	compare.txt
	rm	serial.o
	rm	omp.o
	rm	pthreads.o