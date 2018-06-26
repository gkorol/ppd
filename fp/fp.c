#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define PRINT
#define BS

#define KILL 666
#define COMPARA 222
#define TROCA 111

#define ORDENADO 1
#define NAO_ORDENADO 0

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a > *(int*)b );
}

void bs(int n, int * vetor)
{
    int c=0, d, troca, trocou =1;

    while (c < (n-1) & trocou )
        {
        trocou = 0;
        for (d = 0 ; d < n - c - 1; d++)
            if (vetor[d] > vetor[d+1])
                {
                troca      = vetor[d];
                vetor[d]   = vetor[d+1];
                vetor[d+1] = troca;
                trocou = 1;
                }
        c++;
        }
}

void printVector(int *saco, int tam)
{
	int i;
	for(i=0;i<tam;i++){
		printf("%2d ", saco[i]);
	}
	printf("\n");
}

void killAll(int my_rank, int proc_n) {
  int i;

  for(i=0; i<proc_n; i++){
    if(i != my_rank) {
      MPI_Send(&i, 1, MPI_INT, i, KILL, MPI_COMM_WORLD);
    }
  }

}


main(int argc, char** argv) {

  int i;
  int * saco;
  int tam = atoi(argv[1]); // Tamanho do saco
	int delta = atoi(argv[2]); // Tamanho do vetor a ser trocado  

  int pronto;
  int maior_vizinho;
  int estado;
	int processos_ordenados;

  int my_rank;
  int proc_n;
  MPI_Status status;
  double t1,t2;

  // Alocacao do vetor local
  saco = malloc((tam + delta) * sizeof(int));

  MPI_Init(&argc , &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

  if(status.MPI_TAG == KILL) {
    printf("Lazy bye from %d \n", my_rank);
    MPI_Finalize();
    exit(0);
  }

  // Iicializa o vetor principal com o pior caso para ordenacao
  for( i = 0; i < tam; ++i ) {
    saco[i] = (i-tam)*(-1);
  }

  pronto = 0;
  while(!pronto){
	bs(tam, saco);
 
	if(my_rank < proc_n-1){
		// manda direita
  		MPI_Send(&saco[tam-1],1, MPI_INT, my_rank+1, COMPARA, MPI_COMM_WORLD);
	}

	if(my_rank > 0){
		// recebe da esquerda
		MPI_Recv(&maior_vizinho, 1, MPI_INT, my_rank-1, COMPARA, MPI_COMM_WORLD, &status);
	}	

	if(saco[0] > maior_vizinho) // estou ordenado
		estado = ORDENADO;
	else  // nao estou ordenado
		estado = NAO_ORDENADO;

	MPI_Bcast(&estado,1, MPI_INT, my_rank, MPI_COMM_WORLD);
	
	processos_ordenados = 0;

	for(i=0;i<proc_n-1;i++){
			if(i != my_rank){
				MPI_Bcast(&estado,1, MPI_INT, i, MPI_COMM_WORLD);
			
				if(estado == ORDENADO){
					processos_ordenados++;
				}

			}
	}

	if(processos_ordenados == proc_n-2){
		pronto = 1;
	} else {

		if(my_rank < proc_n-1){
			// manda da esquerda
			MPI_Send(saco,delta, MPI_INT, my_rank+1, TROCA, MPI_COMM_WORLD);
		}	
		
		if(my_rank > 0){
			// recebe direita
			MPI_Recv(&saco[tam],delta, MPI_INT, my_rank-1, TROCA, MPI_COMM_WORLD, &status);
		}

		bs(2*delta, saco[tam-delta]);

		if(my_rank < proc_n-1){
			MPI_Send(&saco[tam],delta, MPI_INT, my_rank+1, TROCA, MPI_COMM_WORLD);
		}	

		if(my_rank > 0){
			// recebe direita
			MPI_Recv(saco,delta, MPI_INT, my_rank-1, TROCA, MPI_COMM_WORLD, &status);
		}

	}


  }
  

  MPI_Finalize();
  exit(0);
}

