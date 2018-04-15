###########################################
##            Problema Banco             ##
###########################################

Modelo: CLIENTE - SERVIDOR

Instrucoes:
  - Para gerar stubs RPC rodar make rpcgen em rpcgen/
  - Para compilar arquivos make all
  - Para executar um a um. Podes rodar ./servidor e ./cliente em rpcgen/
  - Rodar ./cliente passando:
      1)  local do servidor (ex localhost)
      2)  A para processo agencia ou C para caixa eletronico
      3)  0 para funcionamento normal ou 1 para falhas
  - Ou ainda pode rodar run_server.sh para chamar processo de servidor
    e run_clients.sh para chamar conjunto de processos de agencias e caixas
    de entrada padrao (arquivos input_agencia e input_agencia)
