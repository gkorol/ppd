///////////////////////////////////////////////////
// Programa cliente: Agencia ou Caixa Automatico //
///////////////////////////////////////////////////
#include <rpc/rpc.h>
#include "bank.h"

int main(int argc, char const *argv[]) {
  CLIENT *handle;
  char *server;
  int* result;
  char modo;
  int op = 0;
  int cc = 0;
  int saq = 0;
  float dep = 0.0;
  float* sal;

  if( argc != 3 ) {
    printf("Usage: $ %s <A(gencia)||C(aixa)> <server>\n", argv[0]);
    exit(1);
  }
  modo = argv[1][0];
  server = argv[2];

  handle = clnt_create(server, BANKPROG, BANKVERS, "udp");
  if (handle == 0) {
    printf("Nao pode conectar programa remoto/n");
    exit(1);
  }
  inicializar_1(handle);
  while(1) {
    printf("\n\nEscolha a operacao:\n");
    printf("\t1 - Abertura de Conta   (somente agencia)\n");
    printf("\t2 - Fechamento de Conta (somente agencia)\n");
    printf("\t3 - Saque\n");
    printf("\t4 - Deposito\n");
    printf("\t5 - Saldo\n");
    scanf("%d", &op);
    switch (op) {
      case 1:
        printf("\nAbertura de Conta. Aguarde servidor...\n");
        if (modo == 'A') {
          result = abre_1(handle);
        	if (result == (int *)NULL) {
        		printf("Erro ao conectar com servidor!\n");
        		exit(1);
        	}
          if (*result != 0) printf("Conta aberta. Numero %d\n", *result);
          else printf("Operacao falhou!\n");

        } else {
          printf("Dirija-se a uma agencia %c\n", modo);
        }
        break;
      case 2:
        printf("\nFechamento de Conta. Entre com o numero da conta:\n");
        scanf("%d", &cc);
        if (modo == 'A') {
          result = fecha_1(cc, handle);
        	if (result == (int *)NULL) {
        		printf("Erro ao conectar com servidor!\n");
        		exit(1);
        	}
          if (*result != 0) printf("Conta %d fechada.\n", cc);
          else printf("Operacao falhou!\n");

        } else {
          printf("Dirija-se a uma agencia %c\n", modo);
        }
        break;
      case 3:
        printf("\nSaque. Entre com o numero da conta:\n");
        scanf("%d", &cc);
        printf("\nSaque. Entre com o valor:\n");
        scanf("%d", &saq);
        result = saca_1(cc, saq, handle);
        if (result == (int *)NULL) {
          printf("Erro ao conectar com servidor!\n");
          exit(1);
        }
        if (*result != 0) printf("Saque de R$%d efetuado em cc %d.\n", saq, cc);
        else printf("Operacao falhou!\n");

        break;
      case 4:
        printf("\nDeposito. Entre com o numero da conta:\n");
        scanf("%d", &cc);
        printf("\nDeposito. Entre com o valor:\n");
        scanf("%f", &dep);
        result = deposita_1(cc, dep, handle);
        if (result == (int *)NULL) {
          printf("Erro ao conectar com servidor!\n");
          exit(1);
        }
        if (*result != 0) printf("Deposito de R$%2.f efetuado em cc %d.\n", dep, cc);
        else printf("Operacao falhou!\n");

        break;
      case 5:
        printf("\nSaldo. Entre com o numero da conta:\n");
        scanf("%d", &cc);
        sal = saldo_1(cc, handle);
        if (sal == (float *)NULL) {
          printf("Erro ao conectar com servidor!\n");
          exit(1);
        } else {
          printf("Saldo de R$%.2f em cc %d.\n", *sal, cc);
        }        
        break;
      default:
        printf("Operacao ILEGAL\n");
        break;
    }

  }

  clnt_destroy(handle);
  return 0;
}
