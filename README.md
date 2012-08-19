To compile, just write g++ Othello.cpp -o Othello.exe, or use the MakeFile included.

Black player always goes first, though both players can be either a computer or a human. To choose a starting position, a sample file should be set up as follows:
00000001
00002220
00000001
00002220
01100000
02220100
01122002
01112222

Where 1’s correspond to white pieces and 2 corresponds to black pieces.

    The AI checks every 2000 nodes for the time and stops when the time reaches half the time limit. The AI time limit is given as 3-60 seconds. The legal moves will be displayed as a yellow spot on the board, and the last move will be indicated by red. Moves can be taken by writing the row letter and the column number, ie A4 or b5. When there are no legal moves, the game skips the player. When it sees that both have no legal moves, the game ends. After the game is over, the program displays which player wins and gives the difference in number of pieces as a point value.
