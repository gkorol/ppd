#!/bin/bash

procs=("1" "3" "7" "15" "31")
sacos=("100000" "100000000")
# deltas=("1000" "5000")

for s in ${procs[@]}
do
	for i in ${sacos[@]}
	do
		echo "Executando p = "$s" com tamanho "$i" e delta 1000"
		ladrun -np $s mestre_escravo $i 1000 > "results/output_p"$s"_s"$i"_d1000_quick.txt"
	done
done
