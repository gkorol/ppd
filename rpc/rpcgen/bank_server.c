///////////////////////////////////////////////////
// Programa servidor: Processo Administrador     //
///////////////////////////////////////////////////
#include <rpc/rpc.h>
#include "bank.h"

// Define numero maximo de contas abertas
#define MAX_CONTAS 100

// Define tamanho do historico de operacoes
#define MAX_HISTORICO 50

conta contas[MAX_CONTAS];
int total_cc = 0;
int init_done = 0;
int ops[MAX_HISTORICO];
int last_op = 0;

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
  for (i=0;i<MAX_HISTORICO;i++) {
    // Nenhuma op realizada
    ops[i] = -1;
  }

  init_done = 1;
  return (&result);
}

int * abre_1_svc(int ass, struct svc_req *req) {
  /* Abre conta e reotorna id */
  static int result;
  if (ops[ass] == 1) {
    // Ja realizada
    printf("[SERVER] Operacao %d ja realizada!\n",ass);
    result = 1;
    return(&result);
  }
  ops[ass] = 1; // Realizando

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
      printf("[SERVER] Consulta de saldo cc %d\n", cc);
      result = contas[i].saldo;
      return (&result);
    }
  }
  printf("[SERVER] Conta %d nao existe\n", cc);
  result = 0;
  return (&result);
}

int * deposita_1_svc(int cc, float valor, int ass, struct svc_req *req) {
  /* Depoista valor */
  static int result;
  static int i;

  if (ops[ass] == 1) {
    printf("[SERVER] Operacao %d ja realizada!\n",ass);
    result = 1;
    return(&result);
  }
  ops[ass] = 1;

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

int * saca_1_svc(int cc, int valor, int ass, struct svc_req *req) {
  /* Saca valor */
  static int result;
  static int i;

  if (ops[ass] == 1) {
    printf("[SERVER] Operacao %d ja realizada!\n",ass);
    result = 1;
    return(&result);
  }
  ops[ass] = 1;

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
  /* Retorna assinatura para operacao de nao-idempotencia */
  /*  last_op aponta para ultima operacao realizada.
      1) Cliente pede assinatura;
      2) Servidor devolve last_op para Cliente. Incrementa last_op
      3) Cliente solicita operacao de assinatura x
      4) Se ops[x] == -1 entao nao foi realizada. Realizar op e ops[x] = 1
      5) Proximas solicitacoes nao serao efetuadas
  */
  static int result;
  result = last_op;
  if (last_op >= MAX_HISTORICO) {
    // Poderia dar a volta no vetor (fifo). Mas vamos simplificar...
    printf("[SERVER] Numero maximo de operacoes!!!\n");
    result = -1;
    return(&result);
  }
  last_op++;
  printf("[SERVER] Assinatura atual -> %d\n", result);
  return (&result);
}
