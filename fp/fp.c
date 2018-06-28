#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpi.h"

#define PRINT
#define BS

#define KILL 666
#define COMPARA 222
#define TROCA 111

#define ORDENADO 1
#define NAO_ORDENADO 0

/*
* Ordenar pior caso global
* analise de dc contra fp
*/

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a > *(int*)b );
}

void bs(int * vetor, int n)
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
	int *estado_procs;
	int ordenados;

  int my_rank;
  int proc_n;
  MPI_Status status;
  double t1,t2;

  // Alocacao do vetor local
  saco = malloc((tam + delta) * sizeof(int));

	estado_procs = malloc(proc_n * sizeof(int));

  MPI_Init(&argc , &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

  //if(status.MPI_TAG == KILL) {
  //  printf("Lazy bye from %d \n", my_rank);
  //  MPI_Finalize();
  //  exit(0);
  //}

  // Iicializa o vetor principal com o pior caso para ordenacao
  for( i = 0; i < tam; ++i ) {
    saco[i] = (i-tam)*(-1)+(tam*(proc_n-my_rank));
  }

  pronto = 0;
  while(!pronto){

		bs(saco, tam);
		printf("[%d] Vetor ordenado local:", my_rank);
		printVector(saco, tam);
		printf("\n");
 	
		if(my_rank < proc_n-1){
			// manda direita
			printf("[%d] Mandei saco[%d] para %d... \n", my_rank, 0, my_rank+1);
  		MPI_Send(&saco[0],1, MPI_INT, my_rank+1, COMPARA, MPI_COMM_WORLD);
		}

		if(my_rank > 0){
			// recebe da esquerda
			MPI_Recv(&maior_vizinho, 1, MPI_INT, my_rank-1, COMPARA, MPI_COMM_WORLD, &status);
			printf("[%d] Recebi %d de proc %d... \n", my_rank, maior_vizinho, my_rank-1);
		}	

		// Inicializa estado global
		memset(estado_procs, '0', proc_n);
	
		if( my_rank > 0) {
			if(saco[0] >= maior_vizinho) { // estou ordenado
				estado_procs[my_rank] = ORDENADO;
				printf("[%d] Estou ordenado... \n", my_rank);
			}
			else {  											// nao estou ordenado
				estado_procs[my_rank] = NAO_ORDENADO;
				printf("[%d] NAO Estou ordenado (%d >= %d)... \n", my_rank, saco[0], maior_vizinho);
			}
		} else {
			estado_procs[0] = ORDENADO;
		}

		for(i=0;i<proc_n-1;i++){
			MPI_Bcast(&estado_procs[i],1, MPI_INT, i, MPI_COMM_WORLD);
		}

	for(ordenados=0;ordenados<proc_n-1;ordenados++){
		if( estado_procs[ordenados] == NAO_ORDENADO )
			break;
	}

	if(ordenados == proc_n-1){
		pronto = 1;
	} else {

		if(my_rank > 0){
			// manda da esquerda
			printf("[%d] Mandei delta para esquerda: ", my_rank);
			MPI_Send(saco,delta, MPI_INT, my_rank-1, TROCA, MPI_COMM_WORLD);
			printVector(saco, delta);
			printf("\n");
		}	
		
		if(my_rank < proc_n-1){
			// recebe esquerda
			printf("[%d] Recebi delta da direita: ", my_rank);
			MPI_Recv(&saco[tam],delta, MPI_INT, my_rank+1, TROCA, MPI_COMM_WORLD, &status);
			printVector(&saco[tam], delta);
			printf("\n");

			bs(&saco[tam-delta], 2*delta);
			printf("[%d] Vetor ordenado local + delta:", my_rank);
			printVector(&saco[tam-delta], 2*delta);
			printf("\n");
		}

		if(my_rank < proc_n-1){
			MPI_Send(&saco[tam],delta, MPI_INT, my_rank+1, TROCA, MPI_COMM_WORLD);
		}	

		if(my_rank > 0){
			// recebe direita
			MPI_Recv(&saco[tam-delta],delta, MPI_INT, my_rank-1, TROCA, MPI_COMM_WORLD, &status);
		}

	}


  }
  

	printf("[%d] Vetor final: ", my_rank);
	printVector(saco, tam);
	printf("\n");

  MPI_Finalize();
  exit(0);
}

