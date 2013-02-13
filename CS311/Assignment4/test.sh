#!/bin/bash
echo "Bash version ${BASH_VERSION}..."

for name in test.txt pride.txt moby.txt shake.txt books.txt
do
    echo ""
    for i in {1..50..2}
    do
        echo "Results for $i processes on $name" 
        time ./uniqify $i < $name >> results 
        echo ""
    done
done

