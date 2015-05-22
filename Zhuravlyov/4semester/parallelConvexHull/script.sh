#!/bin/bash
INPUT_PROGRAM=./andrew.exe
CHECKER=./a.exe

for i in {01..34}
do
$INPUT_PROGRAM < $i > $i.txt
$CHECKER $i $i.txt $i.a
$INPUT_PROGRAM time < $i
done