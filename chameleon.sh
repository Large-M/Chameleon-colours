#!/bin/bash
cd colors
echo "Generating colour scheme..."
./colors -n 16 $1 | ./script.sh >> ~/Documents/Chameleon/.Xresources


echo "Adding colour scheme to Xresources..."
sed -i 's/*/urxvt./g' ~/Documents/Chameleon/.Xresources

echo "Merging Xresources..."
xrdb -merge ~/.Xresources
echo "Done"
