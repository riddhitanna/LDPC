#include <iostream>
#include <fstream>
#include <time.h>
#include <cmath>
#include <cstdlib>

using namespace std;

// Initialization of n and k
int n, k;

void messageGenerator(int message[], int l) {
	for(int i = 0; i < l; i++) {
		message[i] = rand() % 2;
	}
}

void bec(int codeword[], double p, int n){
	for (int i = 0; i < n; i++) {
		double x = rand();
		if (x <= p * RAND_MAX) {
			codeword[i] = -1;
		}
	}
}

void bsc(int codeword[], double p, int n){
	for (int i = 0; i < (int)(n - sqrt(n)); i++) {
		double x = rand()*1.0 / 32767;
		if (x <= p) {
			if (codeword[i] == 0) {
				codeword[i] = 1;
			} else {
				codeword[i] = 0;
			}
		}
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

void dispArr(int c2[], int n) {
	for(int i = 0; i < n; i++) {
		cout << c2[i] << " ";
	}
	cout << endl;
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
	} else if(ones > zeroes){
		return 1;
	} else {
		return a[0];
	}
}

bool isEqual(int a[], int b[], int n) {
	for(int i = 0; i < n; i++) {
		if(a[i] != b[i]) {
			return false;
		}
	}
	return true;
}

bool isCorrupted(int c2[], int n) {
	for(int i = 0; i < n; i++) {
		if(c2[i] == -1) {
			return true;
		}
	}
	return false;
}

int main() {
	srand(time(0));
	cout << "Enter the value of k: " << endl;
	cin >> k;

	// Assign value to n
	n = k + 2 * sqrt(k) + 1;

	// Generate message.
	int message[k];
	
	// GENERATOR MATRIX
	int G[n][k];
	// Initializing G to all zeros
	for(int row = 0; row < n; row++) {
		for(int col = 0; col < k; col++) {
			G[row][col] = 0;
		}
	}

	// First k rows of Generator Matrix
	for(int row = 0; row < k; row++) {
		for(int col = 0; col < k; col++) {
			if(row == col) {
				G[row][col] = 1;
			}
		}
	}

	// Next sqrt(k) rows of Generator Matrix
	for(int row = k; row < k + sqrt(k); row++) {
		for(int col = (row - k) * sqrt(k); col < (row - k + 1)*sqrt(k); col++) {
			G[row][col] = 1;
		}
	}

	// Next sqrt(k) rows of Generator Matrix
	for(int row = k + sqrt(k); row< n - 1; row++) {
		for(int col = row - k - sqrt(k); col < k; col = col + sqrt(k)) {
			G[row][col] = 1;
		}
	}

	// Last row of Generator Matrxi set to all ones
	for(int col = 0; col < k; col++) {
		G[n-1][col] = 1;
	}
	
	cout << "G-Matrix: " << endl;
	for(int row = 0; row < n; row++) {
		for(int col = 0; col < k; col++) {
			cout << G[row][col] << " ";
		}
		cout << endl;
	}
	cout << endl;
	// END GENERATOR MATRIX

	// Creation of H Matrix
	cout << endl << endl << "H-Matrix: " << endl;
	int H[n - k][n] = {0};
	
	for(int row = 0; row < n - k; row++) {
		for(int col = 0; col < n; col++) {
			H[row][col] = 0;
		}
	}
	
	//first sqrt(k) rows
	int i = 0;
	for(int row = 0; row < (int)sqrt(k); row++) {
		for(int col = i; col <= i + (int)sqrt(k); col++) {
			H[row][col] = 1;
		}
		i = i + sqrt(n);
	}

	//next sqrt(k) + 1 rows
	for(int row = sqrt(k); row < n - k; row++) {
		for(int col = row - sqrt(k); col < n; col = col + sqrt(k) + 1) {
			H[row][col] = 1;
		}
	}
	
	for(int row = 0; row < n - k; row++) {
		for(int col = 0; col < n; col++) {
			cout << H[row][col] << " ";
		}
		cout << endl;
	}
	cout << endl;
	// End of Creation of H Matrix

	// Probability p
	double p = 0;
	// END

	// Check Node Array which stores indices	
	int checkNodeArray[n - k][(int)sqrt(n)];
	int counter1 = 0;
	int counter2 = 0;
	
	int ITERATION = 10;
	// To create an array of checknodes which stores the indices.
	for (int i = 0; i < n - k; i++) {
		counter2 = 0;
		for(int j = 0; j < n; j++) {
			if (H[i][j] == 1) {
				checkNodeArray[counter1][counter2] = j;
				counter2++;
			}
		}
		counter1++;
	}
	// END Check Node Array
	
	// Array to store numbers;
	int ITER;
	cout << "Enter the value of Iteration: " << endl;
	cin >> ITER;
	
	// Take the input of Iteration.
	cout << "Enter the value of iteration per probability:" << endl;
	cin >> ITERATION;
	
	double codes[ITER];
	int total = 0;
	cout << "THE DECODING STARTS:" << endl;
	for(int prob = 0; prob < ITER; prob++) {
		total = 0;
		p = p + 1/(double)ITER;
		cout << "Probability = " << p << endl;
		// DECODING FOR LOOP
		for(int i = 0; i < ITERATION; i++) {
			
			// Generate message
			messageGenerator(message, k);
			// Generate Codeword1
			int codeword1[n];
			for(int i = 0; i < n; i++) {
				codeword1[i] = 0;
			}
			for(int i = 0; i < n; i++) {
				for(int j = 0; j < k; j++) {
					codeword1[i] += message[j] * G[i][j];
				}
				codeword1[i] = codeword1[i] % 2;
			}
			// End Generate Codeword1
			
			// Convert Codeword2
			int temp[(int)sqrt(n)][(int)sqrt(n)];
			for(int i = 0; i < sqrt(n); i++) {
				for(int j = 0; j < sqrt(n); j++) {
					temp[i][j] = 0;
				}
			}
			
			int t = 0;
			for(int i = 0; i < sqrt(n) - 1; i++) {
				for(int j = 0; j < sqrt(n) - 1; j++) {
					temp[i][j] = codeword1[t];
					t++;
				}
			}
			
			for(int i = 0; i < sqrt(n) - 1; i++) {
				int sum = 0;
				for(int j = 0; j < sqrt(n) - 1; j++) {
					sum += temp[i][j];
				}
				temp[i][(int)sqrt(n) - 1] = sum % 2;	
			}
			
			for(int i = 0; i < sqrt(n); i++) {
				int sum = 0;
				for(int j = 0; j < sqrt(n); j++) {
					sum += temp[j][i];			
				}
				temp[(int)sqrt(n) - 1][i] = sum % 2;
			}
			
			int codeword2[n] = {0};
			t = 0;
			for(int i = 0; i < sqrt(n); i++) {
				for(int j = 0; j < sqrt(n); j++) {
					codeword2[t] = temp[i][j];
					t++;
				}
			}
			
			int codeword3[n];
			for(int i = 0; i < n; i++) {
				codeword3[i] = codeword2[i];
			}
			// End Convert Codeword2
			if(p > 0.5) {
				bsc(codeword2, 1 - p, n);
			} else {
				bsc(codeword2, p, n);	
			}
			
			// Creation of checkNode.
			int counter = 0;
			int checkNode[n - k];
			// Set the all nodes to 1.
			for (int i = 0; i < n - k; i++) {
				checkNode[i] = 0;
			}

			// Give values to the nodes.
			for (int i = 0; i < n - k; i++) {
				int XOR = 0;
				for (int j = 0; j < (int)sqrt(n); j++) {
					XOR += codeword2[checkNodeArray[i][j]];
				}
				checkNode[i] = XOR % 2;
			}

			// Start Decoding using tanner graph.
			int iteration = 0;
			do {
				// To check All the nodes
				for(int i = 0; i < n; i++) {
					
					int ProbArray[n - k];
					ProbArray[0] = codeword2[i];
					// Check the checkNodeArray.
					int counter = 1;
					for(int j = 0; j < n - k; j++) {
						for(int m = 0; m < (int)sqrt(n); m++) {
							if(checkNodeArray[j][m] == i) {
								// We get the index where ith checkNode is connected.
								int sum2 = 0;
								for(int z = 0; z < (int)sqrt(n); z++) {
									if(z != m) {
										sum2 += codeword2[checkNodeArray[j][z]];
									}
								}

								// Assign the values to check the maximum of that.
								ProbArray[counter] = sum2 % 2;
								counter++;
							}
						}
					}
					codeword2[i] = mostProbable(ProbArray, counter);			
				}
				for (int i = 0; i < n - k; i++) {
					int XOR = 0;
					for (int j = 0; j < (int)sqrt(n); j++) {
						XOR += codeword2[checkNodeArray[i][j]];
					}
					checkNode[i] = XOR % 2;
				}
				iteration++;
			} while (isNotZero(checkNode, n - k) && iteration < 50); // End all the iteration
			
			// Output through the tanner graph if it is decoded or not.
			if (!isNotZero(checkNode, n - k)) {
				total++;
			}
		}
		// END FOR LOOP which is for each Probability
		cout << total << endl;
		codes[prob] = (double)total / (double)ITERATION;
	}
	// END OF OUTER FOR LOOP
	
	// Store it in a file
	ofstream myfile;
    myfile.open ("NewCodesMatrix3.txt");
    for(int i = 0; i < ITER; i++) {
        myfile << codes[i] << " ";
    }
    myfile.close();
	// End
	
	return 0;
}
