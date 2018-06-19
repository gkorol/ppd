#!/bin/bash

procs=("63" "127" "255" "511")
#procs=("16" "32")
#sacos1=("75000" "90000" "100000")
#sacos2=("100000" "25000000" "75000000" "100000000")
sacos1=("100000000")
deltas=("7500000" "25000000" "20000000" "15000000")

for s in ${procs[@]}
do
	for i in ${deltas[@]}
	do
		echo "Executando p = "$s" com tamanho 1000000000 e delta "$i
		{ time ladrun -np $s dc_a 1000000000 $i ; } 2> "results/output_p"$s"_s1000000000_d"$i"_bubble.txt"
	done
done

#for s in ${procs[@]}
#do
#        for i in ${sacos2[@]}
#        do
#                echo "Executando p = "$s" com tamanho "$i" e delta 7500000"
#                ladrun -np $s dc_a $i 7500000 > "results/output_p"$s"_s"$i"_d7500000_bubble.txt"
#        done
#done
