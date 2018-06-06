#!/bin/bash

sacos=("100" "500" "1000" "5000" "10000")
slaves=("2" "4" "8" "16" "32")

for s in ${slaves[@]}
do
	for i in ${sacos[@]}
	do
		echo "Executando p = "$s" com tamanho [1000 x "$i"]"
		ladrun -np $s mestre_escravo 1000 $i > "results/output_p"$s"_l1000_c"$i"_bubble.txt"
	done
done
