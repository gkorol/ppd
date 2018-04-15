///////////////////////////////////////////////////
// Programa servidor: Processo Administrador     //
///////////////////////////////////////////////////
#include <rpc/rpc.h>
#include "bank.h"

#define MAX_CONTAS 10

conta contas[MAX_CONTAS] = {0};

int * inicializar_1_svc(struct svc_req *req){
  static int result;
  result = 1;
  return (&result);
}

int * abre_1_svc(struct svc_req *req) {
  /* Abre conta e reotorna id */
  static int result;
  result = 35;
  printf("[SERVER] Abrindo conta no servidor\n");
  return (&result);
}

int * fecha_1_svc(int cc, struct svc_req *req) {
  /* Fecha conta da id passada */
  static int result;
  result = 1;
  printf("[SERVER] Fechando conta %d no servidor\n", cc);
  return (&result);
}

int * autentica_1_svc(int cc, struct svc_req *req) {
  /* Retorna se conta existe */
  static int result;
  result = 1;
  printf("[SERVER] Autenticando conta %d no servidor\n", cc);
  return (&result);
}

float * saldo_1_svc(int cc, struct svc_req *req){
  /* Retorna saldo */
  static float result;
  result = 10.5;
  printf("[SERVER] Consulta saldo de %d no servidor\n", cc);
  return (&result);
}

int * deposita_1_svc(int cc,float valor, struct svc_req *req) {
  /* Depoista valor */
  static int result;
  result = 1;
  printf("[SERVER] Depostiando R$ %f -> cc:%d\n", valor, cc);
  return (&result);
}

int * saca_1_svc(int cc, float valor, struct svc_req *req) {
  /* Saca valor */
  static int result;
  result = 1;
  printf("[SERVER] Sacando R$ %f <- cc:%d\n", valor, cc);
  return (&result);
}

int * assinatura_1_svc(struct svc_req *req) {
  /* Retorna assinatura para operacao Exactly once */
  static int result;
  result = 1;
  printf("[SERVER] Retornando assinatura\n");
  return (&result);
}
