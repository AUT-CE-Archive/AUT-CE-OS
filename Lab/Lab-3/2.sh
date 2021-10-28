#!/bin/bash

read -p "Enter first number: " num1
read -p "Enter second number: " num2

# Input type of operation
echo "From operators:"
echo "1. +"
echo "2. -"
echo "3. *"
echo "4. /"
read -p "Select operator number: " op

case $op in
  1) result=$((num1 + num2))
  ;;
  2) result=$((num1 - num2))
  ;;
  3) result=$((num1 * num2))
  ;;
  4) result=$((num1 / num2))
  ;;
esac
echo "Result : $result"

$SHELL