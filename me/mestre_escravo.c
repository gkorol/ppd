#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define TAM_TRAB 10

#define TAG_DONE 666

/*
*
*	TODO[OK]: quicksort (sempre com pior caso) do math.h
*						10K vetores de 100K elementos
* TODO[OK]: Manter ordem do saco
* TODO[OK]: Zero copy: mpi_probe
* TODO: Iniciativa no escravo (após primeiro "round"). Se nao tem trabalho, escravo cai fora; se proc_n-1 pediram trab e nao tinha, mestre cai fora
* 			Trabalho devolvido pode ser um pedido de novo trabalho...
* TODO: (relatorio) pode usar ./time e/ou medidas no codigo
*
*/

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a > *(int*)b );
}

main(int argc, char** argv)
  {
  int i;
  int j;
  int slave;
  int my_rank;			// Identificador deste processo
  int proc_n;				// Numero de processos disparados pelo usuario na linha de comando (np)  
  int * saco;				// saco de trabalho    
  int * message;
  int ret[TAM_TRAB];
	int TAM_TOTAL;
	int trab_rem;
	int done;
	int kill_cnt;
	int *current_trab;
	
	TAM_TOTAL = atoi(argv[1]);

  MPI_Status status; // estrutura que guarda o estado de retorno          
        
  MPI_Init(&argc , &argv); // funcao que inicializa o MPI, todo o codigo paralelo estah abaixo

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pega pega o numero do processo atual (rank)
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);  // pega informacao do numero de processos (quantidade total)

  if ( my_rank == 0 ) {
	
		kill_cnt = 0;
		trab_rem = TAM_TOTAL;

		saco = malloc( TAM_TOTAL * TAM_TRAB * sizeof(int) );
		current_trab = malloc( (proc_n-1) * sizeof(int) );

		for( i = 0; i < TAM_TOTAL; ++i ) {
			for( j = 0; j < TAM_TRAB; ++j ) {
				saco[i*TAM_TRAB+j] = (j-TAM_TRAB)*(-1) + i;
			}
		}

		printf("\nMestre[%d] antes:\n", my_rank);               
		for( i = 0; i < TAM_TOTAL; ++i ) {
			printf("linha %2d: [ ", i);
			for( j = 0; j < TAM_TRAB; ++j ) {
				printf("%2d ",saco[i*TAM_TRAB+j]);
			}
			printf(" ]\n");
		}
 
		for ( i=1 ; i < proc_n ; i++) {
			message = &(saco[(i-1)*TAM_TRAB]);
			MPI_Send(message, TAM_TRAB, MPI_INT, i, 1, MPI_COMM_WORLD);
			trab_rem--;
			current_trab[i-1] = i-1;
		}

		printf("\n");
		while(kill_cnt < (proc_n-1)) {			
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			slave = status.MPI_SOURCE;	

			MPI_Recv(&saco[current_trab[slave-1]*TAM_TRAB], TAM_TRAB, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

			// Envia se tiver trab sobrando
			if( trab_rem > 0 ) {
				message = &(saco[ (TAM_TOTAL - trab_rem) * TAM_TRAB]);
				MPI_Send(message, TAM_TRAB, MPI_INT, slave, 1, MPI_COMM_WORLD);
				current_trab[slave-1] = TAM_TOTAL - trab_rem;
				trab_rem--;				
			} else {
				MPI_Send(message, 1, MPI_INT, slave, TAG_DONE, MPI_COMM_WORLD);
				kill_cnt++;
			}
		}
		
		printf("\n");

		printf("\nMestre[%d] depois:\n", my_rank);               
		for( i = 0; i < TAM_TOTAL; ++i ) {
			printf("linha %2d: [ ", i);
			for( j = 0; j < TAM_TRAB; ++j ) {
				printf("%2d ",saco[i*TAM_TRAB+j]);
			}
			printf(" ]\n");
		}

   } else {
		// papel do escravo

		done = 0;
		saco = malloc(TAM_TRAB * sizeof(int));

		while(!done) {
	
			MPI_Recv(saco, TAM_TRAB, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

			if(status.MPI_TAG == TAG_DONE) {
				done = 1;
			} else {
				qsort(saco, TAM_TRAB, sizeof(int), cmpfunc);

				MPI_Send(saco, TAM_TRAB, MPI_INT, 0, 1, MPI_COMM_WORLD);
			}

		}
	}
     
  MPI_Finalize();
}
