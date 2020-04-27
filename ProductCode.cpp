#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h>

using namespace std;

bool isNotZero(int a[], int n) {
	for(int i = 0; i < n; i++) {
		if(a[i] != 0) {
			return true;
		}
	}
	return false;
}

bool isEqual(int a[], int b[], int n) {
	for(int i = 0; i < n; i++) {
		if(a[i] != b[i]) {
			return false;
		}
	}
	return true;
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
	} else if (zeroes < ones){
		return 1;
	} else {
		return a[0];
	}
}

int xorMe(int a, int b) {
	if (a == 1) {
		if(b == 1) {
			return 0;
		} else {
			return 1;
		}
	} else {
		if(b == 0) {
			return 0;
		} else {
			return 1;
		}
	}
}

void makeCheckNode(int checkNode[], int c2[], int checkNodeArray[], int n, int k){
	int p = 0;
	for (int i = 0; i < n - k; i++) {
		int XOR = 0;
		for (int j = 0; j < (int)sqrt(n); j++) {
			XOR = xorMe(c2[checkNodeArray[p]], XOR);
			p++;
		}
		checkNode[i] = XOR;
	}
}

void bsc(int codeword[], double p, int n){
	srand(time(0));
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

void bec(int codeword[], double p, int n){
	srand(time(0));
	for (int i = 0; i < n; i++) {
		double x = rand()*1.0 / 32767;
		if (x <= p) {
			codeword[i] = -1;
		}
	}
}

bool isCorrectCode(int message[], int k) {
	int count = 0;
	for(int i = 0; i < k; i++) {
		if(message[i] == 0 || message[i] == 1) {
			count++;
		}
	}
	if(count == k) {
		return true;
	} else {
		return false;
	}
}

bool Corrupted(int c2[], int n) {
	for(int i = 0; i < n; i++) {
		if(c2[i] == -1) {
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

int main() {
    // k = number of message bits.
    // n = total number of bits.
    int k, n;
    cout << "Enter the value of k:" << endl;
    cin >> k;
    // Check whether the value of k is a perfect square or not.
	if(sqrt(k) - (int)sqrt(k) == 0) {
		n = (int)(2 * sqrt(k) + k + 1);
	    int message[k];
	    srand(time(0));
	    cout << "Randomly Generated Message:" << endl;
	    for(int i = 0; i < k; i++) {
	    	message[i] = rand() % 2;
	    	cout << message[i] << " ";
		}
		
		cout << endl;
		
	    if(isCorrectCode(message, k)) {
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
			
			int c[n] = {0};
			for(int i = 0; i < n; i++) {
				for(int j = 0; j < k; j++) {
					c[i] += message[j] * G[i][j];
				}
				c[i] = c[i] % 2;
			}
			
			cout << "CODEWORD: " << endl;
			for(int i = 0; i < n; i++) {
				cout << c[i] << " ";
			}
			
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
			
			//Start
			int codeword2[(int)sqrt(n)][(int)sqrt(n)] = {0};
			
			for(int i = 0; i < sqrt(n); i++) {
				for(int j = 0; j < sqrt(n); j++) {
					codeword2[i][j] = 0;
				}
			}
			
			int t = 0;
			for(int i = 0; i < sqrt(n) - 1; i++) {
				for(int j = 0; j < sqrt(n) - 1; j++) {
					codeword2[i][j] = c[t];
					t++;
				}
			}
			
			for(int i = 0; i < sqrt(n) - 1; i++) {
				int sum = 0;
				for(int j = 0; j < sqrt(n) - 1; j++) {
					sum += codeword2[i][j];
				}
				codeword2[i][(int)sqrt(n) - 1] = sum % 2;	
			}
			
			for(int i = 0; i < sqrt(n); i++) {
				int sum = 0;
				for(int j = 0; j < sqrt(n); j++) {
					sum += codeword2[j][i];			
				}
				codeword2[(int)sqrt(n) - 1][i] = sum % 2;
			}
			
			int c2[n] = {0};
			t = 0;
			for(int i = 0; i < sqrt(n); i++) {
				for(int j = 0; j < sqrt(n); j++) {
					c2[t] = codeword2[i][j];
					t++;	
				}
			}
			// End 
			
			cout << "SYNDROME: " << endl;
			int S[n-k] = {0};
			for(int i = 0; i < n - k; i++) {
				for (int j = 0; j < n; j++) {
					S[i] += c2[j] * H[i][j];
				}
				S[i] = S[i] % 2;
			}
			
			for(int i = 0; i < n - k; i++) {
				cout << S[i] << " ";
			}
			cout << endl;
			
			int close = 0;
			int myCodeword[n]; 
			int codingMethod;
			int codeword7[n];
			do {
				cout << "Which method of encoding you want to perform?(1 - BEC || 0 - BSC)" << endl;
				cin >> codingMethod;
				for (int i = 0; i < n; i++) {
					myCodeword[i] = c2[i];
					codeword7[i] = c2[i];
				}
				if (codingMethod == 1) {
					double p;
					cout << "Enter the probability of bit change." << endl;
					cin >> p;
					for (int i = 0; i < n; i++) {
						cout << myCodeword[i] << " ";
					}
					cout << endl;
					
					bec(myCodeword, p, n);
					for (int i = 0; i < n; i++) {
						cout << myCodeword[i] << " ";
					}	
				} else {
					double p;
					cout << "Enter the probability of bit change." << endl;
					cin >> p;
					bsc(myCodeword, p, n);
					cout << "Original : \t";
					for (int i = 0; i < n; i++) {
						cout << c2[i] << " ";
					}
					cout << endl << "Error : \t";
					for (int i = 0; i < n; i++) {
						cout << myCodeword[i] << " ";
					}
				}
				cout << endl << "Do you want to close channel?(1 - YES || 0 - NO)" << endl;	
				cin >> close;
			} while (close != 1);			
			int Save;
			cout << "Do you want to save this as your codeword?(1 - YES || 0 - NO)" << endl;
			cin >> Save;
			if (Save != 0) {
				for (int i = 0; i < n; i++) {
					c2[i] = myCodeword[i];
				}
			}

			cout << endl << "Your Send Codeword is : " << endl;
			for (int i = 0; i < n; i++) {
				cout << c2[i] << " ";
			}
			cout << endl;

			// CheckNodeArrays
			int checkNodeArray[n - k][(int)sqrt(n)];
			int counter1 = 0;
			int counter2 = 0;
			
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

			// Print the checkNodeArray
			for (int i = 0; i < n - k; i++) {
				for (int j = 0; j < (int)sqrt(n); j++) {
					cout << checkNodeArray[i][j] << " ";
				}
				cout << endl;
			}
			// End checkNodeArray

			// Decoding of the Binary Symmetric Channel and Binary Erasure Channel
			if (codingMethod == 0) {
				// Decoding of Binary Symmetric Channel
				int iteration = 0;
				do {
					// A loop which checks for all the variable nodes
					for(int i = 0; i < n; i++) {
						// To store whether one occur or zeros
						int mostProbableArray[n - k];
						mostProbableArray[0] = c2[i];

						// Check the checkNodeArray.
						int counter = 1;
						for(int j = 0; j < n - k; j++) {
							for(int m = 0; m < (int)sqrt(n); m++) {
								if(checkNodeArray[j][m] == i) {
									// We get the index where ith checkNode is connected.
									int sum2 = 0;
									for(int z = 0; z < (int)sqrt(n); z++) {
										if(z != m) {
											sum2 += c2[checkNodeArray[j][z]];
										}
									}

									// Assign the values to check the maximum of that.
									mostProbableArray[counter] = sum2 % 2;
									counter++;
								}
							}
						}

						// Assign new values to codeword
						c2[i] = mostProbable(mostProbableArray, counter);
					}
					// End Loop
					iteration++;
					dispArr(c2, n);
				} while (!isEqual(c2, codeword7, n) && iteration <= 50);

				if(isEqual(c2, codeword7, n)) {
					cout << "SUCSSESS" << endl;
				} else {
					cout << "FAIL" << endl;
				}
			} else {
				// Decoding of Binary Erasure Channel
				int iteration = 0;
				do {
					for (int i = 0; i < n - k; i++) {
						int counter = 0;
						int correpted = -1;
						for (int j = 0; j < (int)sqrt(n); j++) {
							if(c2[checkNodeArray[i][j]] == -1) {
								counter++;
								correpted = j;
							}
						}
						if(counter != 1) {
							continue;
						} else {
							int XOR = 0;
							for(int j = 0; j < (int)sqrt(n); j++) {
								if(j != correpted) {
									XOR = xorMe(c2[checkNodeArray[i][j]], XOR);
								}
							}
							c2[checkNodeArray[i][correpted]] = XOR;
						}
					}
					iteration++;
				} while(Corrupted(c2, n) && iteration <= 50);
				
				if(!Corrupted(c2,n)) {
					cout << "Codeword decoded Successfully." << endl;
					for(int i = 0; i < n; i++) {
						cout << c2[i] << " ";
					}
					cout << endl;
				} else {
					cout << "OOPS!!! SORRY CODEWORD IS NOT CORRECTED." << endl;
				}
			}
		} else {
			cout << "Please give a proper input as a message signal." << endl;
		}
	} else {
		cout << "Please enter a valid value of k : " << endl;
	}
    return 0;
} 
