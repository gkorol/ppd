#!/bin/bash

# sacos devem fechar 100000 (bs) ou 1000000 (qs) para comparar com outros modelos
sacos_2_bs=("50000")
sacos_4_bs=("25000")
sacos_8_bs=("12500")
sacos_16_bs=("6250")
sacos_32_bs=("3125")
sacos_64_bs=("1563")

sacos_2_qs=("500000")
sacos_4_qs=("250000")
sacos_8_qs=("125000")
sacos_16_qs=("62500")
sacos_32_qs=("31250")
sacos_64_qs=("15625")

#deltas para 10% 20% 25% 30% 40%
deltas_2_bs=("500" "10000" "12500" "15000" "20000")
deltas_4_bs=("2500" "5000" "6250" "7500" "10000")
deltas_8_bs=("1250" "2500" "3125" "3750" "5000")
deltas_16_bs=("625" "1250" "1563" "1875" "2500")
deltas_32_bs=("313" "625" "782" "938" "1250")
deltas_64_bs=("157" "313" "391" "469" "626")

deltas_2_qs=("5000" "100000" "125000" "150000" "200000")
deltas_4_qs=("25000" "50000" "62500" "75000" "100000")
deltas_8_qs=("12500" "25000" "31250" "37500" "50000")
deltas_16_qs=("6250" "12500" "15630" "18750" "25000")
deltas_32_qs=("3125" "6250" "7813" "9375" "12500")
deltas_64_qs=("1563" "3125" "3907" "4688" "6250")

# procs=("2" "4" "8" "16" "32" "64")

# Bubble Sort
# 2 procs
for s in ${deltas_2_bs[@]}
do
	for i in ${sacos_2_bs[@]}
	do
		echo "Executando p = 2 com saco "$s" e delta "$i
		(time ladrun -np 2 fp_bs $i $s) > "results/output_p2_s"$i"_d"$s"_bubble.txt" 2>&1
	done
done

# 4 procs
for s in ${deltas_4_bs[@]}
do
	for i in ${sacos_4_bs[@]}
	do
		echo "Executando p = 4 com saco "$s" e delta "$i
		(time ladrun -np 4 fp_bs $i $s) > "results/output_p4_s"$i"_d"$s"_bubble.txt" 2>&1
	done
done

# 8 procs
for s in ${deltas_8_bs[@]}
do
	for i in ${sacos_8_bs[@]}
	do
		echo "Executando p = 8 com saco "$s" e delta "$i
		(time ladrun -np 8 fp_bs $i $s) > "results/output_p8_s"$i"_d"$s"_bubble.txt" 2>&1
	done
done

# 16 procs
for s in ${deltas_16_bs[@]}
do
	for i in ${sacos_16_bs[@]}
	do
		echo "Executando p = 16 com saco "$s" e delta "$i
		(time ladrun -np 16 fp_bs $i $s) > "results/output_p16_s"$i"_d"$s"_bubble.txt" 2>&1
	done
done

# 32 procs
for s in ${deltas_32_bs[@]}
do
	for i in ${sacos_32_bs[@]}
	do
		echo "Executando p = 32 com saco "$s" e delta "$i
		(time ladrun -np 32 fp_bs $i $s) > "results/output_p32_s"$i"_d"$s"_bubble.txt" 2>&1
	done
done

# 64 procs
for s in ${deltas_64_bs[@]}
do
	for i in ${sacos_64_bs[@]}
	do
		echo "Executando p = 64 com saco "$s" e delta "$i
		(time ladrun -np 64 fp_bs $i $s) > "results/output_p64_s"$i"_d"$s"_bubble.txt" 2>&1
	done
done

# Quick Sort
# 2 procs
for s in ${deltas_2_qs[@]}
do
	for i in ${sacos_2_qs[@]}
	do
		echo "Executando p = 2 com saco "$s" e delta "$i
		(time ladrun -np 2 fp_qs $i $s) > "results/output_p2_s"$i"_d"$s"_quick.txt" 2>&1
	done
done

# 4 procs
for s in ${deltas_4_qs[@]}
do
	for i in ${sacos_4_qs[@]}
	do
		echo "Executando p = 4 com saco "$s" e delta "$i
		(time ladrun -np 4 fp_qs $i $s) > "results/output_p4_s"$i"_d"$s"_quick.txt" 2>&1
	done
done

# 8 procs
for s in ${deltas_8_qs[@]}
do
	for i in ${sacos_8_qs[@]}
	do
		echo "Executando p = 8 com saco "$s" e delta "$i
		(time ladrun -np 8 fp_qs $i $s) > "results/output_p8_s"$i"_d"$s"_quick.txt" 2>&1
	done
done

# 16 procs
for s in ${deltas_16_qs[@]}
do
	for i in ${sacos_16_qs[@]}
	do
		echo "Executando p = 16 com saco "$s" e delta "$i
		(time ladrun -np 16 fp_qs $i $s) > "results/output_p16_s"$i"_d"$s"_quick.txt" 2>&1
	done
done

# 32 procs
for s in ${deltas_32_qs[@]}
do
	for i in ${sacos_32_qs[@]}
	do
		echo "Executando p = 32 com saco "$s" e delta "$i
		(time ladrun -np 32 fp_qs $i $s) > "results/output_p32_s"$i"_d"$s"_quick.txt" 2>&1
	done
done

# 64 procs
for s in ${deltas_64_qs[@]}
do
	for i in ${sacos_64_qs[@]}
	do
		echo "Executando p = 64 com saco "$s" e delta "$i
		(time ladrun -np 64 fp_qs $i $s) > "results/output_p64_s"$i"_d"$s"_quick.txt" 2>&1
	done
done
