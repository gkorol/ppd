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
  int* ass;

  if( argc != 3 ) {
    printf("Usage: $ %s <A(gencia)||C(aixa)> <server>\n", argv[0]);
    exit(1);
  }
  modo = argv[1][0];
  server = argv[2];

  handle = clnt_create(server, BANKPROG, BANKVERS, "tcp");
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
    printf("\t9 - Sair\n");
    scanf("%d", &op);
    switch (op) {
      case 1:
        printf("\nAbertura de Conta. Aguarde servidor...\n");
        if (modo == 'A') {
          ass = assinatura_1(handle);
          result = abre_1(*ass, handle);
        	if (result == (int *)NULL) {
        		printf("Erro ao conectar com servidor!\n");
        		exit(1);
        	}
          // if (*result != 0) printf("Conta aberta. Numero %d (ass: %d)\n", *result, *ass);
          // else printf("Operacao falhou!\n");
          while (*result == 0) {
            printf("Operacao falhou! Tentando NOVAMENTE...\n");
            result = abre_1(*ass, handle);
          }

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
          // if (*result != 0) printf("Conta %d fechada.\n", cc);
          // else printf("Operacao falhou!\n");
          while (*result == 0) {
            printf("Operacao falhou! Tentando NOVAMENTE...\n");
            result = fecha_1(cc, handle);
          }

        } else {
          printf("Dirija-se a uma agencia %c\n", modo);
        }
        break;
      case 3:
        printf("\nSaque. Entre com o numero da conta:\n");
        scanf("%d", &cc);
        printf("\nSaque. Entre com o valor:\n");
        scanf("%d", &saq);
        ass = assinatura_1(handle);
        result = saca_1(cc, saq, *ass, handle);
        if (result == (int *)NULL) {
          printf("Erro ao conectar com servidor!\n");
          exit(1);
        }
        // if (*result != 0) printf("Saque de R$%d efetuado em cc %d. (ass: %d)\n", saq, cc, *ass);
        // else printf("Operacao falhou!\n");
        while (*result == 0) {
          printf("Operacao falhou! Tentando NOVAMENTE...\n");
          result = saca_1(cc, saq, *ass, handle);
        }

        break;
      case 4:
        printf("\nDeposito. Entre com o numero da conta:\n");
        scanf("%d", &cc);
        printf("\nDeposito. Entre com o valor:\n");
        scanf("%f", &dep);
        ass = assinatura_1(handle);
        result = deposita_1(cc, dep, *ass, handle);
        if (result == (int *)NULL) {
          printf("Erro ao conectar com servidor!\n");
          exit(1);
        }
        // if (*result != 0) printf("Deposito de R$%2.f efetuado em cc %d. (ass: %d)\n", dep, cc, *ass);
        // else printf("Operacao falhou!\n");
        while (*result == 0) {
          printf("Operacao falhou! Tentando NOVAMENTE...\n");
          result = deposita_1(cc, dep, *ass, handle);
        }
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
      case 9:
        printf("Saindo...\n");
        clnt_destroy(handle);
        return 0;
        break;
      default:
        printf("Operacao ILEGAL\n");
        break;
    }

  }
}
