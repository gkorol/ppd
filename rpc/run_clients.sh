#!/bin/sh
# Start client Process


# Abrem algumas agencias e solicitam algumas operacoes
./rpcgen/cliente A localhost 0 < input_agencia
./rpcgen/cliente A localhost 0 < input_agencia

# Operacoes de caixa em paralelo
./rpcgen/cliente A localhost 0 < input_caixa &
./rpcgen/cliente A localhost 0 < input_caixa &
./rpcgen/cliente A localhost 0 < input_caixa &

# Caixa com operacoes remotas falhando
./rpcgen/cliente A localhost 1 < input_caixa &
