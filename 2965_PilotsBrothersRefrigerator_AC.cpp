#include <string>
#include <iostream>
#include <queue>
#include <algorithm>
#include <utility>

using namespace std;

const int MAX_N = (1 << 16) + 1; 

bool board[4][4] = {false};
int flip_count[4][4] = {0};

int main() {
    int game = 0; 

    string line; 
    for (int i=0; i<4; i++) {
        cin >> line; 
        for (int j=0; j<4; j++) {
            if (line[j] == '-') 
                board[i][j] = true;
        }
    }

    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            if (not board[i][j]) {
                for (int k=0; k<4; k++) {
                    flip_count[i][k]++;
                    flip_count[k][j]++;
                }
                flip_count[i][j]--;
            }
        }
    }

    int count = 0;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            if (flip_count[i][j] % 2 != 0)
                count++;
        }
    }

    cout << count << endl;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            if (flip_count[i][j]%2 != 0)
                cout << (i+1) << " " << (j+1) << endl;
        }
    }

    return 0;
}