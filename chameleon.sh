#!/bin/bash
cd colors
./colors -n 16 $1 | ./script.sh >> ../output.txt | sed -i "s/\*/urxvt\./g"
