#!/bin/sh
# Start client Process


# Abrem algumas agencias e solicitam algumas operacoes
./rpcgen/cliente A localhost < input_agencia 0
./rpcgen/cliente A localhost < input_agencia 0

# Operacoes de caixa em paralelo
./rpcgen/cliente A localhost < input_caixa 0 &
./rpcgen/cliente A localhost < input_caixa 0 &
./rpcgen/cliente A localhost < input_caixa 0 &

# Caixa com operacoes remotas falhando
./rpcgen/cliente A localhost < input_caixa 1 &
