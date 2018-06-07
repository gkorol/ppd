#!/bin/bash

sacos=("100" "500" "1000" "5000" "10000")

for i in ${sacos[@]}
  do
    echo "Executando sequencial com tamanho [1000 x "$i"]"
    #./mestre_escravo_seq 1000 $i > "results/output_seq_l1000_c"$i"_bubble.txt"
    ./mestre_escravo_seq 1000 $i > "results/output_seq_l1000_c"$i"_quick.txt"
done
