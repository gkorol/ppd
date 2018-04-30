#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "mpi.h"

#define WORK_TAG    1
#define ELEICAO_TAG 9
#define URNA_TAG    99
#define FALHA_TAG   999
#define NOVO_COORD  9999
#define KILL        666

void falha_coord(int proc_n) {
  int foo = 1;
  int target = rand()%(proc_n-1) + 1;
  MPI_Send(&foo, 1, MPI_INT, target, FALHA_TAG, MPI_COMM_WORLD);
}

void convoca_eleicao(int my_rank, int proc_n) {
  if (my_rank == proc_n-1)
    MPI_Send(&my_rank, 1, MPI_INT, 1, ELEICAO_TAG, MPI_COMM_WORLD);
  else
    MPI_Send(&my_rank, 1, MPI_INT, my_rank+1, ELEICAO_TAG, MPI_COMM_WORLD);
}

void informa_novo_coord(int my_rank, int proc_n, int coord) {
  if (my_rank == proc_n-1)
    MPI_Send(&coord, 1, MPI_INT, 1, NOVO_COORD, MPI_COMM_WORLD);
  else
    MPI_Send(&coord, 1, MPI_INT, my_rank+1, NOVO_COORD, MPI_COMM_WORLD);
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
  fflush(stdout);
}

main(int argc, char** argv)
  {
  int my_rank;
  int proc_n;
  int message;
  int work = 1;
  int atual_coord = 1; // Inicia como coord
  int ja_fui_coord = 0; // Para usarmos todos processos
  int done = 0;
  int falhas = 0;
  int aviso_novo_coord = 0;
  int *urna;
  int espera_urna = 0;
  MPI_Status status;

  MPI_Init(&argc , &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

  urna = (int *)malloc(proc_n*sizeof(int));

  if ( my_rank == 0 ) {
    srand(time(NULL));
    message = 5;
    // informa_novo_coord(my_rank, proc_n, (rand()%(proc_n-1) + 1));
    falha_coord(proc_n);
    while (!done) {
      MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      if (status.MPI_TAG == WORK_TAG) {
        switch (message){
        // Falha coordenadores em especificas msgs
          case 5:
            falha_coord(proc_n);
            break;
          case 20:
            falha_coord(proc_n);
            break;
          case 30:
            falha_coord(proc_n);
            break;
          case 40:
            // Termina
            done = 1;
            break;
        }
      }
    }
    // Send kill through ring and terminte
    MPI_Send(&message, 1, MPI_INT, 1, KILL, MPI_COMM_WORLD);

  }
  else {
    while(!done) {
      MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // recebo da esquerda

      switch (status.MPI_TAG) {

        case ELEICAO_TAG:
          printf("[%d] Mensagem ELEICAO\n", my_rank);
          // Apos ELEICAO_TAG, espera pela urna, se candidata e passa adiante
          MPI_Recv(urna, proc_n, MPI_INT, MPI_ANY_SOURCE, URNA_TAG, MPI_COMM_WORLD, &status);
          printf("[%d] Mensagem URNA\n", my_rank);

          // if (my_rank != atual_coord) // Atual coord nao se candidata
          if (!ja_fui_coord) // Aqueles que ja foram, nao se candidatam
            urna[my_rank] = 1;

          if (espera_urna) {
            espera_urna = 0;
            message = computa_urna(urna, proc_n);
            if (message == 0) { // Nenhum processo livre
              printf("[%d] Nenhum processo disponivel. Enviando KILL no anel\n", my_rank);
              done = 1;
              if (my_rank == proc_n-1)
                MPI_Send(&message, 1, MPI_INT, 1, KILL, MPI_COMM_WORLD);
              else
                MPI_Send(&message, 1, MPI_INT, my_rank+1, KILL, MPI_COMM_WORLD);
            } else {
              printf("[%d] Novo coordenador eleito: %d\n", my_rank, message);
              if (my_rank == proc_n-1)
                MPI_Send(&message, 1, MPI_INT, 1, NOVO_COORD, MPI_COMM_WORLD);
              else
                MPI_Send(&message, 1, MPI_INT, my_rank+1, NOVO_COORD, MPI_COMM_WORLD);

              aviso_novo_coord = 1;
            }
          } else {
            if (my_rank == proc_n-1)
              MPI_Send(&message, 1, MPI_INT, 1, ELEICAO_TAG, MPI_COMM_WORLD);
            else
              MPI_Send(&message, 1, MPI_INT, my_rank+1, ELEICAO_TAG, MPI_COMM_WORLD);

            if (my_rank == proc_n-1)
              MPI_Send(urna, proc_n, MPI_INT, 1, URNA_TAG, MPI_COMM_WORLD);
            else
              MPI_Send(urna, proc_n, MPI_INT, my_rank+1, URNA_TAG, MPI_COMM_WORLD);
          }

          break;

        case NOVO_COORD:
          printf("[%d] Mensagem NOVO_COORD %d\n", my_rank, message);

          // Atualizo coordenacao
          atual_coord = message;

          if (my_rank == message) {
            printf("[%d] Fui eleito novo coordenador.\n", my_rank);
            ja_fui_coord = 1;
            // Inicia novo trabalho
            work = 1;
            if (my_rank == proc_n-1)
              MPI_Send(&work, 1, MPI_INT, 1, WORK_TAG, MPI_COMM_WORLD);
            else
              MPI_Send(&work, 1, MPI_INT, my_rank+1, WORK_TAG, MPI_COMM_WORLD);
          }

          if (aviso_novo_coord) {
            printf("[%d] Todos ja foram avisados do novo coordenador.\n", my_rank);
            aviso_novo_coord = 0;
          } else {
            informa_novo_coord(my_rank, proc_n, message);
          }
          break;

        case FALHA_TAG:
          printf("[%d] Mensagem FALHA_TAG\n", my_rank);
          if (my_rank != atual_coord) {
            // Convoco eleicao
            init_urna(urna, proc_n);
            espera_urna = 1;

            if (my_rank == proc_n-1)
              MPI_Send(&message, 1, MPI_INT, 1, ELEICAO_TAG, MPI_COMM_WORLD);
            else
              MPI_Send(&message, 1, MPI_INT, my_rank+1, ELEICAO_TAG, MPI_COMM_WORLD);

            if (my_rank == proc_n-1)
              MPI_Send(urna, proc_n, MPI_INT, 1, URNA_TAG, MPI_COMM_WORLD);
            else
              MPI_Send(urna, proc_n, MPI_INT, my_rank+1, URNA_TAG, MPI_COMM_WORLD);

          } else {
            // Apenas repasso pq sou o atual coordeandor
            if (my_rank == proc_n-1)
              MPI_Send(&message, 1, MPI_INT, 1, FALHA_TAG, MPI_COMM_WORLD);
            else
              MPI_Send(&message, 1, MPI_INT, my_rank+1, FALHA_TAG, MPI_COMM_WORLD);
          }
          break;

        case WORK_TAG:
          if (atual_coord != my_rank) {
            printf("[%d] Mensagem de TRABALHO %d\n", my_rank, message);
            message = message * my_rank;
          } else {
            printf("[%d] Recebi trabalho de %d\n", my_rank, message);
            work++;
            message = work;
            printf("[%d] Novo TRABALHO %d\n", my_rank, message);
            // Informa rank 0 do novo trabalho
            MPI_Send(&message, 1, MPI_INT, 0, WORK_TAG, MPI_COMM_WORLD);
          }

          if (my_rank == proc_n-1)
            MPI_Send(&message, 1, MPI_INT, 1, WORK_TAG, MPI_COMM_WORLD);
          else
            MPI_Send(&message, 1, MPI_INT, my_rank+1, WORK_TAG, MPI_COMM_WORLD);
          break;

        case KILL:
          printf("[%d] Mensagem de KILL. Good bye\n", my_rank);
          if (my_rank == proc_n-1)
            MPI_Send(&message, 1, MPI_INT, 1, KILL, MPI_COMM_WORLD);
          else
            MPI_Send(&message, 1, MPI_INT, my_rank+1, KILL, MPI_COMM_WORLD);
          done = 1;
          break;
        default:
          printf("[%d] Mensagem SEM TAG!\n", my_rank);
          done = 1;
          break;
      }
    }
  }
  free(urna);
  MPI_Finalize();
}
