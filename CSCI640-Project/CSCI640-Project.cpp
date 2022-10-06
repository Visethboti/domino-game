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

		int getTotalAvailablePiece() {
			return (28 - pieceIDCounter + 1);
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
		
		bool getPieceFromPile(int numberOfPieces) {
			if (cdominoes->getTotalAvailablePiece() >= numberOfPieces) {
				data_domino piece;
				for (int i = 0; i < numberOfPieces; i++) {
					piece = cdominoes->getPiece();
					playerDominoPieces.push_back(piece);
					cout << "Player " << playerID << " picked [" << piece.left << "|" << piece.right << "]" << endl;
				}
				return true;
			}
			
			return false;
		}

		void displayPieces() {
			int numOfPieces = playerDominoPieces.size();
			cout << "Player " << playerID << " have " << numOfPieces << " pieces in hand :" << endl;
			for (int i = 0; i < numOfPieces; i++) {
				data_domino piece = playerDominoPieces.at(i);
				cout << i << "-[" << piece.left << "|" << piece.right << "] ";
			}
			cout << endl;
		}

		data_domino getPieceInHand(int pieceID) {
			return playerDominoPieces.at(pieceID);
		}

		void removePieceInHand(int pieceID) {
			playerDominoPieces.erase(playerDominoPieces.begin()+pieceID);
		}

		deque<data_domino> getPiecesInHand() {
			return playerDominoPieces;
		}

		int getNumPiecesInHand() {
			return playerDominoPieces.size();
		}

		int getID() {
			return playerID;
		}
		
		int getHeaviestPieceFromHand() {
			data_domino piece = playerDominoPieces.at(1);
			int highest = piece.right + piece.left;
			int resultIndex = 0;

			for (int i = 1; i < playerDominoPieces.size(); i++) {
				piece = playerDominoPieces.at(i);
				if (highest < (piece.right + piece.left)) {
					highest = piece.right + piece.left;
					resultIndex = i;
				}
			}
			return resultIndex;
		}

	private:
		deque<data_domino> playerDominoPieces;
		CDominoes* cdominoes;
		int playerID;

};

class CTable {
	public:
		CTable() {}  // constructor
		~CTable() {} // destructor

		void initGame() {
			cdominoes.init();
			player[0].init(1, &cdominoes);
			player[1].init(2, &cdominoes);

			player[0].getPieceFromPile(10);
			player[1].getPieceFromPile(10);
			player[0].displayPieces();
			player[1].displayPieces();
			cdominoes.printDominoPile();

			runGame();
		}

		void printTable() {
			data_domino piece;
			cout << "----------- Table -----------" << endl;
			for (int i = 0; i < tablePieces.size(); i++) {
				piece = tablePieces.at(i);
				cout << "[" << piece.left << "|" << piece.right << "] ";
			}
			cout << endl;
		}

	private:
		deque<data_domino> tablePieces;
		CPlayer player[2];
		CDominoes cdominoes;

		void runGame() {
			bool win = false, playerDone = false;
			int winnerID;
			int currentPlayerIndex;
			CPlayer* currentPlayer;

			data_domino piece;

			// starting player
			currentPlayerIndex = 0;
			currentPlayer = &player[currentPlayerIndex];
			
			// place first piece
			// find the heaviest piece from the first player to place down	
			int heaviestPieceIndex = currentPlayer->getHeaviestPieceFromHand();
			// place piece
			piece = player->getPieceInHand(heaviestPieceIndex);
			player->removePieceInHand(heaviestPieceIndex);
			placeFirstPiece(piece);

			do {
				// switch player
				if (currentPlayerIndex == 0)
					currentPlayerIndex = 1;
				else
					currentPlayerIndex = 0;
				currentPlayer = &player[currentPlayerIndex];

				do {
					// check for winner
					for (int i = 0; i < sizeof(player) / sizeof(CPlayer); i++) {
						if (player[i].getNumPiecesInHand() == 0) {
							win = true;
							winnerID = i;
							break; // exit of while loop
						}
					}

					// check if can play
					if (checkCanPlay(*currentPlayer)) { //// if can, play
						// display the table
						printTable();
						bool done = false;
						do {
							// display all pieces
							currentPlayer->displayPieces();
							// ask player which piece
							cout << "Player " << currentPlayer->getID() << " enter a number " << "(0-" << currentPlayer->getNumPiecesInHand() - 1 << ") to choose which piece to place: ";

							int choice;
							cin >> choice;
							piece = currentPlayer->getPieceInHand(choice);
							cout << "Enter head or tail (1 or 0) the piece:[" << piece.left << "|" << piece.right << "]: ";
							int head;
							cin >> head;

							// place piece
							if (placePiece(piece, head)) {
								currentPlayer->removePieceInHand(choice);
								done = true;
							}
							else { // fail
								// nothing, just do again
							}
						} while (!done);
						playerDone = true;
					}
					else { // if no, draw piece
						// draw piece
						if (currentPlayer->getPieceFromPile(1)) {
							playerDone = false;
						}
						else { // if cant draw piece, pass
							cout << "No piece available." << endl;
							playerDone = true;
						}
					}
				} while (!playerDone);
				playerDone = false;
			} while (!win);	
		} 

		bool checkCanPlay(CPlayer player) {
			deque<data_domino> playerDominoPieces = player.getPiecesInHand();
			int size = playerDominoPieces.size();
			for (int i = 0; i < size; i++) {
				data_domino piece = playerDominoPieces.at(i);
				if (piece.right == tablePieces.front().left)
					return true;
				if (piece.left == tablePieces.back().right)
					return true;
			}
			return false;
		}

		bool placePiece(data_domino piece, bool head) { // head = true, tail  false
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

			cout << msg << endl;
			return !fail;
		}

		void placeFirstPiece(data_domino piece) { // place first piece
			tablePieces.push_front(piece);
		}
};


int main()
{	
	CTable game;
	game.initGame();
}