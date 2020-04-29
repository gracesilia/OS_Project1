#!/bin/bash

in_folder="testdata"

sudo dmesg -C

for i in TIME_MEASUREMENT FIFO_1 PSJF_2 RR_3 SJF_4;
do
	echo Demoing $i 
	sudo ./main < $in_folder/$i.txt
	sudo dmesg -c | grep Project1
	echo ""
done