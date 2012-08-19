/*
*	Othello.cpp
*	author: Ramon Sandoval
*/

#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <time.h>

#define END 10001
#define TIMESUP 10000
#define BASECASE 20
#define MOVELISTSIZE 2
#define ANDCONST -8
#define INFINITE 1000000
#define LASTSAMURAI 5

using namespace std;

struct twoInts {
	int a, b;
} thetwo;

int allAboard[8][8];
string yellow;
string red;
string black;
string white;
string normal;
string abnormal;
string clear;
int reesesPieces;
int mexicanStandoff;
int col[8];
int row[8];
int turnPresent;
int TuringTest[2];
int evidence[65*MOVELISTSIZE];
int zeroes;
int ones;
int twos;
int endOfTime;
int nodeOverload;
time_t start, end;

void setUp();
void endersGame();
void theCount();
int Objection(int (*gameBoard)[8], int i, int j, int turn, int base, int *evidence);
int Judas(int (*gameBoard)[8], int i, int j, int turn, int base);
void cleanAndClear();
int legalLoophole(int (*gameBoard)[8], int turn, int *evidence);
void emptyRow(int (*gameBoard)[8], int curRow);
void rowRowFightThePowa(int (*gameBoard)[8], int curRow);
void onBoard(int (*gameBoard)[8]);
int theGame();
void surveySays();
void timeLimit();
int turnItUp();
int Joshua();
int theWolfPack(int (*gameBoard)[8], int depth, int alpha, int beta, int *moves, int turn);
int heuristic(int (*gameBoard)[8]);
int changeSeats(int turn);
int andTheWinnerIs();
int onceAndFutureKing(int (*gameBoard)[8], int turn);
int countAllPieces();

int countAllPieces() {
	reesesPieces = 0;

	for(int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (allAboard[i][j] == 0 || allAboard[i][j] == 3)
				reesesPieces++; 
		}
	}
}

int andTheWinnerIs() {
	int white = 0;
	int black = 0;

	for(int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (allAboard[i][j] == 1)
				++white;
			else if (allAboard[i][j] == 2)
				++black;
		}
	}

	if (white > black) {
		cout << "White wins by " << white - black << " points!\n";
		if (TuringTest[0] == 1) {
			cout << "WHITE: I'VE WON. JUST AS PLANNED\n";
		}
		else {
			cout << "CONGRATULATIONS WHITE! YOU ARE A COOL GUY\n";
		}
		
		if (TuringTest[1] == 1) {
			cout << "BLACK: SYSTEM ERROR! SYSTEM ERROR! SYSTKJLSDFHSDFSO8YIHUITBFUIWOPLDHFHWWW...........\n";
		}
		else {
			cout << "BLACK WHITED OUT! LOST $250\n";
		}
	}
	else if (white < black) {
		cout << "Black wins by " << black - white << " points!\n";
		if (TuringTest[1] == 1) {
			cout << "BLACK: I'VE WON. JUST AS PLANNED\n";
		}
		else {
			cout << "CONGRATULATIONS BLACK! YOU ARE A COOL GUY\n";
		}
		
		if (TuringTest[0] == 1) {
			cout << "WHITE: SYSTEM ERROR! SYSTEM ERROR! SYSTKJLSDFHSDFSO8YIHUITBFUIWOPLDHFHWWW...........\n";
		}
		else {
			cout << "WHITE BLACKED OUT! LOST $250\n";
		}
	}
	else {
		cout << "TIE!\n";
		if (TuringTest[1] == 1) {
			cout << "BLACK: SUCH A STRANGE GAME. THE ONLY WINNING MOVE IS NOT TO PLAY\n";
		}
		if (TuringTest[0] == 1) {
			cout << "WHITE: SUCH A STRANGE GAME. THE ONLY WINNING MOVE IS NOT TO PLAY\n";
		}
	}
}

int onceAndFutureKing(int (*gameBoard)[8], int turn) {
	int guy = 0;
	int otherGuy = 0;
	
	for(int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (gameBoard[i][j] == turn)
				++guy;
			else if (gameBoard[i][j] == changeSeats(turn))
				++otherGuy;
		}
	}
	
	return (guy-otherGuy);
}

inline int changeSeats(int turn) {
	if (turn == 1)
		++turn;
	else
		--turn;
	
	return turn;
}

inline int wrongMove(int (*gameBoard)[8], int i, int j, int l, int m, int turn) {

	if (gameBoard[i][j] == 0 || gameBoard[i][j] == 3) {
		return 0;
	}
	else if (gameBoard[i][j] == turn) {
		if (gameBoard[l][m] == turn)
			return 0;
		else
			return -50;
	}
	else {
		if (gameBoard[l][m] == changeSeats(turn))
			return 0;
		else
			return 50;
	}
}

inline int otherWrongMove(int (*gameBoard)[8], int i, int j, int l, int m, int turn) {
	if (gameBoard[i][j] == 0 || gameBoard[i][j] == 3) {
		return 0;
	}
	else if (gameBoard[i][j] == turn) {
		if (gameBoard[l][m] == turn)
			return 0;
		else
			return -10;
	}
	else {
		if (gameBoard[l][m] == changeSeats(turn))
			return 0;
		else
			return 10;
	}
}

inline int legalMovesHO(int (*gameBoard)[8], int *legalMoves, int turn) {
	int val = 0;
	int i = 0;
	
	while (legalMoves[i] != END) {
		++val;
		i += MOVELISTSIZE;
	}
	
	i = 0;
	legalLoophole(gameBoard, changeSeats(turn), legalMoves);
	
	while (legalMoves[i] != END) {
		--val;
		i += MOVELISTSIZE;
	}
	
	return val;
}

inline int checkRowCol(int (*gameBoard)[8], int turn) {
	int val = 0;
	int other = changeSeats(turn);
	
	for (int i = 2; i < 6; ++ i) {
		if (gameBoard[0][i] == turn)
			++val;
		else if (gameBoard[0][i] == other)
			--val;
		
		if (gameBoard[7][i] == turn)
			++val;
		else if (gameBoard[7][i] == other)
			--val;
		
		if (gameBoard[i][0] == turn)
			++val;
		else if (gameBoard[i][0] == other)
			--val;
		
		if (gameBoard[i][7] == turn)
			++val;
		else if (gameBoard[i][7] ==other)
			--val;
	}
	
	return val;
}

int heuristic(int (*gameBoard)[8], int turn, int *legalMoves, int movesLeft) {
	int val = 0;
	int dummy[3];
	
	if (movesLeft <= 0)
		return END*onceAndFutureKing(gameBoard, turn);
	
	if (gameBoard[0][0] == 0) {
		if (Objection(gameBoard, 0, 0, changeSeats(turn), BASECASE, dummy) == 1)
			val -= 100;
	}
	else if (gameBoard[0][0] == turn)
		val += 100;
	else if (gameBoard[0][0] == 3) {
		if (Objection(gameBoard, 0, 0, changeSeats(turn), BASECASE, dummy) == 1)
			val -=100;
		else
			val +=100;
	}
	else
		val -= 100;
		
	if (gameBoard[7][0] == 0) {
		if (Objection(gameBoard, 7, 0, changeSeats(turn), BASECASE, dummy) == 1)
			val -= 100;
	}
	else if (gameBoard[7][0] == turn)
		val += 100;
	else if (gameBoard[7][0] == 3) {
		if (Objection(gameBoard, 7, 0, changeSeats(turn), BASECASE, dummy) == 1)
			val -=100;
		else
			val +=100;
	}
	else
		val -= 100;
		
	if (gameBoard[7][7] == 0) {
		if (Objection(gameBoard, 7, 7, changeSeats(turn), BASECASE, dummy) == 1)
			val -= 100;
	}
	else if (gameBoard[7][7] == turn)
		val += 100;
	else if (gameBoard[7][7] == 3) {
		if (Objection(gameBoard, 7, 7, changeSeats(turn), BASECASE, dummy) == 1)
			val -=100;
		else
			val +=100;
	}
	else
		val -= 100;
		
	if (gameBoard[0][7] == 0) {
		if (Objection(gameBoard, 0, 7, changeSeats(turn), BASECASE, dummy) == 1)
			val -= 100;
	}
	else if (gameBoard[0][7] == turn)
		val += 100;
	else if (gameBoard[0][7] == 3) {
		if (Objection(gameBoard, 0, 7, changeSeats(turn), BASECASE, dummy) == 1)
			val -=100;
		else
			val +=100;
	}
	else
		val -= 100;
		
	val += wrongMove(gameBoard,1,1,0,0,turn);
	val += wrongMove(gameBoard,6,1,7,0,turn);
	val += wrongMove(gameBoard,1,6,0,7,turn);
	val += wrongMove(gameBoard,6,6,7,7,turn);
	
	val += otherWrongMove(gameBoard,0,1,0,0,turn);
	val += otherWrongMove(gameBoard,1,0,0,0,turn);
	val += otherWrongMove(gameBoard,0,6,0,7,turn);
	val += otherWrongMove(gameBoard,7,1,7,0,turn);
	val += otherWrongMove(gameBoard,6,0,7,0,turn);
	val += otherWrongMove(gameBoard,1,7,0,7,turn);
	val += otherWrongMove(gameBoard,7,6,7,7,turn);
	val += otherWrongMove(gameBoard,6,7,7,7,turn);
	
	val += legalMovesHO(gameBoard, legalMoves, turn);
	val += checkRowCol(gameBoard, turn);

	return val;
}

int theWolfPack(int (*gameBoard)[8], int depth, int alpha, int beta, int *legalMoves, int turn, int movesLeft) {
	int evidence[65*MOVELISTSIZE];
	int copyBoard[8][8];
	int newAlpha;	

	++nodeOverload;
	if (nodeOverload > 2000) {
		nodeOverload = 0;
		time(&end);
		if (difftime(end, start) > endOfTime) { 
			return TIMESUP;
		} 
	}

	//cout << "depth = " << depth << " ";
	if (depth == 0) {
		/*onBoard(gameBoard);
		cout << "heuristic = " << heuristic(gameBoard, turn) << endl;
		string x;
		cin >> x;*/
		return heuristic(gameBoard, turn, legalMoves, movesLeft);
	}
	else if (legalMoves[0] == END) {
		//cout << "TRAPCARD!";
		legalLoophole(gameBoard, changeSeats(turn), evidence);
		if (evidence[0] == END)
			return END*onceAndFutureKing(gameBoard, turn);	//May be a problem here, check later
		
		newAlpha = -theWolfPack(gameBoard, depth, -beta, -alpha, evidence, changeSeats(turn), movesLeft); //May have to be depth -1, not just depth
		alpha = (alpha<newAlpha)?newAlpha:alpha;

		return alpha;
	}
	
	int k = 0;
	while (legalMoves[k] != END) {
		for(int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				copyBoard[i][j] = gameBoard[i][j];
			}
		}
		copyBoard[legalMoves[k]][legalMoves[k+1]] = turn;
		Judas(copyBoard, legalMoves[k], legalMoves[k+1], turn, BASECASE);
		legalLoophole(copyBoard, changeSeats(turn), evidence);
		//onBoard(gameBoard);
		
		newAlpha = -theWolfPack(copyBoard, depth-1, -beta, -alpha, evidence, changeSeats(turn), movesLeft - 1);
		if (abs(newAlpha) == TIMESUP)
			return TIMESUP;
		
		alpha = (alpha<newAlpha)?newAlpha:alpha;
		
		if (beta <= alpha)
			break;
		
		k += MOVELISTSIZE;
	}
	
	/*onBoard(gameBoard);
	cout << "heuristic at level " << depth << " = " << alpha << endl;
	string x;
	cin >> x;*/
	
	return alpha;
}

int Joshua() {

	int k;
	int alpha;
	int move = 0;
	int tempMove = 0;
	int newAlpha = 0;
	int copyBoard[8][8];
	int moves[64*MOVELISTSIZE];
	int divConst = 2;
	int inc = 0;

	time(&start);
	time(&end);

	if (evidence[MOVELISTSIZE] != END) {

		for(inc = 0; inc < reesesPieces; ++inc) {
			//cout << start << endl << end;
			//if (difftime(start, end) > 5) break;
			k = 0;
			alpha = -INFINITE;
			while (evidence[k] != END) {
				for(int i = 0; i < 8; ++i) {
					for (int j = 0; j < 8; ++j) {
						copyBoard[i][j] = allAboard[i][j];
					}
				}
				copyBoard[evidence[k]][evidence[k+1]] = turnPresent;
				Judas(copyBoard, evidence[k], evidence[k+1], turnPresent, BASECASE);
				legalLoophole(copyBoard, changeSeats(turnPresent), moves);
		
				newAlpha = -theWolfPack(copyBoard, inc, -INFINITE, INFINITE, moves, changeSeats(turnPresent), reesesPieces - 1);
				//cout << "moves: " << newAlpha << "\t";
				
				if (abs(newAlpha) == TIMESUP) {
					tempMove = -1;
					break;
				}
					
				if (newAlpha > alpha) {
					alpha = newAlpha;
					tempMove = k;
					divConst = 2;
					//if (alpha > END) break;
				}
				else if (newAlpha == alpha) {
					if (rand()%100 < 99/divConst) {
						alpha = newAlpha;
						tempMove = k;
					}
					++divConst;
				}
				
				k += MOVELISTSIZE;
			}
			
			//cout << "\nBREAK\n";
			
			if (tempMove >= 0)
				move = tempMove;
				
			time(&end);
			if (difftime(end, start) > endOfTime) break;
			if (alpha > END) {
				/*cout << "ATATATATATATA! You're already dead" << endl;
				string alreadydead;
				cin >> alreadydead;*/
				break;
			}
		}
	}
	
	//cout << "The time is: " << end << " and " << start << endl;
	cout << "The time is: " << difftime(end, start) << endl;
	cout << "The number of moves ahead is: " << inc << endl;
	allAboard[evidence[move]][evidence[move + 1]] = turnPresent;
	thetwo.a = evidence[move];
	thetwo.b = evidence[move + 1];
	reesesPieces--;
	Judas(allAboard, evidence[move], evidence[move+1], turnPresent, BASECASE);
}

int zeroWing() {
	int zeroes = 0;

	for(int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (allAboard[i][j] == 0) {
				++zeroes;
			}
		}
	}
	
	return zeroes;
}

int readjust() {
	for(int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (allAboard[i][j] > LASTSAMURAI) {
				allAboard[i][j] -= LASTSAMURAI;
			}
		}
	}
}

void setUp() {
	col[0] = -1;
	col[1] = 0;
	col[2] = 1;
	col[3] = -1;
	col[4] = 1;
	col[5] = -1;
	col[6] = 0;
	col[7] = 1;
	
	row[0] = -1;
	row[1] = -1;
	row[2] = -1;
	row[3] = 0;
	row[4] = 0;
	row[5] = 1;
	row[6] = 1;
	row[7] = 1;
	
	clear = "\x1b\[1;42m";
	yellow = "\x1b\[1;43m";
	red = "\x1b\[1;41m";
	black = "\x1b\[1;40m";
	white = "\x1b\[1;34;47m";
	normal = "\x1b\[1;39;46m";
	abnormal = "\x1b\[1;39;49m";
	
	thetwo.a = 0;
	thetwo.b = 0;
	nodeOverload = 0;
}

void endersGame() {
	cout << "    ";
	for (int i = 0; i < 49; ++i) {
		cout << "-";
	}
	cout << "\n";
}

void theCount() {
	cout << "\n\n";

	cout << normal;
	cout << "     ";
	for (int i = 0; i < 8; ++i) {
		cout << "  " << i << "   ";
	}
	cout << "\n\n";
}

int Objection(int (*gameBoard)[8], int i, int j, int turn, int base, int *evidence) {
	int newi, newj;
	int evidenceStack = 0;
	switch(base) {
		case BASECASE:
			for (int k = 0; k < 8; ++k) {
				newi = i + row[k];
				newj = j + col[k];
				
				if (newi & ANDCONST || newj & ANDCONST)
					continue;
				else if (gameBoard[newi][newj] == 0 || gameBoard[newi][newj] == 3) {
					continue;
				}
				else if (gameBoard[newi][newj] != turn) {
					if (Objection(gameBoard, newi, newj, turn, k, evidence) == 1) {
						//cout << "legal move at " << (char)(i + 'A') << j << endl;
						evidence[evidenceStack++] = i;
						evidence[evidenceStack++] = j;
						evidence[evidenceStack] = END;
						return 1;
					}
					else {
						continue;
					}
				}
			}
			break;
		default:
			newi = i + row[base];
			newj = j + col[base];
			
			if (newi & ANDCONST || newj & ANDCONST)
				return 0;
			else if (gameBoard[newi][newj] == turn) {
				return 1;
			}
			else if (gameBoard[newi][newj] == 0 || gameBoard[newi][newj] == 3)
					return 0;
			else if (gameBoard[newi][newj] != turn) {
				return Objection(gameBoard, newi, newj, turn, base, evidence);
			}
	}
	
	gameBoard[i][j] = 0;
	return 0;
}

int Judas(int (*gameBoard)[8], int i, int j, int turn, int base) {
	int newi, newj;
	
	switch(base) {
		case BASECASE:
			for (int k = 0; k < 8; ++k) {
				newi = i + row[k];
				newj = j + col[k];
				
				if (newi & ANDCONST || newj & ANDCONST)
					continue;
				else if (gameBoard[newi][newj] == 0 || gameBoard[newi][newj] == 3) {
					continue;
				}
				else if (gameBoard[newi][newj] != turn) {
					Judas(gameBoard, newi, newj, turn, k);
				}
			}
			break;
		default:
			newi = i + row[base];
			newj = j + col[base];
			
			if (newi & ANDCONST || newj & ANDCONST)
				return 0;
			else if (gameBoard[newi][newj] == turn) {
				gameBoard[i][j] = turn;
				return 1;
			}
			else if (gameBoard[newi][newj] == 0 || gameBoard[newi][newj] == 3)
					return 0;
			else if (gameBoard[newi][newj] != turn) {
				if (Judas(gameBoard, newi, newj, turn, base) == 1){
					gameBoard[i][j] = turn;
					return 1;
				}
			}
	}
	return 0;
}

void cleanAndClear(int (*gameBoard)[8]) {
	for(int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (gameBoard[i][j] == 3) {
				gameBoard[i][j] = 0;
			}
		}
	}
}

int legalLoophole(int (*gameBoard)[8], int turn, int *evidence) {
	cleanAndClear(gameBoard);
	evidence[0] = END;
	int* startEvidence = evidence;
	
	for(int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (gameBoard[i][j] == 0) {
				if (Objection(gameBoard, i, j, turn, BASECASE, startEvidence) == 1) {
					gameBoard[i][j] = 3;
					startEvidence += MOVELISTSIZE;
				}	
			}
		}
	}
	
	return evidence[0];
}

void emptyRow(int (*gameBoard)[8], int curRow) {
	cout << normal;
	cout << "    |";
	for (int i = 0; i < 8; ++i) {
		switch (gameBoard[curRow][i]) {
			case (2 + LASTSAMURAI):
				cout << red << "  " << black << " " << red  << "  " << normal << "|";
				break;				
			case (1 + LASTSAMURAI):
				cout << red << "  " << white << " " << red  << "  " << normal << "|";
				break;			
			case 3:
				cout << yellow << "     " << normal << "|";
				break;
			case 2:
				cout << "  " << black << " " << normal  << "  " << "|";
				break;
			case 1:
				cout << "  " << white << " " << normal  << "  " << "|";
				break;
			default: 
				cout << "     |";
		}
	}
	
	cout << endl;
}

void rowRowFightThePowa(int (*gameBoard)[8], int curRow) {
	endersGame();
	emptyRow(gameBoard, curRow);
	cout << " " << (char)(curRow + 'A') << "  |";
	char piece;
	for (int i = 0; i < 8; ++i) {
		switch(gameBoard[curRow][i]) {
			case 0: piece = ' '; cout << "  " << piece << "  |";
				break;
			case 1: piece = 'O'; cout << white << "  " << piece << "  " << normal << "|";
				break;
			case 2: piece = '*'; cout << black << "  " << piece << "  " << normal << "|";
				break;
			case 3: piece = ' '; cout << yellow << "  " << piece << "  " << normal << "|";
				break;
			case (1 + LASTSAMURAI): piece = 'O'; cout << white << "  " << piece << "  " << normal << "|";
				break;
			case (2 + LASTSAMURAI): piece = '*'; cout << black << "  " << piece << "  " << normal << "|";
				break;
			default: 
				cout << "     |";
		}
	}
	
	cout << endl;
	emptyRow(gameBoard, curRow);
}

void onBoard(int (*gameBoard)[8]) {
	theCount();
	for (int i = 0; i < 8; ++i) {
		rowRowFightThePowa(gameBoard, i);
	}
	endersGame();
	cout << abnormal;
}

int theGame() {
	
	if (legalLoophole(allAboard, turnPresent, evidence) == END) {
		if (mexicanStandoff == 1) {
			allAboard[thetwo.a][thetwo.b] += LASTSAMURAI;
			onBoard(allAboard);
			allAboard[thetwo.a][thetwo.b] -= LASTSAMURAI;
			return 2;
		}
		else {
			allAboard[thetwo.a][thetwo.b] += LASTSAMURAI;
			onBoard(allAboard);
			allAboard[thetwo.a][thetwo.b] -= LASTSAMURAI;
			mexicanStandoff = 1;
			cout << "NO MOVE FOR YOU!\n";
			return 1;
		}
	}
	
	allAboard[thetwo.a][thetwo.b] += LASTSAMURAI;
	onBoard(allAboard);
	allAboard[thetwo.a][thetwo.b] -= LASTSAMURAI;
	mexicanStandoff = 0;
	string dummy;
	int row, col;
	
	if (TuringTest[turnPresent-1] == 1) {
		cout << "\nSWORD OF OMENS, GIVE ME SIGHT BEYOND SIGHT!\n";
		Joshua();
		cout << "\n";
		return 0;
	}
	
	if (turnPresent == 1) {
		cout <<"\nYOUR MOVE WHITE!: ";
	}
	else {
		cout <<"\nYOUR MOVE BLACK!: ";
	}
	
	while(1) {
		cin >> dummy;

		if (dummy[0] >= 'a' && dummy[0] <= 'h')
			row = dummy[0]-'a';
		else
			row = dummy[0]-'A';

		col = dummy[1]-'0';
		
		if (dummy == "exit" || dummy == "EXIT")
			exit(1);

		if (row < 0 || row > 7 || col < 0 || col > 7) {
			cout << "How cute...BUT IT'S WRONG!: ";
		}
		else if (allAboard[row][col] != 3) {
			cout << "How cute...BUT IT'S WRONG!: ";
		}
		else {
			break;
		}
	}
	allAboard[row][col] = turnPresent;
	thetwo.a = row;
	thetwo.b = col;
	reesesPieces--;
	Judas(allAboard, row, col, turnPresent, BASECASE);
	
	return 0;
}

void surveySays() {
	string dummy;
	
	while(1) {
		cout << "Specify board position? (Y/N): ";
		cin >> dummy;
		
		if (dummy == "Y" || dummy == "y") {
			cout << "Schoolyard rules eh?\n";
			while(1) {
				cout << "Enter name of text file: ";
				cin >> dummy;
		
				ifstream input;
				input.open(dummy.c_str());
				if(!input.is_open()) {
					cerr << "Sorry, but your file is in another castle\n";
					continue;
				}
				for(int i = 0; i < 8; ++i) {
					input >> dummy;
					if (dummy.size() < 8 || input.eof()) {
						cout << "Yo file is outta style\n";
						return;
					}
					
					for (int j = 0; j < 8; ++j) {
						if (dummy[j] >= '0' && dummy[j] <= '2')
							allAboard[i][j] = dummy[j] - '0';
						else {
							cout << "Yo file is outta style\n";
							return;
						}
					}
				}
				return;
			}
		}
		else if (dummy == "N" || dummy == "n") {
			cout << "Oldschool is the best school\n";
			break;
		}
		else {
			cout << "No comprende Senore\n";
		}
	}
}

void timeLimit() {
	string dummy;
	while(1) {
		cout << "Enter a time limit for the AI (3-60): ";
		cin >> dummy;
		
		int timeLimit = strtol(dummy.c_str(), NULL, 0);
		
		if (timeLimit >= 3 && timeLimit <= 60) {
			endOfTime = timeLimit/2;
			break;
		}
		else {
			cout << "No comprende Senore\n";
		}
	}
}

int turnItUp() {
	string dummy;
	
	while(1) {
		cout << "Is the white player human?: ";
		cin >> dummy;

		if (dummy == "Y" || dummy == "y") {
			cout << "Exactly what a robot would say\n";
			TuringTest[0] = 0;
			break;
		}
		else if (dummy == "N" || dummy == "n") {
			cout << "ACTIVATE TERMINATOR\n";
			TuringTest[0] = 1;
			break;
		}
		else {
			cout << "No comprende Senore\n";
		}
	}
	
	while(1) {
		cout << "Is the black player human?: ";
		cin >> dummy;

		if (dummy == "Y" || dummy == "y") {
			cout << "Exactly what a robot would say\n";
			TuringTest[1] = 0;
			break;
		}
		else if (dummy == "N" || dummy == "n") {
			cout << "ACTIVATE TERMINATOR\n";
			TuringTest[1] = 1;
			break;
		}
		else {
			cout << "No comprende Senore\n";
		}
	}
}

int main() {

	srand(time(NULL));
	setUp();	
	
	allAboard[3][3] = 1;
	allAboard[3][4] = 2;
	allAboard[4][3] = 2;
	allAboard[4][4] = 1;
	turnPresent = 2;
	mexicanStandoff = 0;
	endOfTime = 1;

	surveySays();
	timeLimit();
	turnItUp();

	zeroes = zeroWing();
	countAllPieces();
	
	while(zeroes > 0) {
		int test = theGame();
		
		if (test == 2) {
			//cout << "GOOOOAAAAALLLLLLLL!!!!!!!!!\n";
			andTheWinnerIs();
			exit(1);
		}
		else if (test == 0) {
			--zeroes;
		}
		
		turnPresent = changeSeats(turnPresent);
			
		cout << "\n-------------------------------------------------------\n";
	}
	
	onBoard(allAboard);
	//cout << "GOOOOAAAAALLLLLLLL!!!!!!!!!\n";
	andTheWinnerIs();
	exit(1);
}

