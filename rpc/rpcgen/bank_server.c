///////////////////////////////////////////////////
// Programa servidor: Processo Administrador     //
///////////////////////////////////////////////////
#include <rpc/rpc.h>
#include "bank.h"

#define MAX_CONTAS 10

conta contas[MAX_CONTAS];
int total_cc = 0;
int init_done = 0;

int * inicializar_1_svc(struct svc_req *req){
  static int result;
  static int i;
  result = 1;

  if (init_done==1) return(&result);

  printf("[SERVER] Inicializando contas\n");

  for (i=0; i<MAX_CONTAS; i++) {
    contas[i].id = 0;
    contas[i].saldo = 0;
  }

  init_done = 1;
  return (&result);
}

int * abre_1_svc(struct svc_req *req) {
  /* Abre conta e reotorna id */
  static int result;
  if (total_cc == MAX_CONTAS) {
      printf("[SERVER] Nova conta nao permitida. Total excedido.\n");
      result = 0;
      return (&result);
  } else {
      contas[total_cc].id = total_cc + 1;
      total_cc++;
      printf("[SERVER] Abrindo conta nro %d\n",total_cc);
      result = total_cc;
      return (&result);
  }
}

int * fecha_1_svc(int cc, struct svc_req *req) {
  /* Fecha conta da id passada */
  static int result;
  static int i;

  for (i=0;i<MAX_CONTAS;i++) {
    if (contas[i].id == cc) {
      printf("[SERVER] Fechando conta %d no servidor\n", cc);
      contas[i].id = 0;
      contas[i].saldo = 0;
      result = 1;
      return (&result);
    }
  }
  printf("[SERVER] Conta %d nao existe\n", cc);
  result = 0;
  return (&result);
}

int * autentica_1_svc(int cc, struct svc_req *req) {
  /* Retorna se conta existe */
  static int result;
  static int i;

  for (i=0;i<MAX_CONTAS;i++) {
    if (contas[i].id == cc) {
      printf("[SERVER] Autenticando conta %d no servidor\n", cc);
      result = 1;
      return (&result);
    }
  }
  printf("[SERVER] Conta %d nao existe\n", cc);
  result = 0;
  return (&result);
}

float * saldo_1_svc(int cc, struct svc_req *req){
  /* Retorna saldo */
  static float result;
  static int i;

  for (i=0;i<MAX_CONTAS;i++) {
    if (contas[i].id == cc) {
      printf("[SERVER] Consulta saldo em %d no servidor\n", cc);
      result = contas[i].saldo;
      return (&result);
    }
  }
  printf("[SERVER] Conta %d nao existe\n", cc);
  result = 0;
  return (&result);
}

int * deposita_1_svc(int cc, float valor, struct svc_req *req) {
  /* Depoista valor */
  static int result;
  static int i;

  for (i=0;i<MAX_CONTAS;i++) {
    if (contas[i].id == cc) {
      printf("[SERVER] Depostiando R$ %f -> cc:%d\n", valor, cc);
      contas[i].saldo = contas[i].saldo + valor;
      result = 1;
      return (&result);
    }
  }
  printf("[SERVER] Conta %d nao existe\n", cc);
  result = 0;
  return (&result);
}

int * saca_1_svc(int cc, int valor, struct svc_req *req) {
  /* Saca valor */
  static int result;
  static int i;

  for (i=0;i<MAX_CONTAS;i++) {
    if (contas[i].id == cc) {
      printf("[SERVER] Sacando R$ %d <- cc:%d\n", valor, cc);
      contas[i].saldo = contas[i].saldo - valor;
      result = 1;
      return (&result);
    }
  }
  printf("[SERVER] Conta %d nao existe\n", cc);
  result = 0;
  return (&result);
}

int * assinatura_1_svc(struct svc_req *req) {
  /* Retorna assinatura para operacao Exactly once */
  static int result;
  result = 1;
  printf("[SERVER] Retornando assinatura\n");
  return (&result);
}
