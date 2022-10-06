#include <iostream>
#include <time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
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
			cout << "[" << mypiece.left << "|" << mypiece.right << "]"
				<< " available = " << mypiece.available << endl;
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

	private:
		deque<data_domino> playerDominoPieces[2];

};

class CTable {
	public:
		CTable() {}  // constructor
		~CTable() {} // destructor

	private:
		deque<data_domino> tablePieces;
};


int main()
{
	srand(time(NULL));
	CRandom* r = new CRandom();
	
	CDominoes* d = new CDominoes();
	d->init();

	d->getPiece();
	d->getPiece();
	d->getPiece();
	d->getPiece();

	d->printDominoPile();

}