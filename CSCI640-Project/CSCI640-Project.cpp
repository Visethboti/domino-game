#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string>
#include <iostream>
#include <deque>

using namespace std;

class CRandom {
    public:
		CRandom(){}  // constructor
		~CRandom(){} // destructor

		int getRandomPublic(int rangeLow, int rangeHigh){
			int myRand_scaled;

			myRand_scaled=getRandomPrivate(rangeLow, rangeHigh);

		    return myRand_scaled;
		}

		void shuffle(int array[], int size)
		{
			// Initialize seed randomly
			srand(time(0));

			for (int i = 0; i < size; i++)
			{
				// Random for remaining positions.
				int r = i + (rand() % (size - i));

				swap(array[i], array[r]);
			}
		}

	private:
		// Generates uniform distribution between rangeLow and rangeHigh
		int getRandomPrivate(int rangeLow, int rangeHigh) {
		    double myRand = rand()/(1.0 + RAND_MAX);
		    int range = rangeHigh - rangeLow + 1;
		    int myRand_scaled = (myRand * range) + rangeLow;
		    return myRand_scaled;
		}

	protected:
		// Generates uniform distribution between rangeLow and rangeHigh
		int getRandomProtected(int rangeLow, int rangeHigh) {
		    double myRand = rand()/(1.0 + RAND_MAX);
		    int range = rangeHigh - rangeLow + 1;
		    int myRand_scaled = (myRand * range) + rangeLow;
		    return myRand_scaled;
		}
};

struct data_domino {
	int right, left;
	bool available;
};

class CDominoes{
	public:
		CDominoes() {}  // constructor
		~CDominoes() {} // destructor

		void init() {
			pieceIDCounter = 0;
			set_availability_onCreate();
			initShuffleIndex();
		}

		data_domino getPiece() {
			int index = randomIndex[pieceIDCounter];
			pieceIDCounter++;
			data_domino mypiece = dominoPile[index];
			//cout << "[" << mypiece.left << "|" << mypiece.right << "]" << endl;
			mypiece.available = false;
			dominoPile[index] = mypiece;

			return(mypiece);
		}

		void printDominoPile() {
			data_domino mypiece;
			int counter = 0;
			for (int right = 0; right < 7; right++) {
				for (int left = right; left < 7; left++) {
					mypiece = dominoPile[counter];
					counter++;
					cout << "[" << mypiece.left << "|" << mypiece.right << "] Status(" << mypiece.available << ")  ";
				}
				cout << endl;
			}

			// Count available pieces and total pieces
			int totalPieces = sizeof(dominoPile) / sizeof(data_domino);
			counter = 0;
			for (int i = 0; i < totalPieces; i++) {
				if (dominoPile[i].available)
					counter++;
			}
			cout << "dominoPile stores " << totalPieces << " pieces. " << counter << " pieces are available." << endl;
		}

	private:
		data_domino dominoPile[28];
		int randomIndex[28];
		CRandom crandom;
		int pieceIDCounter;

		void set_availability_onCreate() {
			data_domino mypiece;
			int counter = 0;
			for (int right = 0; right < 7; right++) {

				for (int left = right; left < 7; left++) {
					mypiece.right = right;
					mypiece.left = left;
					
					mypiece.available = true;

					cout << "[" << mypiece.left << "|" << mypiece.right << "] Status(" << mypiece.available << ")  ";
					dominoPile[counter] = mypiece;
					counter++;
				}
				cout << endl;
			}

			// Count available pieces and total pieces
			int totalPieces = sizeof(dominoPile)/sizeof(data_domino);
			counter = 0;
			for (int i = 0; i < totalPieces; i++) {
				if (dominoPile[i].available)
					counter++;
			}
			cout << "dominoPile stores " << totalPieces << " pieces. " << counter << " pieces are available." << endl;
		}

		void initShuffleIndex() {
			for (int i = 0; i < 28; i++) {
				randomIndex[i] = i;
			}
			// shuffle the array of indexes
			crandom.shuffle(randomIndex, 28);
		}
};

class CPlayer {
	public:
		CPlayer() {}  // constructor
		~CPlayer() {} // destructor

		void init(int id, CDominoes* newCDominoes) {
			playerID = id;
			cdominoes = newCDominoes;
		}
		
		void getPieceFromPile(int numberOfPieces) {
			data_domino piece;
			for (int i = 0; i < numberOfPieces; i++) {
				piece = cdominoes->getPiece();
				playerDominoPieces.push_back(piece);
				cout << "Player " << playerID << " picked [" << piece.left << "|" << piece.right << "]" << endl;
			}
		}

		void displayPieces() {
			int numOfPieces = playerDominoPieces.size();
			cout << "Player " << playerID << " have " << numOfPieces << " pieces in hand :" << endl;
			for (int i = 0; i < numOfPieces; i++) {
				data_domino piece = playerDominoPieces.at(i);
				cout << "[" << piece.left << "|" << piece.right << "] ";
			}
			cout << endl;
		}

		deque<data_domino> getPiecesInHand() {
			return playerDominoPieces;
		}

		int getNumPiecesInHand() {
			return playerDominoPieces.size();
		}

	private:
		deque<data_domino> playerDominoPieces;
		CDominoes* cdominoes;
		//CTable* ctable;
		int playerID;

};

class CTable {
	public:
		CTable() {}  // constructor
		~CTable() {} // destructor

		void initGame() {
			cdominoes.init();
			player1.init(1, &cdominoes);
			player2.init(2, &cdominoes);

			player1.getPieceFromPile(10);
			player2.getPieceFromPile(10);
			player1.displayPieces();
			player2.displayPieces();
			cdominoes.printDominoPile();

			// startGame(){}
		}

	private:
		deque<data_domino> tablePieces;
		CPlayer player1;
		CPlayer player2;
		CDominoes cdominoes;

		void runGame() {
			bool win = false;
			int winnerID;
			CPlayer* currentPlayer;
			int playerTurnID;

			// starting player
			playerTurnID = 1;
			

			do{
				// check for winner
				if (player1.getNumPiecesInHand() == 0) {
					win = true;
					winnerID = 1;
				}
				else if (player2.getNumPiecesInHand() == 0) {
					win = true;
					winnerID = 2;
				}

				// for each player
				currentPlayer
				// check if can play
				// 
				// if can, play
				// 
				// if no, draw piece
				// draw piece
				// if cant draw piece, pass

				// switch player
				
			} while (!win);	
		}

		void placePiece(data_domino piece, bool head) { // head = true, tail  false
			string msg = "Valid move: piece placed.";
			bool fail = false;

			// check
			if (head) {
				if (piece.right == tablePieces.front().left)
					tablePieces.push_front(piece);
				else {
					msg = "Invalid move: cannot place at head.";
					fail = true;
				}

			}
			else {
				if (piece.left == tablePieces.back().right)
					tablePieces.push_back(piece);
				else {
					msg = "Invalid move: cannot place at tail.";
					fail = true;
				}
			}

			if (fail) {
			}

			cout << msg << endl;
		}
};


int main()
{	
	CTable game;
	game.initGame();
}