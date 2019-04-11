#!/bin/bash
CONT=0
while [	$CONT -lt 5 ]; do
	./PSO --max_iter 100 --size 100 --iner 0.957893216 --c1 0.856789321 --c2 0.356789321 |grep "Best solution found" >> resultados.txt
	let CONT=CONT+1
done
