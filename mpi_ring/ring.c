#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "mpi.h"

#define ELEICAO_TAG 9
#define FALHA_TAG   99
#define NOVO_COORD  999
#define KILL        666

#define MAX_BUFFER  100

void envia_kill(int my_rank, int proc_n) {
  if (my_rank == proc_n-1)
    MPI_Send(&my_rank, 1, MPI_INT, 1, KILL, MPI_COMM_WORLD);
  else
    MPI_Send(&my_rank, 1, MPI_INT, my_rank+1, KILL, MPI_COMM_WORLD);

  // MPI_Send(&my_rank, 1, MPI_INT, 0, KILL, MPI_COMM_WORLD);
}

void init_urna(int * urna, int size) {
  int i;
  for (i=0; i<size; i++) {
    urna[i] = 0;
  }
}

int computa_urna(int * urna, int proc_n) {
  int max = 0;
  int i;

  for(i=proc_n; i>0; i--) {
    if (urna[i] == 1) {
      // Retorna o primeiro proc valido
      max = i;
      break;
    }
  }
  return max;
}

void print_urna(int *urna, int proc_n) {
  int i;

  printf("Urna: ");
  for(i=1; i<proc_n; i++) {
    printf("[%d] = %d; ", i, urna[i]);
  }
  printf("\n");
}

int main(int argc, char** argv)
  {
  int my_rank;
  int proc_n;
  int message[MAX_BUFFER];
  int message_send;
  int my_next; // Point to the next process in the ring
  int achei_next;
  int atual_coord;
  int ja_fui_coord = 0; // Para usarmos todos processos
  int done = 0;
  int falhas = 0;
  int aviso_novo_coord = 0;
  int *urna;
  int *procs_alive;
  int i;
  int espera_urna = 0;
  int dead_procs = 0;
  MPI_Status status;

  MPI_Init(&argc , &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

  urna = (int *)malloc(proc_n*sizeof(int));		//Aloca urna para mandar pelo anel
  procs_alive = (int *)malloc(proc_n*sizeof(int));	//Aloca vetor para manter quais processus estao vivos

  for(i=0; i<proc_n; i++) {
	procs_alive[i] = 1;
  }

  atual_coord = proc_n - 1; // Inicia execução com proc_n - 1 de coordenador
  if (my_rank == proc_n - 1)
	ja_fui_coord = 1;

  if ( my_rank == 0 ) {
    srand(time(NULL));
    message[0] = 0;

    //Proc 0 vai esperar algum tempo, e enviar mensagens de falha no anel

		sleep(1);
		MPI_Send(message, 1, MPI_INT, 2, FALHA_TAG, MPI_COMM_WORLD);

		sleep(1);
		MPI_Send(message, 1, MPI_INT, 1, FALHA_TAG, MPI_COMM_WORLD);

		sleep(1);
		MPI_Send(message, 1, MPI_INT, 1, FALHA_TAG, MPI_COMM_WORLD);

    sleep(1);
    MPI_Send(message, 1, MPI_INT, 1, FALHA_TAG, MPI_COMM_WORLD);

    sleep(1);
    MPI_Send(message, 1, MPI_INT, 1, FALHA_TAG, MPI_COMM_WORLD);

	  done = 1;

    // Wait some time, send kill through ring and terminte
    sleep(5);
    MPI_Send(&message, 1, MPI_INT, 1, KILL, MPI_COMM_WORLD);
    printf("[proc 0] Terminating. Good bye.\n");

  }
  else {

    if (my_rank == proc_n - 1) {
      my_next = 1;
    } else {
      my_next = my_rank + 1;
    }

    while(!done) {
      MPI_Recv(message, MAX_BUFFER, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      switch (status.MPI_TAG) {

        case ELEICAO_TAG:
          // Recebi uma mensagem com tag de eleicao
          printf("[proc %d] Recebi mensagem ELEICAO\n", my_rank);

          // Se tem eleicao, atual coordenador falhou. Atualizar my_next

          procs_alive[atual_coord] = 0;
		      // Usa o vetor para escolher o prozimo (1 = alive e 0 = dead)
          achei_next = 0;
          i = my_rank+1;
          while(!achei_next) {
            if(i > 0 && procs_alive[i] == 1) {
              my_next = i;
              achei_next = 1;
            }
            i = (i+1)%proc_n;
          }

          // Duas opcoes aqui:
          // (i) Todos aqueles je foram coordenadores nao se candidatam mais;
          // (ii) Somente o coordenador atual nao se candidata (estaria falho).
          // Opcao ii faz com que a coordenacao fique alternando entre os dois processos
          // de maior rank, ja que a eleicao e com base nesse numero e somente um
          // estara falho.

          // if (my_rank != atual_coord)     // Atual coord nao se candidata (i)
          if (!ja_fui_coord)     // Aqueles que ja foram, nao se candidatam (ii)
            message[my_rank] = 1;

          if (espera_urna) {
            espera_urna = 0;
            message_send = computa_urna(message, proc_n);
            if (message_send == 1) { // Nenhum processo livre
              printf("[proc %d] Sou o ultimo processo disponivel. Enviando KILL no anel\n", my_rank);
              envia_kill(my_rank, proc_n);
            } else {
              printf("[proc %d] Novo coordenador eleito: %d\n", my_rank, message_send);
	      			MPI_Send(&message_send, 1, MPI_INT, my_next, NOVO_COORD, MPI_COMM_WORLD);
              aviso_novo_coord = 1;
            }
          } else {
              MPI_Send(&message, proc_n, MPI_INT, my_next, ELEICAO_TAG, MPI_COMM_WORLD);
          }

          break;

        case NOVO_COORD:
          // Recebi uma mensagem com tag de novo coordenador
          printf("[proc %d] Fui avisado de NOVO COORDENADOR %d\n", my_rank, message[0]);

          // Atualizo coordenacao
          atual_coord = message[0];

          if (my_rank == message[0]) {
            printf("[proc %d] Fui eleito novo coordenador.\n", my_rank);
            ja_fui_coord = 1;
          }

          if (aviso_novo_coord) {
            // Iniciei o aviso de novo coordenar e ja recebi a mensagem
            // novamente. Todos no anel foram avisados
            printf("[proc %d] Todos ja foram avisados do novo coordenador.\n", my_rank);
            aviso_novo_coord = 0;
          } else {
            // Repasso a mensagem de novo coordenador
            MPI_Send(message, 1, MPI_INT, my_next, NOVO_COORD, MPI_COMM_WORLD);
          }
          break;

        case FALHA_TAG:
          // Recebi uma mensagem com tag de falha. Coordenador caiu
          printf("[proc %d] Detectei FALHA no coordenador %d\n", my_rank, atual_coord);

          if (my_rank != atual_coord) {

            // Atualizo my_next, caso ele seja o coordenador que falhou
            achei_next = 0;
            i = my_rank+1;
            while(!achei_next) {
              if(i > 0 && procs_alive[i] == 1) {
                my_next = i;
                achei_next = 1;
              }
              i = (i+1)%proc_n;
            }

            // Convoco eleicao
            init_urna(urna, proc_n);
            espera_urna = 1;
	    			// Envio Urna
            MPI_Send(urna, proc_n, MPI_INT, my_next, ELEICAO_TAG, MPI_COMM_WORLD);

          } else {
            // Por algum motivo proc 0, mando tag falha para o atual coord.
            // Apenas repassa...
            MPI_Send(message, proc_n, MPI_INT, my_next, FALHA_TAG, MPI_COMM_WORLD);
          }
          break;

        case KILL:
          // Execucao acabou, repasso kill no anel e termino
          printf("[proc %d] Mensagem de KILL. Good bye.\n", my_rank);
          envia_kill(my_rank, proc_n);
          done = 1;
          break;

        default:
          printf("[proc %d] Mensagem SEM TAG VALIDA!\n", my_rank);
          done = 1;
          break;
      }
    }
  }
  free(urna);
  MPI_Finalize();
  return(0);
}
