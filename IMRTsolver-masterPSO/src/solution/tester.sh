 #!/bin/bash
 while [$CONT -lt 3 ]; do
    ./PSO --max_iter 2 --size 3 >> resultados.txt
    let CONT=CONT+1
done
cat resultados.txt | grep "Best Solution found"