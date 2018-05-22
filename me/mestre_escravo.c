#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define TAM_TRAB 15

void ordena(int * vec, int size) {
  int aux, i, j;

  for(i = 0; i < size; ++i) {
    for(j = 0; j< size; ++j) {
      if (vec[i] < vec[j]) {
	aux = vec[j];
        vec[j] = vec[i];
        vec[i] = aux;
      }
    }
  }
}

main(int argc, char** argv)
  {
  int i;
  int j;
  int my_rank;       // Identificador deste processo
  int proc_n;        // Numero de processos disparados pelo usuario na linha de comando (np)  
  int * saco;      // saco de trabalho    
  int * message;
  int ret[TAM_TRAB];
  MPI_Status status; // estrutura que guarda o estado de retorno          
        
  MPI_Init(&argc , &argv); // funcao que inicializa o MPI, todo o codigo paralelo estah abaixo

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pega pega o numero do processo atual (rank)
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);  // pega informacao do numero de processos (quantidade total)

  srand(time(NULL));

  if ( my_rank == 0 ) // qual o meu papel: sou o mestre ou um dos escravos?
     {
     // papel do mestre
     // inicializo o saco de trabalho
     saco = malloc(proc_n * TAM_TRAB * sizeof(int));

     for( i = 0; i < proc_n; ++i ) {
       for( j = 0; j < TAM_TRAB; ++j ) {
	   saco[i*TAM_TRAB+j] = rand()%TAM_TRAB;
       }
     }

     // mostro o saco

     printf("\nMestre[%d]:\n", my_rank);               
     for( i = 0; i < proc_n; ++i ) {
       printf("linha %d: ", i);
       for( j = 0; j < TAM_TRAB; ++j ) {
	   printf("%3d ",saco[i*TAM_TRAB+j]);
       }
       printf("\n");
     }
 
     // mando o trabalho para os escravos fazerem

     for ( i=1 ; i < proc_n ; i++)
         {
         message = &(saco[(i-1)*TAM_TRAB]);
         MPI_Send(message, TAM_TRAB, MPI_INT, i, 1, MPI_COMM_WORLD); // envio trabalho saco[i-1] para escravo com id = i
         }
     
     // recebo o resultado

// USAR MPI PROBE PARA JA APONTAR O RECV PARA LINHA DO ESCRAVO i 

     printf("\n");
     for ( i=1 ; i < proc_n ; i++)
         {
         // recebo mensagens de qualquer emissor e com qualquer etiqueta (TAG)

         MPI_Recv(ret, TAM_TRAB, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

	 printf("Recv de %d: ", status.MPI_SOURCE);
         for( i = 0; i < TAM_TRAB; ++i ) {
	   printf("%3d ", ret[i]);
         }
         printf("\n");

         }
     }                 
  else                 
     {
     // papel do escravo

     // inicializo o saco de trabalho
     saco = malloc(TAM_TRAB * sizeof(int));

     // recebo mensagem

     MPI_Recv(saco, TAM_TRAB, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

     // executo opera��o
     ordena(saco, TAM_TRAB);
     // retorno resultado para o mestre

     MPI_Send(saco, TAM_TRAB, MPI_INT, 0, 1, MPI_COMM_WORLD);
     }
     
  MPI_Finalize();
}
