#!/bin/bash

#procs_c=("63" "127" "255" "511")
procs_c=("127" "255" "511")
procs_a=("15" "31")
#deltas_qs=("750000" "250000" "200000" "150000")
deltas_qs=("75000" "65000" "55000" "30000")
#deltas_bs=("75000" "25000" "20000" "15000")
deltas_bs=("4500" "3000" "2500" "1000")

# roda versao A para quick...
#for s in ${procs_a[@]}
#do
#	for i in ${deltas_qs[@]}
#	do
#		#echo "Executando p = "$s" com tamanho 1000000 e delta "$i
#		(time ladrun -np $s dc_a_qs 1000000 $i) > "results/output_a_p"$s"_s1000000_d"$i"_quick.txt" 2>&1
#		#echo "(time ladrun -np "$s" dc_a_qs 1000000 "$i") > results/output_a_p"$s"_s1000000_d"$i"_quick.txt 2>&1"
#	done
#done

# roda versao A para bubble...
#for s in ${procs_a[@]}
#do
#	for i in ${deltas_bs[@]}
#	do
#		#echo "Executando p = "$s" com tamanho 100000 e delta "$i
#		(time ladrun -np $s dc_a_bs 100000 $i) > "results/output_a_p"$s"_s100000_d"$i"_bubble.txt" 2>&1
#		#echo "(time ladrun -np "$s" dc_a_bs 100000 "$i") > results/output_a_p"$s"_s100000_d"$i"_bubble.txt 2>&1"
#	done
#done

# roda versao B para quick...
#for s in ${procs_a[@]}
#do
#	for i in ${deltas_qs[@]}
#	do
#		#echo "Executando p = "$s" com tamanho 1000000 e delta "$i
#		(time ladrun -np $s dc_b_qs 1000000 $i) > "results/output_b_p"$s"_s1000000_d"$i"_quick.txt" 2>&1
#		#echo "(time ladrun -np "$s" dc_b_qs 1000000 "$i") > results/output_b_p"$s"_s1000000_d"$i"_quick.txt 2>&1"
#	done
#done

# roda versao B para bubble...
#for s in ${procs_a[@]}
#do
#	for i in ${deltas_bs[@]}
#	do
#		#echo "Executando p = "$s" com tamanho 100000 e delta "$i
#		(time ladrun -np $s dc_b_bs 100000 $i) > "results/output_b_p"$s"_s100000_d"$i"_bubble.txt" 2>&1
#		#echo "(time ladrun -np "$s" dc_b_bs 100000 "$i") > results/output_b_p"$s"_s100000_d"$i"_bubble.txt 2>&1"
#	done
#done

# roda versao C para quick...
for s in ${procs_c[@]}
do
	for i in ${deltas_qs[@]}
	do
		#echo "Executando p = "$s" com tamanho 1000000 e delta "$i
		(time ladrun -np $s dc_a_qs 1000000 $i) > "results/output_c_p"$s"_s1000000_d"$i"_quick.txt" 2>&1
		#echo "(time ladrun -np "$s" dc_a_qs 1000000 "$i") > results/output_c_p"$s"_s1000000_d"$i"_quick.txt 2>&1"
	done
done

# roda versao C para bubble...
for s in ${procs_c[@]}
do
	for i in ${deltas_bs[@]}
	do
		#echo "Executando p = "$s" com tamanho 100000 e delta "$i
		(time ladrun -np $s dc_a_bs 100000 $i) > "results/output_c_p"$s"_s100000_d"$i"_bubble.txt" 2>&1
		#echo "(time ladrun -np "$s" dc_a_bs 100000 "$i") > results/output_c_p"$s"_s100000_d"$i"_bubble.txt 2>&1"
	done
done
