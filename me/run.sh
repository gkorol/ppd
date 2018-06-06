#!/bin/bash

sacos=("100" "500" "1000" "5000" "10000")
slaves=("2" "4" "8" "16" "32")

for s in ${slaves[@]}
do
	for i in ${sacos[@]}
	do
		for j in ${sacos[@]}
		do
			"ladrun -np "$s" mestre_escravo "$i" "$j" > results/output_p"$s"_l"$i"_c"$j"_bubble.txt"
		done
	done
done
