#!/bin/bash
CONT=0
while [	$CONT -lt 5 ]; do
	./PSO --max_iter 10 --size 10 |grep "Best solution found" >> resultados.txt
	let CONT=CONT+1
done
