#!/bin/bash
CONT=0
while [	$CONT -lt 5 ]; do
	./PSO --max_iter 2 --size 5 |grep "Best solution found" >> resultados.txt
	let CONT=CONT+1
done
