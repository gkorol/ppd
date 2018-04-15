/* bank.x
  Especificacao inspirada no material de RPC em
  https://docs.oracle.com/cd/E19683-01/816-1435/rpcgenpguide-21470/index.html
*/
struct conta {
  int id;
  float saldo;
};

program BANKPROG{
  /* nome do programa remoto */
  version BANKVERS{
      /* declaração da versão */
      int INICIALIZAR(void) = 1;    /* Inicializa... */
      int ABRE(void) = 2;           /* Abre conta e reotorna id */
      int FECHA(int) = 3;           /* Fecha conta da id passada */
      int AUTENTICA(int) = 4;       /* Retorna se conta existe */
      float SALDO(int) = 5;         /* Retorna saldo */
      int DEPOSITA(int,float) = 6;  /* Depoista valor */
      int SACA(int, float) = 7;     /* Saca valor */
      int ASSINATURA(void) = 8;     /* Retorna assinatura para operacao Exactly once */
    } = 1;
/* definição da versão do programa */
} = 0x30090949;
/* número do programa remoto (deve ser único) */
