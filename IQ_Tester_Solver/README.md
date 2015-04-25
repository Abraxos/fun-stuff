# IQ Tester Puzzle

![IQ Tester Puzzle](http://raw.githubusercontent.com/Abraxos/fun-stuff/master/IQ_Tester_Solver/iqtester.jpg)

This is just a silly little puzzle that my friend showed me and dared me to see how long it would take to code up a program to solve it. Naturally with python it only took about a half hour. There is a picture of the puzzle in the accompanying file: iqtester.jpg

The basic idea of the game is that starting with one pin removed in any one of the 15 locations on the board, the player must eliminate all but one pin. The way to eliminate a pin is to jump over it. At least that's the easy mode. There is also a medium mode, which requires that the last pin standing be in the location that started out empty, and a hard mode, which requires that there be 8 pins and no possible moves on the board. Fun fact: if you wanna mess with someone, ask them to solve the puzzle in medium mode starting from pin 7 (its the center pin, I enumerated all the pin holes as 0-14, top-to-bottom and left-to-right, to write a program to solve it).

Anyway, the program is fairly simple, it uses a DFS approach to calculate all possible states and then when a solution state is found, it returns the current DFS stack which also represents the path taken to get the solution.
