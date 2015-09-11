# Text-to-Image Converter

This is a very small python script designed to convert a simple text file to an image of a given format. The program is quite simple and requires a text file input, an output filepath, and a width of the image. The rest is derived automatically: The type of image is determined by the extension on the output filepath and the height of the image depends on the amount of text as well as the font size. The program defaults to the font given to it in the same directory as the script file under the name text2image_font.ttf and the default size is 15, but these can be changed to use any font and size you want.

## Standard Usage:

The typical way to use this program is as follows:

    $ text2image -i /mydirectory/mytextfile.txt -o /mydirectory/myimagefile.jpg -w 300
Where `-i` indicates the input text file, `-o` indicates the output file and `-w` indicates the width of the image. Please note that you can use any image format recognized by PIL so `.jpg`, `.png`, and `.tiff` are all equally valid as well as many others. You can also set the width, in pixels, to be anything you like. Be aware however that in order for this program to function properly the text file must exist and be valid, there must not be a file at the given output path, and the width must be an integer in pixels.

A more advanced usage might look something like this:

    $ text2image -i /mydirectory/mytextfile.txt -o /mydirectory/myimagefile.jpg -w 300 -f /mydirectory/myfont.ttf -s 20
Where `-f` indicates the font (which must be in a valid TTF format) and `-s` indicates the font size in points. As mentioned earlier, these two parameters default to `/<myscriptdirectory>/text2image_font.ttf` and 15 respectively.

## Installation:

The installation should be very simple, just copy the files in this repo into a directory of your choice on your computer such as `/opt/text2image/` and make sure that the script file is executable. Then make a symlink to it in `/usr/local/bin` so that you can execute it from anywhere on your system (again assuming a Linux system).

If you do not want to make a symlink in your `/usr/local/bin` directory, then you should still be able to execute the file the typical linux way by providing the path:

    $ ./text2image -i /mydirectory/mytextfile.txt -o /mydirectory/myimagefile.jpg -w 300 -f /mydirectory/myfont.ttf -s 20

### Requirements:

This program requires Python 2.7 and the PIL or Pillow libraries (although it was written and tested using Pillow). As far as I know those should be available on Windows systems, but do not come to me with questions on how to install it.

## Bugs:

As of right now I do not know of any bugs, but that does not mean that there aren't any (for that matter they are all but guaranteed). This program has only been tested on Linux using python 2.7 and the Pillow library which is a fork of PIL. If you discover any bugs please report them through the github issues page on this repo and I will try to fix them.
