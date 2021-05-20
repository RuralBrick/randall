#!/usr/bin/bash

echo "Testing w/ no opt args..."
for n in {0..100}
do
    x=$(./randall $n | wc -c)
    if [ $x -ne $n ]
    then
       echo "Failed at NBYTES=$n"
       exit 1
    fi
done
echo "Done"

input[0]="rdrand"
input[1]="mrand48_r"
input[2]="/dev/urandom"
size=${#input[@]}

echo "Testing w/ i args..."
for n in {0..100}
do
    index=$(($RANDOM % $size))
    i=${input[$index]}
    x=$(./randall $n -i $i | wc -c)
    if [ $x -ne $n ]
    then
       echo "Failed at NBYTES=$n INPUT=$i"
       exit 1
    fi
done
echo "Done"

echo "Testing w/ o args"
for n in {2..100}
do
    if [ $(($RANDOM % 2)) -eq 0 ]
    then
	o="stdio"
    else
	max=$(($n - 1))
	o=$(($RANDOM % $max))
	o=$(($o + 1))
    fi
    x=$(./randall $n -o $o | wc -c)
    if [ $x -ne $n ]
    then
       echo "Failed at NBYTES=$n OUTPUT=$o"
       exit 1
    fi
done
echo "Done"

echo "Testing w/ i and o args..."
for n in {2..300}
do
    index=$(($RANDOM % $size))
    i=${input[$index]}
    if [ $(($RANDOM % 2)) -eq 0 ]
    then
	o="stdio"
    else
	max=$(($n - 1))
	o=$(($RANDOM % $max))
	o=$(($o + 1))
    fi
    x=$(./randall $n -i $i -o $o | wc -c)
    if [ $x -ne $n ]
    then
       echo "Failed at NBYTES=$n INPUT=$i OUTPUT=$o"
       exit 1
    fi
done
echo "Done"

echo "Testing finished"


