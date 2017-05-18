#!/bin/bash
cd colors
echo "Generating colour scheme..."
./colors -n 16 $1 | ./script.sh >> ../urxvt.txt


echo "Adding colour scheme to Xresources..."
sed -i 's/*/urxvt./g' ../urxvt.txt

echo "Merging Xresources..."
xrdb -merge ~/.Xresources
echo "Done"
