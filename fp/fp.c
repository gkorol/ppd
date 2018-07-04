#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpi.h"

//#define PRINT
#define BS

#define KILL 666
#define COMPARA 222
#define TROCA 111

#define ORDENADO 1
#define NAO_ORDENADO 0

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

main(int argc, char** argv) {

  int i;
  int * saco;
  int tam = atoi(argv[1]); // Tamanho do saco
	int delta = atoi(argv[2]); // Tamanho do vetor a ser trocado
	int trocas = 0;
	int its = 0;

  int pronto;
  int menor_vizinho;
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

  // Iicializa o vetor principal com o pior caso para ordenacao
  for( i = 0; i < tam; ++i ) {
    saco[i] = (i-tam)*(-1)+(tam*(proc_n-my_rank));
  }

  pronto = 0;
  while(!pronto){
	its++;
    #ifdef BS
    bs(saco, tam);
    #else
    qsort(saco, tam, sizeof(int), cmpfunc);
    #endif

    #ifdef PRINT
		printf("[%d] Vetor ordenado local:", my_rank);
		printVector(saco, tam);
		printf("\n");
    fflush(stdout);
    #endif

    if(my_rank > 0){
			// Manda o seu menor valor para vizinho da esquerda
  		MPI_Send(&saco[0],1, MPI_INT, my_rank-1, COMPARA, MPI_COMM_WORLD);
		}

		if(my_rank < proc_n-1){
			// Recebe o menor valor da direita
			MPI_Recv(&menor_vizinho, 1, MPI_INT, my_rank+1, COMPARA, MPI_COMM_WORLD, &status);
		}

		// Inicializa vetor do estado global
		memset(estado_procs, '0', proc_n);

		if(my_rank < proc_n-1){
			if(menor_vizinho >= saco[tam-1]) {
        // Se meu maior eh menor que o menor do vizinho, estou ordenado
				estado_procs[my_rank] = ORDENADO;
        #ifdef PRINT
				printf("[%d] Ordenado. maior local = %d, menor de %d = %d \n",
        my_rank, saco[tam-1], my_rank+1,menor_vizinho);
        #endif
			}
			else {
        // Se nao, nao estou ordenado
				estado_procs[my_rank] = NAO_ORDENADO;
        #ifdef PRINT
				printf("[%d] NAO ordenado. maior local = %d, menor de %d = %d \n",
        my_rank, saco[tam-1], my_rank+1, menor_vizinho);
        #endif
			}
		} else {
      // Ultimo proc sempre ordenado (nao tem vizinho na direita)
			estado_procs[my_rank] = ORDENADO;
		}

		for(i=0;i<proc_n-1;i++)
			MPI_Bcast(&estado_procs[i],1, MPI_INT, i, MPI_COMM_WORLD);


	  for(ordenados=0;ordenados<proc_n-1;ordenados++){
		    if( estado_procs[ordenados] == NAO_ORDENADO )
			     break;
	  }

	  if(ordenados == proc_n-1){
		    pronto = 1;
	  } else {

  		if(my_rank > 0){
  			// manda delta posicoes do meu vetor para esquerda se for preciso
        if(estado_procs[my_rank-1] == NAO_ORDENADO) {
    			MPI_Send(saco, delta, MPI_INT, my_rank-1, TROCA, MPI_COMM_WORLD);
					trocas++;
          #ifdef PRINT
          printf("[%d] Mandei de zero a delta-1 para %d: ", my_rank, my_rank-1);
    			printVector(saco, delta);
    			printf("\n");
          #endif

      	  MPI_Recv(saco,delta, MPI_INT, my_rank-1, TROCA, MPI_COMM_WORLD, &status);
          #ifdef PRINT
          printf("[%d] Recebi em 0 a delta-1 de %d: ", my_rank, my_rank-1);
    			printVector(saco, delta);
    			printf("\n");
          #endif

        }
  		}

  		if(my_rank < proc_n-1){
  			// recebe delta posicoes da direita se precisar
        if(estado_procs[my_rank] == NAO_ORDENADO) {
    			MPI_Recv(&saco[tam],delta, MPI_INT, my_rank+1, TROCA, MPI_COMM_WORLD, &status);
          #ifdef PRINT
          printf("[%d] Recebi de tam a delta de %d: ", my_rank, my_rank-1);
    			printVector(&saco[tam], delta);
    			printf("\n");
          #endif

          #ifdef BS
          bs(&saco[tam-delta], 2*delta);
          #else
          qsort(&saco[tam-delta], 2*delta, sizeof(int), cmpfunc);
          #endif

          MPI_Send(&saco[tam],delta, MPI_INT, my_rank+1, TROCA, MPI_COMM_WORLD);
          #ifdef PRINT
    			printf("[%d] Ordenei e mandei de tam a delta para %d:", my_rank, my_rank+1);
    			printVector(&saco[tam-delta], 2*delta);
    			printf("\n");
          #endif

        }
  		}

	 }

  }

  #ifdef PRINT
	printf("[%d] Vetor final: ", my_rank);
	printVector(saco, tam);
	printf("\n");
  fflush(stdout);
  #endif
	
	printf("[%d] Trocas: %d\n", my_rank, trocas);
	printf("[%d] Iteracoes: %d\n", my_rank, its);

  MPI_Finalize();
  exit(0);
}
