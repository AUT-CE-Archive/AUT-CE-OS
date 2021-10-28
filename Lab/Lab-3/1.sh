#!/bin/bash

read -p "Enter first number: " num1
read -p "Enter second number: " num2

# c
re='^[0-9]+$'
if ! [[ $num1 =~ $re && $num2 =~ $re ]] ;
    then echo "Woops! Numbers supported only - Terminated ):"; $SHELL
fi

# a
sum=$((num1 + num2 ))
echo "Sum: $sum"

# b
if [ $num1 -gt $num2 ]
    then echo "Greater Number: $num1"
else echo "Greater Number: $num2"
fi

$SHELL