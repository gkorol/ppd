##################################################################
# Fontes gerados a partir da descricao de protocolo em bank.x 	#
# 										$ rpcgen -N bank.x 												#
##################################################################

# Se ao rodar servidor:
# Cannot register service: RPC: Unable to receive; errno = Connection refused
# Lembre de iniciar rpcbind (sudo /etc/init.d/rpcbind start)

all:
	gcc -c bank_xdr.c
	gcc -I. bank_client.c bank_server.c bank_clnt.c bank_xdr.o -o cliente
	gcc -I. bank_server.c bank_svc.c bank_xdr.o -o servidor

clean:
	rm -f cliente
	rm -f servidor
	rm -f bank_xdr.o

run_cliente:
	cliente 1 localhost

run_servidor:
	servidor
