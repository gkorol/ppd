#!/bin/sh
# Start client Process


# Abrem algumas agencias e solicitam algumas operacoes
./rpcgen/cliente A localhost < input_agencia
./rpcgen/cliente A localhost < input_agencia

# Operacoes de caixa em paralelo
./rpcgen/cliente A localhost < input_caixa &
./rpcgen/cliente A localhost < input_caixa &
./rpcgen/cliente A localhost < input_caixa &
