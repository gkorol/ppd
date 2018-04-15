///////////////////////////////////////////////////
// Programa cliente: Agencia ou Caixa Automatico //
///////////////////////////////////////////////////
#include <rpc/rpc.h>
#include "bank.h"

int main(int argc, char const *argv[]) {
  CLIENT *handle;
  char modo;
  char *server;
  int op = 0;
  int* result;

  if( argc < 3 ) {
    printf("Usage: $ ./client <1-agencia||2-caixa> <server>\n");
    exit(1);
  }
  modo = (char)argv[1];
  server = argv[2];

  /* Seta a conexão para uma chamada remota de procedimento */
  handle = clnt_create(server, BANKPROG, BANKVERS, "udp");
  if (handle == 0) {
    printf("Nao pode conectar programa remoto/n");
    exit(1);
  }
  while(1) {
    printf("\n\nEscolha a operacao:\n");
    printf("1 - Abertura de Conta   (somente agencia)\n");
    printf("2 - Fechamento de Conta (somente agencia)\n");
    printf("3 - Saque\n");
    printf("4 - Deposito\n");
    printf("5 - Saldo\n");
    scanf("%d", &op);
    switch (op) {
      case 1:
        printf("\nAbertura de Conta. Aguarde servidor...\n");
        result = abre_1_svc(handle);
      	if (result == (int *)NULL) {
      		printf("Erro ao conectar ao servidor!\n");
      		exit(1);
      	}
        if (*result != 0) {
          printf("Conta aberta. Numero %d\n", *result);
	      }
        break;
      case 2:
        printf("\nFechamento de Conta. Aguarde servidor...\n");
        break;
      case 3:
        printf("\nSaque. Aguarde servidor...\n");
        break;
      case 4:
        printf("\nDeposito. Aguarde servidor...\n");
        break;
      case 5:
        printf("\nSaldo. Aguarde servidor...\n");
        break;
      default:
        printf("Operacao ILEGAL\n");
        break;
    }

  }

  clnt_destroy(handle);
  return 0;
}
