#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <omp.h>

struct args_v {
	int size;
	int* linha;
	int** matrix;
	int nlinha;
	int** resultado;
};

typedef struct args_v ARGS;


int* len(char* file01,char* file02) {

	int* sizes = malloc(sizeof(int)*2);

	sizes[0] = 0;
	sizes[1] = 0;

	FILE* fp = fopen(file01,"r");

	char c;

	for (c = getc(fp); c != EOF; c = getc(fp)) {

		if (c == '\n')
			sizes[0] += 1;
	}

	fclose(fp);


	fp = fopen(file02,"r");

	for (c = getc(fp); c != EOF; c = getc(fp)) {

		if (c == '\n')
			sizes[1] += 1;
	}

	fclose(fp);

	return sizes;
}


ARGS* makeStructs(int** resultado,int** matrix,int** matrix2,int nthreads) {

	ARGS* struct_array;
	struct_array = malloc(sizeof(struct args_v)*nthreads);
	
	for(int i = 0;i < nthreads;i++) {
		struct_array[i].nlinha = i;
		struct_array[i].size = nthreads;
		struct_array[i].linha = matrix[i];
		struct_array[i].matrix = matrix2;
		struct_array[i].resultado = resultado;
	}

	return struct_array;
}

void printaMatrix(int** resultado,int nthreads) {

	for(int i = 0;i < nthreads;i++) {
		for(int j = 0;j < nthreads;j++) {
			printf("%d ",resultado[i][j]);
		}
		printf("\n");
	}

}


void appendCompareTXT(char* modelo,double time, int memory) {

	FILE* fp;
	fp = fopen("compare.txt","a");
	fprintf(fp,"\n\n%s\n",modelo);
	fprintf(fp,"Tempo: %f\n",time);
	fprintf(fp, "Memória: %d\n", memory);

}

void printaMatrix(int** resultado,int nthreads);
int* len(char* file01,char* file02);
ARGS* makeStructs(int** resultado,int** matrix,int** matrix2,int nthreads);