#!/bin/bash
cd colors
echo "Generating colour scheme..."
./colors -n 16 $1 | ./xresourses.sh >> ../xresourses.txt


echo "Adding colour scheme to Xresourses.txt"
#sed -i 's/*./*/g' ../xresourses.txt
