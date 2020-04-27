#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <cmath>
#include <cstdlib>

using namespace std;

void bsc(int codeword[], double p, int n){
	srand(time(0));
	double x;
	for(int i = 0; i < n; i++) {
		x = (double)rand() / (double)RAND_MAX;
		if(x <= p) {
			codeword[i] = 1;
		}
	}
}

void bec(int codeword[], double p, int n){
	srand(time(0));
	for (int i = 0; i < n; i++) {
		double x = rand()*1.0 / 32767;
		if (x <= p) {
			codeword[i] = -1;
		}
	}
}

int mostProbable(int a[], int n) {
	int ones = 0;
	int zeroes = 0;
	for(int i = 0; i < n; i++) {
		if(a[i] == 0) {
			zeroes++;
		} else {
			ones++;
		}
	}
	if(zeroes > ones) {
		return 0;
	} else {
		return 1;
	}
}

bool isNotZero(int a[], int n) {
	for(int i = 0; i < n; i++) {
		if(a[i] != 0) {
			return true;
		}
	}
	return false;
}

bool Corrupted(int c2[], int n) {
	for(int i = 0; i < n; i++) {
		if(c2[i] == -1) {
			return true;
		}
	}
	return false;
}

int main () {
  	ifstream myfile1("Hmatrix_1.txt");
  	
	// Calculate Dc and Dv
	int Dc = 0;
	int Dv = 0;
	
	int lengthOfCodeWord = 0;
	char c;
	string s;
	getline(myfile1,s);
	int i = 0;
	int col = 0;
	while(s[i] != '\0') {
		if(s[i] == '1') {
			Dv++;
		}
		if(s[i] == '0' || s[i] == '1') {
			col++;
		}
		i++;
	}
	myfile1.close();
	
	// To get the value of Dc
	int row = 0;
	ifstream myfile2("Hmatrix_1.txt");
	while(getline(myfile2, s)) {
		if(s[0] == '1') {
			Dc++;
		}
		row++;
	}
	myfile2.close();
	
	// Print the value of Dc
	cout << "Dv: " << Dv << endl;
	cout << "Dc: " << Dc << endl;
	cout << "Rows: " << row << endl;
	cout << "Cols: " << col << endl;
	
	// For Decoding
	int checkNodeArray[row][Dv] = {0};
	
	for(int i = 0; i < row; i++) {
		for(int j = 0; j < Dv; j++) {
			checkNodeArray[i][j] = 0;
		}
	}

	// To create an array of checknodes which stores the indices.
	int temp = 0;
	ifstream myfile3("Hmatrix_1.txt");
	int counter1 = 0;
	int counter2 = 0;
	i = 0;
	cout << "checkNodeArray :" << endl;
	while(getline(myfile3, s)){
		i = 0;
		while(s[i] != '\0') {
			if(s[i] == '1') {
				checkNodeArray[counter1][counter2] = i/2;
				counter2++;
			}
			i++;
			if(counter2 >= Dv) {
				counter2 = 0;
				counter1++;
				break;
			}
		}
		if(counter1 >= row) {
			break;
		}
	}
	myfile3.close();

	// To print the checkNodeArray.
	for(int i = 0; i < row; i++) {
		for(int j = 0; j < Dv; j++) {
			cout << checkNodeArray[i][j] << "\t";
		}
		cout << endl;
	}

	// Codeword
	int codeword[col]; 
	for(int i = 0; i < col; i++) {
		codeword[i] = 0;
	}
	
	cout << "Which channel you want to select?(0 - BSC || 1 - BEC)" << endl;
	bool channel;
	cin >> channel;
	if(!channel) {
		// Probability
		double prob = 0.01;
		
		// Pass through BSC (Binary Symmetric Channel)
		bsc(codeword, prob, col);
		
		// Codeword
		for(int i = 0; i < col; i++) {
			cout << codeword[i];
		}

		// Creation of checkNode.
		int counter = 0;
		int checkNode[row];
		
		// Set the all nodes to 1.
		for(int i = 0; i < row; i++) {
			checkNode[i] = 1;
		}

		// Give values to the nodes.
		for(int i = 0; i < row; i++) {
			int XOR = 0;
			for (int j = 0; j < Dv; j++) {
				XOR += codeword[checkNodeArray[i][j]];
			}
			checkNode[i] = XOR % 2;
		}

		// Print the value of node.
		cout << "Check Node values initially: " << endl;
		for (int  i = 0; i < row; i++) {
			cout << checkNode[i] << " ";
		}
		cout << endl;

		// Start Decoding using tanner graph.
		int iteration = 0;
		do {
			int array[col][Dc];
			for(int i = 0; i < col; i++) {
				array[i][0] = codeword[i];
			}
			
			// To check All the nodes
			for(int i = 0; i < col; i++) {
				// A particular checkNode
				int count = 1;
				for(int j = 0; j < row; j++) {
					for(int k = 0; k < Dv; k++) {
						if(checkNodeArray[j][k] == i) {
							// Just to calculate module sum 2 for a particular checkNode.
							int sum = 0;
							for(int m = 0; m < Dv; m++) {
								if(m != k) {
									sum += codeword[checkNodeArray[j][m]];
								}
							}
							array[i][count] = sum % 2;
							count++;
						}
					}
				}
				codeword[i] = mostProbable(array[i], count);
			}
			cout << endl;
			counter = 0;
			for (int i = 0; i < row; i++) {
				int XOR = 0;
				for (int j = 0; j < Dv; j++) {
					XOR += codeword[checkNodeArray[i][j]];
					counter++;
				}
				checkNode[i] = XOR % 2;
			}			
			iteration++;
		} while (isNotZero(checkNode, row) && iteration < 10); // End all the iteration
		
		// Output through the tanner graph if it is decoded or not.
		if (!isNotZero(checkNode, row)) {
			cout << "The codeword is decoded correctly." << endl;
			for(int i = 0; i < col; i++) {
				cout << codeword[i] << " ";
			}
			cout << endl;
		} else {
			cout << "Oops!! Codeword is not decoded successfully." << endl;
		}
	} else {
		double prob = 0.5;
		bec(codeword, prob, col);

		// Creation of checkNode.
		int counter = 0;
		int checkNode[row];
		
		// Set the all nodes to 1.
		for(int i = 0; i < row; i++) {
			checkNode[i] = 1;
		}

		// Give values to the nodes.
		for(int i = 0; i < row; i++) {
			int XOR = 0;
			for (int j = 0; j < Dv; j++) {
				XOR += codeword[checkNodeArray[i][j]];
			}
			checkNode[i] = XOR % 2;
		}

		// Print the value of node.
		cout << "Check Node values initially: " << endl;
		for (int  i = 0; i < row; i++) {
			cout << checkNode[i] << " ";
		}
		cout << endl;

		// Decoding
		int iteration = 0;
		do {
			for (int i = 0; i < row; i++) {
				int counter = 0;
				int correpted = -1;
				for (int j = 0; j < Dv; j++) {
					if(codeword[checkNodeArray[i][j]] == -1) {
						counter++;
						correpted = j;
					}
				}
				if(counter != 1) {
					continue;
				} else {
					int XOR = 0;
					for(int j = 0; j < Dv; j++) {
						if(j != correpted) {
							XOR += codeword[checkNodeArray[i][j]];
						}
					}
					codeword[checkNodeArray[i][correpted]] = XOR % 2;
				}
			}
			iteration++;
		} while(Corrupted(codeword, col) && iteration < 50);
		
		if(!Corrupted(codeword, col)) {
			cout << "Codeword decoded Successfully." << endl;
			for(int i = 0; i < col; i++) {
				cout << codeword[i] << " ";
			}
			cout << endl;
		} else {
			cout << "OOPS!!! SORRY CODEWORD IS NOT CORRECTED." << endl;
		}
	}
	return 0;
}
