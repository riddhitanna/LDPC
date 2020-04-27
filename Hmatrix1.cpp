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

int errors(int codeword2[], int codeword3[], int n) {
	int counter = 0;
	for(int i = 0; i < n; i++) {
		if(codeword2[i] != codeword3[i]){
			counter++;	
		}
	}
	return counter;
}

int ones(int codeword[], int ll) {
	int counter = 0;
	for(int i = 0; i < ll; i++) {
		if(codeword[i] != 1) {
			counter++;
		}
	}
	return counter;
}

int minusones(int codeword[], int ll) {
	int counter = 0;
	for(int i = 0; i < ll; i++) {
		if(codeword[i] == -1) {
			counter++;
		}
	}
	return counter;
}

int main () {
	srand(time(0));
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

	// Codeword
	int codeword[col]; 
	for(int i = 0; i < col; i++) {
		codeword[i] = 0;
	}
	
	cout << "Which channel you want to select?(0 - BSC || 1 - BEC)" << endl;
	bool channel;
	cin >> channel;
	
	int bitflip[30];
	int iterationArray[30];
	
	if(!channel) {
			int lemda = 0;
			// Probability
			double prob = 0.444;
			
			cout << "Iteration: " << lemda << endl;
			for(int i = 0; i < col; i++) {
				codeword[i] = 0;
			}
			// Pass through BSC (Binary Symmetric Channel)
			bsc(codeword, prob, col);
	
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
				bitflip[lemda] = ones(codeword, col);
				iterationArray[lemda] = lemda;
				lemda++;
				iteration++;
			} while (iteration < 20); // End all the iteration
			
			// Output through the tanner graph if it is decoded or not.
	} else {
		int lemda = 0;
			double prob = 0.4444;
			
			for(int i = 0; i < col; i++) {
				codeword[i] = 0;
			}
			
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
				bitflip[lemda] = minusones(codeword, col);
				iterationArray[lemda] = lemda;
				lemda++;		
			} while(Corrupted(codeword, col) && iteration < 50);	
	}
	
	ofstream myfile;
    myfile.open("iterationArray.txt");
    for(int i = 0; i < 20; i++) {
        myfile << iterationArray[i] << " ";
    }
    myfile.close();
	
	myfile.open("bitflips.txt");
    for(int i = 0; i < 20; i++) {
		myfile << bitflip[i] << " ";
    }
    myfile.close();
	return 0;
}
