#!/bin/bash

read -p "Enter x: " x
read -p "Enter y: " y
read -p "Enter filename: " filename

line=0
while read p; do
    if [ $line -ge $x ] && [ $line -lt $y ]; then
        echo "$p"
    fi

    ((line++))
done < $filename

$SHELL