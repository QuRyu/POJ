#include <string>
#include <iostream>
#include <queue>
#include <algorithm>
#include <utility>

using namespace std;

const int MAX_N = (1 << 16) + 1; 

const int HandleChange[16] = {
    0x111F, 0x222F, 0x444F, 0x888F,
    0x11F1, 0x22F2, 0x44F4, 0x88F8, 
    0x1F11, 0x2F22, 0x4F44, 0x8F88, 
    0xF111, 0xF222, 0xF444, 0xF888
};

bool visited[MAX_N] = {0};
int history[MAX_N] = {-1};

int main() {
    int game = 0; 

    string line; 
    for (int i=0; i<4; i++) {
        cin >> line; 
        for (int j=0; j<4; j++) {
            if (line[j] == '-') 
                game |= 1 << (i*4 + j);
        }
    }

    queue<int> q;
    q.push(game);
    bool solved = false;

    while (not solved) {
        int n = q.size();

        for (int i=0; i<n; i++) {
            int state = q.front(); q.pop();

            for (int j=0; j<16; j++) {
                int new_state = state ^ HandleChange[j];

                if (not visited[new_state]) {
                    visited[new_state] = true;
                    history[new_state] = j;

                    if (new_state == 0xFFFF) {
                        solved = true;
                        break;
                    }

                    q.push(new_state);
                }
            }
        }
    }

    vector<int> ret; 
    int state = 0xFFFF;
    while (state != game) {
        ret.push_back(history[state]);
        state ^= HandleChange[history[state]];
    }

    cout << ret.size() << endl;
    for (int i=ret.size()-1; i>=0; i--) {
        cout << (ret[i] / 4 + 1) << " " << (ret[i] % 4 + 1) << endl;
    }

    return 0;
}