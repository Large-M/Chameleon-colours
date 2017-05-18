Chameleon Colours
=
A shell script which automatically applies 2f30's color script to your terminal emulator of choice.

Using Chameleon
-
1. Once you've cloned the repository, move into the colors directory and run these commands: `make && cd bin && gcc hex2col.c -o hex2col && mv hex2col ../ && cd ..`

2. To use the Chameleon you'll need to run the script file and pass the image you want to extract the colour from as an argument. Like this: 
`./chameleon.sh ~/Pictures/Wallpapers/choice.png`
> **Note**
> * All files that are passed as arguments need to be .png files in the current version of this script

Currently Supported Terminals
-
* RXVT Unicode

##### 

Special Thanks to
-
[**2f30**](git.2f30.org/), The developer of the colour extractor