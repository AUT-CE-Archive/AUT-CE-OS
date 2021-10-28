#!/bin/bash

while :
do
    read -p "Enter number: " num

    sum=0
    reverse=0
    while [ $num -gt 0 ]
    do
        digit=$(( $num % 10 ))
        sum=$(( $sum + $digit ))
        reverse=$(( $reverse * 10 + $digit ))
        num=$(( $num / 10 ))
    done
    echo "Reversed number: $reverse"
    echo "Sum of digits: $sum"
    echo ""
done

$SHELL