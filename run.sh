#!/bin/bash

if [[ $EUID -ne 0 ]]; then
	echo "You forget sudo!"
	exit 1
fi

dmesg -C
for path in OS_PJ1_Test/FIFO*; do
	name=`basename -s .txt "${path}"`
        echo "Processing ${name}..."

	./scheduler < "OS_PJ1_Test/${name}.txt" | tee "output/${name}_stdout.txt"
	dmesg -c | grep Project1 | tee "output/${name}_dmesg.txt"
done

chown -R os:os output/