#!/bin/bash
cd colors
echo "Generating colour scheme..."
./colors -n 16 $1 | ../urxvt.sh >> ../xresourses.txt


echo "Adding colour scheme to Xresourses.txt"
sed -i 's/*/urxvt./g' ../xresourses.txt
