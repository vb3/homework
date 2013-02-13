#!/bin/bash
echo "Bash version ${BASH_VERSION}..."
LOG=results.log
exec > $LOG 2>&1

for type in t p
do
    echo ""
    for i in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192
    do
        echo "Results for $i number of $type" 
        time primes $type $i 4294967295 n  
        echo ""
        echo ""
    done
done

