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

		void shuffle(int array[], int size)//shuffle 
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
			cout << "CDomino: getting piece from domino pile. Piece got: [" << mypiece.left << "|" << mypiece.right << "]" << endl;
			mypiece.available = false;
			dominoPile[index] = mypiece;

			return(mypiece);
		}

		void printDominoPile() {
			data_domino mypiece;
			int counter = 0;
			cout << "CDomino: printing domino pile:" << endl;
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
			cout << "CDomino: generating domino pieces." << endl;
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

		void initShuffleIndex() { //shuffle domino pieces index
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

		void init(int id, CDominoes* newCDominoes, bool turnOnAI) {
			playerID = id;
			cdominoes = newCDominoes;
			aiON = turnOnAI;
		}
		
		bool getPieceFromPile(int numberOfPieces) {
			cout << "CPlayer: Player " << playerID << " getting piece from domino pile." << endl;
			if (cdominoes->getTotalAvailablePiece() >= numberOfPieces) {
				data_domino piece;
				for (int i = 0; i < numberOfPieces; i++) {
					piece = cdominoes->getPiece();
					playerDominoPieces.push_back(piece);
					cout << "CPlayer: Player " << playerID << " got [" << piece.left << "|" << piece.right << "] added to hand." << endl;
				}
				return true;
			}
			
			return false;
		}

		void displayPieces() {
			int numOfPieces = playerDominoPieces.size();
			cout << "CPlayer: printing player hand: Player " << playerID << " have " << numOfPieces << " pieces in hand :" << endl;
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
			int resultIndex = 0;
			data_domino piece = playerDominoPieces.at(resultIndex);
			int highest = piece.right + piece.left;
			
			cout << "CPlayer: Player " << playerID << " get heaviest piece from hand. " << endl;

			for (int i = 1; i < playerDominoPieces.size(); i++) {
				piece = playerDominoPieces.at(i);
				if (highest < (piece.right + piece.left)) {
					highest = piece.right + piece.left;
					resultIndex = i;
				}
			}
			return resultIndex;
		}

		bool getAIStatus() {
			return aiON;
		}

	private:
		deque<data_domino> playerDominoPieces;
		CDominoes* cdominoes;
		int playerID;
		bool aiON;

};

class AIDominoPlayer {
public:
	AIDominoPlayer() {}  // constructor
	~AIDominoPlayer() {} // destructor

	int getChoice(deque<data_domino> currentTable, deque<data_domino> currentHand) {
		deque<int> result = getPossibleChoiceIndex(currentTable, currentHand);

		// select a choice by random
		int randomChoice = crandom.getRandomPublic(0, (result.size() - 1));

		return result[randomChoice];
	}

	int getChoiceHead() {
		return crandom.getRandomPublic(0, 1);
	}
private:
	CRandom crandom;

	deque<int> getPossibleChoiceIndex(deque<data_domino> currentTable, deque<data_domino> currentHand) {
		const int size = currentHand.size();
		deque<int> result;
		for (int i = 0; i < size; i++) {
			if (checkCanPlayPiece(currentTable, currentHand.at(i))) {
				result.push_back(i);
			}
		}
		return result;
	}

	bool checkCanPlayPiece(deque<data_domino> currentTable, data_domino piece) { // check if a piece is able to be play in the current table
		for (int i = 0; i < currentTable.size(); i++) {
			data_domino tablePiece = currentTable.front();
			if (piece.right == tablePiece.left || piece.left == tablePiece.left)
				return true;
			tablePiece = currentTable.back();
			if (piece.right == tablePiece.right || piece.left == tablePiece.right)
				return true;
		}

		return false;
	}
};

class CTable {
	public:
		CTable() {}  // constructor
		~CTable() {} // destructor

		void initGame() {
			cdominoes.init();
			player[0].init(1, &cdominoes, true);
			player[1].init(2, &cdominoes, true);

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
		CRandom crandom;

		AIDominoPlayer aiDominoPlayer;
		
		void runGame() {
			bool win = false, playerDone = false;
			int currentPlayerIndex;
			CPlayer* currentPlayer;
			const int numberOfPlayers = sizeof(player) / sizeof(CPlayer);

			data_domino piece;

			// random which player to go first
			currentPlayerIndex = crandom.getRandomPublic(0, (numberOfPlayers-1)); // random player index to be first
			currentPlayer = &player[currentPlayerIndex];
			cout << "CTable: random player: Player " << currentPlayer->getID() << " goes first." << endl;
			
			// place first piece
			// find the heaviest piece from the first player to place down	
			int heaviestPieceIndex = currentPlayer->getHeaviestPieceFromHand();
			// place piece
			piece = currentPlayer->getPieceInHand(heaviestPieceIndex); // get piece from player hand
			currentPlayer->removePieceInHand(heaviestPieceIndex); // remove piece from player hand
			placeFirstPiece(piece); // place piece to the table
			cout << "CTable: Player " << currentPlayer->getID() << " play the heaviest piece: [" << piece.left << "|" << piece.right << "] " << endl;

			do {
				// switch player
				if (currentPlayerIndex == 0) {
					currentPlayerIndex = 1;
				}
				else
					currentPlayerIndex = 0;
				currentPlayer = &player[currentPlayerIndex];
				cout << "CTable: next player turn. Current player is Player " << currentPlayer->getID() << endl;

				do {
					// check if can play
					if (checkCanPlay(*currentPlayer)) { //// if can, play
						// display the table
						printTable();
						bool done = false;
						do {
							int choice;
							int head;
							// if ai is on
							if (currentPlayer->getAIStatus()) {
								currentPlayer->displayPieces();
								cout << "CTable: Player " << currentPlayer->getID() << " enter a number " << "(0-" << currentPlayer->getNumPiecesInHand() - 1 << ") to choose which piece to place: ";
								choice = aiDominoPlayer.getChoice(tablePieces, currentPlayer->getPiecesInHand());
								cout << "AI picked " << choice << endl;
								piece = currentPlayer->getPieceInHand(choice);
								cout << "CTable: Enter 1 for head or 0 for tail (1/0) the piece:[" << piece.left << "|" << piece.right << "]: ";
								head = aiDominoPlayer.getChoiceHead();
								cout << "AI picked " << head << endl;
							}
							else { // if ai is off, ask user for input
								do {
									// display all pieces
									currentPlayer->displayPieces();
									// ask player which piece
									cout << "CTable: Player " << currentPlayer->getID() << " enter a number " << "(0-" << currentPlayer->getNumPiecesInHand() - 1 << ") to choose which piece to place: ";

									cin >> choice; // Ask for input choice of piece
									if (choice < 0 || choice > currentPlayer->getNumPiecesInHand() - 1) {
										cout << "CTable: Invalid choice. Input a valid piece number " << "(0-" << currentPlayer->getNumPiecesInHand() - 1 << ")." << endl;
									}
								} while (choice < 0 || choice > currentPlayer->getNumPiecesInHand() - 1);

								piece = currentPlayer->getPieceInHand(choice);
								
								do {
									cout << "CTable: Enter 1 for head or 0 for tail (1/0) the piece:[" << piece.left << "|" << piece.right << "]: ";
									cin >> head; // Ask for input choice of head or tail
									if (head != 0 && head != 1)
										cout << "CTable: Invalid choice. Input 1 for head or 0 for tail." << endl;
								} while (head != 0 && head != 1);
							}
							

							// place piece
							if (placePiece(piece, head)) { // can place
								cout << "CTable: Player " << currentPlayer->getID() << " play the piece:[" << piece.left << "|" << piece.right << "]" << endl;
								currentPlayer->removePieceInHand(choice); // remove piece from player hand.
								done = true;
							}
							else { // fail
								// nothing, ask player to enter a valid move
							}
						} while (!done);
						playerDone = true;
					}
					else { // if no, draw piece
						// draw piece
						if (currentPlayer->getPieceFromPile(1)) {
							cout << "CTable: Player " << currentPlayer->getID() << " could not play. Drawed a piece from domino pile." << endl;
							playerDone = false;
						}
						else { // if cant draw piece, pass
							cout << "CTable: No piece available." << endl;
							playerDone = true;
						}
					}
				} while (!playerDone);
				playerDone = false;

				// check for winner
				// check if any player can play
				win = true;
				for (int i = 0; i < numberOfPlayers; i++) {
					if (checkCanPlay(player[i])) {
						win = false; // stay in the loop
					}
				}
			} while (!win);	

			// calculate winner
			int playerRanking[2][numberOfPlayers]; // [0] is PlayerID, [1] is score
			for (int i = 0; i < numberOfPlayers; i++) {
				playerRanking[1][i] = player[i].getNumPiecesInHand();
				playerRanking[0][i] = player[i].getID();
			}
			// sort the ranking by player score (smallest to highest)
			for (int i = 0; i < numberOfPlayers-1; i++) {
				for (int j = i+1; j < numberOfPlayers; j++) {
					if (playerRanking[1][i] > playerRanking[1][j]) {
						// swape
						int tempScore, tempID;
						tempID = playerRanking[0][i];
						tempScore = playerRanking[1][i];
						playerRanking[0][i] = playerRanking[0][j];
						playerRanking[1][i] = playerRanking[1][j];
						playerRanking[0][j] = tempID;
						playerRanking[1][j] = tempScore;
					}
				}
			}

			// print each player hand
			cout << "===== Result ======" << endl;
			for (int i = 0; i < numberOfPlayers; i++) {
				cout << "CTable: Player " << i << " pieces left: " << endl;
				player[i].displayPieces();
			}

			// Print Winner 
			cout << "CTable: printing winner list" << endl;
			for (int i = 0; i < numberOfPlayers; i++) {
				cout << i+1 << ". Player " << playerRanking[0][i] << " . Pieces left = " << playerRanking[1][i] << endl;
			}
			cout << "CTable: End of the game. Exiting...";
		} 

		bool checkCanPlay(CPlayer player) {
			deque<data_domino> playerDominoPieces = player.getPiecesInHand();
			int size = playerDominoPieces.size();
			for (int i = 0; i < size; i++) {
				data_domino piece = playerDominoPieces.at(i);
				data_domino tablePiece = tablePieces.front();
				if (piece.right == tablePiece.left || piece.left == tablePiece.left)
					return true;
				tablePiece = tablePieces.back();
				if (piece.right == tablePiece.right || piece.left == tablePiece.right)
					return true;
			}
			return false;
		}

		

		bool placePiece(data_domino piece, bool head) { // head = true, tail  false
			string msg = "CTable: Valid move.";
			bool fail = false;
			data_domino tablePiece;
			// check
			if (head) { // if head
				tablePiece = tablePieces.front();
				if (piece.right == tablePiece.left){
					tablePieces.push_front(piece);
				}
				else if (piece.left == tablePiece.left) {
					int temp = piece.left;
					piece.left = piece.right;
					piece.right = temp;
					tablePieces.push_front(piece);
				}
				else {
					msg = "CTable: Invalid move: cannot place at head.";
					fail = true;
				}
			}
			else { // if tail
				tablePiece = tablePieces.back();
				if (piece.left == tablePiece.right) {
					tablePieces.push_back(piece);
				}
				else if (piece.right == tablePiece.right) {
					int temp = piece.left;
					piece.left = piece.right;
					piece.right = temp;
					tablePieces.push_back(piece);
				}
				else {
					msg = "CTable: Invalid move: cannot place at tail.";
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