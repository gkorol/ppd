#!/bin/bash

#procs=("1" "3" "7" "15" "31")
procs=("16" "32")
sacos1=("75000" "90000" "100000")
sacos2=("100000" "25000000" "75000000" "100000000")
# deltas=("1000" "5000")

for s in ${procs[@]}
do
	for i in ${sacos1[@]}
	do
		echo "Executando p = "$s" com tamanho "$i" e delta 7500"
		ladrun -np $s dc_a $i 7500 > "results/output_p"$s"_s"$i"_d7500_quick.txt"
	done
done

for s in ${procs[@]}
do
        for i in ${sacos2[@]}
        do
                echo "Executando p = "$s" com tamanho "$i" e delta 7500000"
                ladrun -np $s dc_a $i 7500000 > "results/output_p"$s"_s"$i"_d7500000_quick.txt"
        done
done
