// Note: Medida de tempo baseada no https://www.geeksforgeeks.org/how-to-measure-time-taken-by-a-program-in-c/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#define DEBUG            // comentar esta linha quando for medir tempo

#define BS             // comentar esta linha para usar quick sort, do contrário, usa bubble sort

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

int main(int argc, char** argv)
{
    int TAM_TOTAL;
	  int TAM_TRAB;
    int * saco;				// saco de trabalho
    int i, j;
    clock_t t;

    //TAM_TOTAL = atoi(argv[1]);
  	TAM_TRAB  = atoi(argv[1]);

    saco = malloc( TAM_TRAB * sizeof(int) );

    t = clock();
    
   // for( i = 0; i < TAM_TOTAL; ++i ) {
			for( j = 0; j < TAM_TRAB; ++j ) {
				saco[j] = (j-TAM_TRAB)*(-1);
			}
//		}

    #ifdef DEBUG
    printf("\nMatriz de trabalhos original:\n");
//		for( i = 0; i < TAM_TOTAL; ++i ) {
//			printf("linha %2d: [ ", i);
			for( j = 0; j < TAM_TRAB; ++j ) {
				printf("%2d ",saco[j]);
			}
//			printf(" ]\n");
//		}
    #endif

    #ifdef BS
		  printf("\nSequencial - Vetores ordenados com Bubble Sort\n");
      
      bs(TAM_TRAB,saco);

      t = clock() - t;
		#else
		  printf("\nSequencial - Vetores ordenados com Quick Sort\n");
      
      qsort(saco, TAM_TRAB, sizeof(int), cmpfunc);
      t = clock() - t;
		#endif
    printf("Tempo decorrido = %2.8f s\n", ((double)t)/CLOCKS_PER_SEC);
		fflush(stdout);

//    #ifdef DEBUG
//    printf("\nTrabalhos ordenados:\n");
//		for( i = 0; i < TAM_TOTAL; ++i ) {
//			printf("linha %2d: [ ", i);
//			for( j = 0; j < TAM_TRAB; ++j ) {
//				printf("%2d ",saco[i*TAM_TRAB+j]);
//			}
//			printf(" ]\n");
//		}
//    #endif

    return 0;
}
