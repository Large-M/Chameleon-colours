#!/bin/bash
cd colors
./colors -n 16 $1 | ./script.sh >> ../.Xresources
sed -i 's/*/urxvt./g' ~/Documents/Chameleon/.Xresources
