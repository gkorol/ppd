/*
*
*	TODO[]: Otimizacoes:
*					A - Codigo normal
*					B - Aumenta nro de processos, pq alguns desses procs nao trabalham (procs nao folhas)
*					C - Realiza parte do trabalho (<Delta) localmente, e divide o que sobrou. (Limita nro de procs ao nro de cores)
*
*	Obs: Speed-ups super lineares -> dividir trabalho ao meio, melhora 4x tempo do BS (n²)
* Obs2:
*	--> Left child: (2*my_rank)+1
*	--> Right child: (2*my_rank)+2
* --> Father: (my_rank-1)/2  (integer division)
* source: http://www.cs.fsu.edu/~lacher/courses/COP4531/lectures/binary_heaps/slide11.html#slide23link1
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

int *interleaving(int vetor[], int tam)
{
	int *vetor_auxiliar;
	int i1, i2, i_aux;

	vetor_auxiliar = (int *)malloc(sizeof(int) * tam);

	i1 = 0;
	i2 = tam / 2;

	for (i_aux = 0; i_aux < tam; i_aux++) {
		if (((vetor[i1] <= vetor[i2]) && (i1 < (tam / 2)))
		    || (i2 == tam))
			vetor_auxiliar[i_aux] = vetor[i1++];
		else
			vetor_auxiliar[i_aux] = vetor[i2++];
	}

	return vetor_auxiliar;
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

// void mostra(int vet[], int tam)
// {
// 	int i;
// 	for (i = 0; i < tam; i++) {
// 		printf("%d ", vet(i));
// 	}
// 	printf("\n");
// }

int main(int argc, char** argv) {

	int i;
	int * saco;
	int * ret;
	int delta;
	int tam;

	int filho_esq;
	int filho_dir;
	int pai;

	int my_rank;
  int proc_n;
  MPI_Status status;

  MPI_Init(&argc , &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

  if ( my_rank != 0 ) {
		pai = (my_rank-1)/2;
		MPI_Recv(&ret, tam, MPI_INT, pai, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &tam);  // descubro tamanho da mensagem recebida

  } else {
		tam 	= atoi(argv[1]);           // defino tamanho inicial do vetor
		delta = atoi(argv[2]);					 // defino o delta

		saco = malloc(tam * sizeof(int));
		ret  = malloc(tam * sizeof(int));

		for( i = 0; i < tam; ++i ) {
			saco[i] = (i-tam)*(-1);
		}
	}

	if ( tam <= delta )
		bs(tam, saco);  // conquisto
	else {
		// dividir
    // quebrar em duas partes e mandar para os filhos
		filho_esq = (my_rank*2)+1;
		filho_dir = (my_rank*2)+2;
		// Envia para os dois filhos
		// MPI_Send ( &vetor[0], filho esquerda, tam_vetor/2 );  // mando metade inicial do vetor
		// MPI_Send ( &vetor[tam_vetor/2], filho direita , tam_vetor/2 );  // mando metade final

		MPI_Send(&saco[0],      tam/2, MPI_INT, filho_esq, 1, MPI_COMM_WORLD);
		MPI_Send(&saco[tam/2], tam/2, MPI_INT, filho_dir, 1, MPI_COMM_WORLD);

		// Recebe dos dois filhos
		// MPI_Recv ( &vetor[0], filho esquerda);
		// MPI_Recv ( &vetor[tam_vetor/2], filho direita);
		// MPI_Recv(saco, TAM_TRAB, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		MPI_Recv(ret,       tam/2, MPI_INT, filho_esq, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		MPI_Recv(ret+tam/2, tam/2, MPI_INT, filho_dir, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		// Intercala
		saco = interleaving(ret, tam);
	}

	// Manda para o pai

	if( my_rank !=0 )
		pai = (my_rank-1)/2;
		MPI_Send(saco, tam, MPI_INT, pai, 1, MPI_COMM_WORLD);
	//else
		//Mostra(saco,tam);

  MPI_Finalize();
	return 0;
}
