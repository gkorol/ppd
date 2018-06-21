#!/bin/bash

sacos=("100000")

for i in ${sacos[@]}
  do
    echo "Executando sequencial com tamanho "$i
    ./mestre_escravo_seq $i> "results/output_seq_l"$i"_bubble.txt"
    #./mestre_escravo_seq $i > "results/output_seq_l"$i"_quick.txt"
done
