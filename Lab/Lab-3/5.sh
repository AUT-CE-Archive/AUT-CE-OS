#!/bin/bash

read -p "Shape number: " num

case $num in
  1)
    for j in {1..5}
    do
        for (( i=1; i<=$j; i++ ))
        do
            echo -n "$j"
        done
        echo ""
    done
  ;;
  2)
    n=6
    for (( i=1; i<=$n ; i++))
    do
        # Print Spaces
        for (( j=$n; j>=i; j-- ))
        do echo -n " "
        done

        # Print Stars
        for (( k=1; k<=i; k++ ))
        do echo -n " *"
        done

        echo "" # New Line
    done

    n_r=`expr $n - 1`

    for (( i=$n_r; i>=1; i--))
    do
        # Print Spaces
        for (( j=i; j<=$n_r; j++ ))
        do
            if [ $j -eq $n_r ]
                then echo -n " "
            fi

            echo -n " "
        done

        # Print Stars
        for (( k=1; k<=i; k++ ))
        do echo -n " *"
        done

        echo "" # New Line
    done
  ;;
  3)
    for i in {1..5}
    do  
        for (( j=0; j<$i; j++ ))
        do  
            echo -n "|"

            if ! [ $j -eq $(($i - 1)) ]
            then
                echo -n " "
            fi

        done
        echo "_"
    done
  ;;
esac
echo "$result"

$SHELL