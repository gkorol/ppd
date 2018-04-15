Modelagem - Problema Banco


Proc ADM
	- Abertura de cc (para Ag)		(E.O.)
	- Fechamento de cc (para Ag)
	- Autenticação (para Ag e Caixas)

Proc Agencia
	- Abertura de cc 			(E.O.)
	- Autenticação
	- Fechamento de cc
	- Retira 				(E.O.)
	- Deposita 				(E.O.)
	- Consulta

Proc Caixa:
	- Retira 				(E.O.)
	- Deposita 				(E.O.)
	- Consulta

(Cliente/Servidor)
ADM -----> Ag_1
    -----> Ag_2
    -----> Ag_3
    -----> Ag_n
    -----> Cx_1
    -----> Cx_2
    -----> Cx_3
    -----> Cx_m

Servidor:
ADM:			Vetor de agencias;
		 	Vetor de caixas;
			Vetor de contas;
			Vetor de historico de operacoes por conta (para saber quais operacoes e de quais processos ja foram realizadas) (pode estar na struct da conta)

			Retorna autenticacao (verifica se conta existe);
			Retorna deposito, saque e consulta;
			Retorna abertura e fechamento;
			Retorna assinatura (fornece assinatura da operacao)
==============================================================================================
Cliente:
Agencias:		Chama abertura e fechamento;

Agencias e Caixas:	Chama saque, deposito, e consulta;
			Chama autenticação;
			Chama assinatura inicial e nome;
==============================================================================================
Assinatura: <contador>

Operacoes E.O.: 1) Autentica
		2) Pede assinatura
		3) Chama deposito, saque, etc...
		4) Espera confirmação (time_out)
		5) 	Se falha, volta ao 3
		6) 	senao, Sucesso
