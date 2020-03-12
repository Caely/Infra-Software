#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "header.h"
#include <sys/resource.h>


int** getMatrix(char* file,int size);
void* multMatrix(void* t1);
void escreveMatrix(int** resultado,int size);

struct rusage r_usage;


//==================[MAIN]===============

char* matrixTXT = "pthreads.o";

int main(int argc,char* argv[]) {

	clock_t inicio = clock();
	double time = 0.0;

//pega o tamanho da matriz
	int* threads = len(argv[1],argv[2]);
	int nthreads = threads[0];

	if(threads[0] != threads[1]) {
		printf("Impossível multiplicar!\n");
		exit(1);
	}

//pega cada matrix
	int** matrix = getMatrix(argv[1],nthreads);
	int** matrix2 = getMatrix(argv[2],nthreads);

//aloca espaço pro resultado
	int** resultado = malloc(sizeof(int*)*nthreads);

	for(int i = 0;i < nthreads;i++) 
			resultado[i] = malloc(sizeof(int)*nthreads);


//cria e aloca as srtucts

	pthread_t tid[nthreads];

	ARGS* struct_array = makeStructs(resultado,matrix,matrix2,nthreads);

//multiplica as matrizes separadamente por linha
	for(int i = 0;i < nthreads;i++) 
		pthread_create(&tid[i],NULL,multMatrix,&struct_array[i]);


	for(int i = 0;i < nthreads;i++)
		pthread_join(tid[i],NULL);


//printa a matriz final
//	printaMatrix(resultado,nthreads[0]);


	clock_t fim = clock();
	time += (double)(fim-inicio)/CLOCKS_PER_SEC;
	printf("%f\n",time);

	int memory = getrusage(RUSAGE_SELF,&r_usage); 
    printf("Memoria %ld\n",r_usage.ru_maxrss);
	appendCompareTXT("Pthread:",time,r_usage.ru_maxrss);
	escreveMatrix(resultado,nthreads);
//	printf("\n");
//	printaMatrix(resultado,nthreads[0]);
	return 0;
}


//=========[FUNCOES]=================


int** getMatrix(char* file,int size) {

	FILE* fp = fopen(file,"r");

	char letra[100];

	int** matrix = malloc(sizeof(int*) * size);

	for(int i = 0; i < size;i++)
		matrix[i] = malloc(sizeof(int) * size);

	for(int i = 0; i < size;i++) {
		for(int j = 0;j < size;j++) {
			fscanf(fp,"%s",letra);
			matrix[i][j] = atoi(letra);
		}
	}

	return matrix;
}

void* multMatrix(void* t1) {

    struct args_v* t2 = (struct args_v *)t1;

    int size = t2->size;
    int** matrix = t2->matrix;
    int* linha = t2->linha;
    int** resultado = t2->resultado;
    int nlinha = t2->nlinha;

	int aux = 0;

	for(int i = 0;i < size;i++) {
		for(int j = 0;j < size;j++) {
			aux += linha[j] * matrix[j][i];
		}
		resultado[nlinha][i] = aux;
		aux = 0;
	}
}

void escreveMatrix(int** resultado,int size) {

	FILE* fp;

	int** matrix = resultado;

	fp = fopen(matrixTXT,"w");

	for(int i = 0;i < size;i++) {
		for(int j = 0;j < size;j++) {
			fprintf(fp,"%d ",resultado[i][j]);
		}
		fprintf(fp,"%s","\n");
	}
}