#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "header.h"
#include <sys/resource.h>
#include <mpi.h>

struct rusage r_usage;

int** getMatrix(char* file,int size);
void* multMatrix(ARGS t1,int argc, char* argv[]);
void escreveMatrix(int** resultado,int size);

//==================[MAIN]===============

char* matrixTXT = "serial.o";

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

	ARGS* struct_array = makeStructs(resultado,matrix,matrix2,nthreads);

//multiplica as matrizes separadamente por linha
	for(int i = 0;i < nthreads;i++)
		multMatrix(struct_array[i],argc,argv);


//printa a matriz final
//	printaMatrix(resultado,nthreads[0]);


	clock_t fim = clock();
	time += (double)(fim-inicio)/CLOCKS_PER_SEC;
	printf("%f\n",time);

	int memory = getrusage(RUSAGE_SELF,&r_usage); 
    printf("Memoria %ld\n",r_usage.ru_maxrss);
	appendCompareTXT("Serial:",time,memory);
//	printf("\n");
//	printaMatrix(resultado,nthreads[0]);

	escreveMatrix(resultado,nthreads);

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

void* multMatrix(ARGS t1, int argc, char *argv[]) {
	
    int size = t1.size;
    int** matrix = t1.matrix;
    int* linha = t1.linha;
    int** resultado = t1.resultado;
    int nlinha = t1.nlinha;

	int aux = 0;

    int tamanho, mensagem, rank, metade;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &tamanho);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int inicio = 0;
    for(inicio=0;inicio<size;inicio++){
        if(inicio == rank){
            metade = size * (inicio + 1) / rank;
            for(int i = 0;i < metade;i++) {
                for(int j = 0;j < size;j++) {
                    aux += linha[j] * matrix[j][i];
                }
                resultado[nlinha][i] = aux;
                aux = 0;
                MPI_Send(&resultado[nlinha][i],size,MPI_INT,tamanho-1,1,MPI_COMM_WORLD);
            }
        }
    }
    if(tamanho-1 == rank){
        for(int i = 0; i < metade;i++){
            for(int j = 0; j < metade;j++){
                if(i==rank){
                    MPI_Recv(&resultado[i][j],size, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }
        }
    }
    MPI_Finalize();
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