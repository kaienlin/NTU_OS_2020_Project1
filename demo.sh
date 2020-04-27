#!/bin/bash

if [[ $EUID -ne 0 ]]; then
	echo "You forget sudo!"
	exit 1
fi

echo ======== kernel version ========
uname -r
echo ================================

make clean && make
dmesg -C
for name in TIME_MEASUREMENT FIFO_1 PSJF_2 RR_3 SJF_4; do
        echo "Processing ${name}..."

	./scheduler < "OS_PJ1_Test/${name}.txt"
	dmesg -c | grep Project1
        sleep 2
done

make clean