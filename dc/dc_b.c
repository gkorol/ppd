/*
*
*	TODO[]: Otimizacoes:
*		[OK]	A - Codigo normal
*					B - Realiza parte do trabalho (<Delta) localmente, e divide o que sobrou. (Limita nro de procs ao nro de cores)
*					C - Aumenta nro de processos, pq alguns desses procs nao trabalham (procs nao folhas)
*
*	Obs: Speed-ups super lineares -> dividir trabalho ao meio, melhora 4x tempo do BS (n²)
*
*	Obs2:
*	--> Left child: (2*my_rank)+1
*	--> Right child: (2*my_rank)+2
* --> Father: (my_rank-1)/2  (integer division)
* 		source: http://www.cs.fsu.edu/~lacher/courses/COP4531/lectures/binary_heaps/slide11.html#slide23link1
* Obs3:
* DELTA: Testei em casa apenas com (np = 4), portanto, delta não podia ser > tam/2 senão dava erro de invalid rank
* (tentava mandar mensagem para ranks inexistentes). tam = 100 | proc_n = 4 | delta = 100/4 = 25 | necessito de 7 processos para executar.
* Talvez isso não aconteça no LAD, pois estou rodando em uma VM.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

// #define PRINT
// #define BS

#define KILL 666

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a > *(int*)b );
}

void merge(int*output, int* vector1, int size1, int* vector2, int size2, int* vector3, int size3)
{
int i, j, k, m;
i = j = k = m = 0;

while(i <size1+size2+size3)
    if (j < size1 )
        if(k < size2)
            if(m<size3)
                if(vector1[j] < vector2[k])
                    if(vector1[j] < vector3[m])
                        output[i++] = vector1[j++];
                    else
                        if(vector3[m] < vector2[k])
                            output[i++] = vector3[m++];
                        else
                            output[i++] = vector2[k++];
                else
                    if(vector2[k] < vector3[m])
                        output[i++] = vector2[k++];
                    else
                        output[i++] = vector3[m++];
            else
                if(vector1[j] < vector2[k])
                    output[i++] = vector1[j++];
                else
                    output[i++] = vector2[k++];
        else
            if(m<size3)
                if(vector1[j] < vector3[m])
                    output[i++] = vector1[j++];
                else
                    output[i++] = vector3[m++];
            else
                output[i++] = vector1[j++];
    else
        if(k<size2)
            if(m<size3)
                if(vector2[k] < vector3[m])
                    output[i++] = vector2[k++];
                else
                    output[i++] = vector3[m++];
            else
                output[i++] = vector2[k++];
        else
            output[i++] = vector3[m++];
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

void sortVector(int tam, int *saco){
  #ifdef BS
    bs(tam, saco);
  #else
    qsort(saco, tam, sizeof(int), cmpfunc);
  #endif
}

main(int argc, char** argv) {

	int i;
	int * saco;
  int * local;
	int delta;
	int tam;

	int filho_esq;
	int filho_dir;
	int pai;

	int my_rank;
  int proc_n;
  MPI_Status status;
  double t1,t2;

	tam 	= atoi(argv[1]); // Tamanho do saco
  delta	= atoi(argv[2]); // delta

	// Alocacao do vetor principal e do vetor de retorno das recursoes
	saco = malloc(tam * sizeof(int));
  local = malloc(delta * sizeof(int));

  MPI_Init(&argc , &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

	// Definicao do delta - assumindo proc_n multiplo de 2
	// delta = (tam*2)/proc_n;

//  printf("Proc: %2d, pai: %2d, filho_esq: %2d, filho_dir: %2d\n",
//    my_rank, (my_rank-1)/2, (my_rank*2)+1, (my_rank*2)+2);

/*************************************
-- Inicialização ou recebimento do pai
*************************************/

  if ( my_rank != 0 ) {
		// Nodos da arvore: recebem mensagem do nodo pai e verificam o tamanho real recebido
		pai = (my_rank-1)/2;

    #ifdef PRINT
			printf("Pai: %d | Filho: %d | tam: %d", pai, my_rank, tam);
		#endif

		MPI_Recv(saco, tam, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    if(status.MPI_TAG == KILL) {
      printf("Lazy bye from %d \n", my_rank);
      MPI_Finalize();
      exit(0);
    }

    MPI_Get_count(&status, MPI_INT, &tam);

		#ifdef PRINT
			printf(" | Tamanho real recebido: %d\n", tam);
		#endif

  } else {
    printf("Delta: %d\n", delta);
		// Raiz da arvore: inicializa o vetor principal com o pior caso para ordenacao
		for( i = 0; i < tam; ++i ) {
			saco[i] = (i-tam)*(-1);
		}

    // Inicia contagem de tempo
		t1 = MPI_Wtime();

	}

/*************************************
-- Processamento: divisão ou conquista
*************************************/

	if ( tam <= delta )
	{
		// Conquista: realiza a ordenacao do vetor recebido
    sortVector(tam, saco);

	}	else {
    // Separa um delta para processar localmente
    for(i=0;i<delta;i++){
      local[i] = saco[i];
    }

		// Divisão: Quebrar o vetor em duas partes e mandar para os filhos
		filho_esq = (my_rank*2)+1;
		filho_dir = (my_rank*2)+2;

    if(filho_dir > proc_n || filho_esq > proc_n) {
      printf("Sem mais processos livres para arvore. Terminando!\n");
      fflush(stdout);
      killAll(my_rank,proc_n);
      MPI_Finalize();
      exit(0);
    } else {

  		// Envia uma metade para cada filho
  		MPI_Send(saco[delta],       (tam-delta)/2, MPI_INT, filho_esq, 1, MPI_COMM_WORLD);
  		MPI_Send(saco[delta]+tam/2, (tam-delta)/2, MPI_INT, filho_dir, 1, MPI_COMM_WORLD);

      // Processsamento local enquanto não recebe resultado dos filhos
      sortVector(delta, local);

  		// Aguarda os filhos completarem suas tarefas e recebe o resultado
  		MPI_Recv(saco[delta],       (tam-delta)/2, MPI_INT, filho_esq, 1, MPI_COMM_WORLD, &status);
  		MPI_Recv(saco[delta]+tam/2, (tam-delta)/2, MPI_INT, filho_dir, 1, MPI_COMM_WORLD, &status);

  		// Intercala os vetores recebidos dos filhos
      //void merge(int*output, int* vector1, int size1, int* vector2, int size2, int* vector3, int size3)
  		//saco = interleaving(saco, tam);
      merge(saco, saco[delta], (tam-delta)/2, saco[delta]+tam/2, (tam-delta)/2, local, delta);

    }

	}

/*************************************/
/* Retorno ao pai ou finalizacao
/*************************************/

	if( my_rank !=0 ) {
		// Envia vetor de retorno para o pai
		pai = (my_rank-1)/2;
		MPI_Send(saco, tam, MPI_INT, pai, 1, MPI_COMM_WORLD);
	}	else{
    // Finaliza contagem de tempo
		t2 = MPI_Wtime();
    #ifdef BS
		printf("Mestre[%d] Vetor ordenado com Bubble Sort\n", my_rank);
		#else
		printf("Mestre[%d] Vetor ordenado com Quick Sort\n", my_rank);
		#endif
		printf("Tempo decorrido = %f s\n", t2-t1);
		// Acabou a ordenacao do vetor principal, exibe resultado
//		printf("Vetor ordenado: ");
//		printVector(saco,tam);
    killAll(my_rank,proc_n);
    MPI_Finalize();
    exit(0);
	}

  // MPI_Finalize();
  MPI_Recv(saco, tam, MPI_INT, MPI_ANY_SOURCE, KILL, MPI_COMM_WORLD, &status);
  printf("Bye from %d\n", my_rank);
  MPI_Finalize();
  exit(0);
}
