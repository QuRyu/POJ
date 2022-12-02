#include <iostream>
#include <string>
#include <queue>

/*
 * Each piece can be in one of the two states, black or white. In total there
 * are (1 << 16) + 1 possible states on the board. To find the minimum number 
 * of rounds needed to flip the stones into either all black or white given 
 * an initial state, we can try all possible flips on each piece and do a BFS
 * to find the answer. The algorithm terminates when the goal is achieved or 
 * when we can't reach a new state. 
 *
 * Since each piece has only two states and there are 16 of them, we can use
 * bits in an integer to represent them. The FLIP array below tabulates the 
 * effect of flip on each piece. 
 */

using namespace std;

const int MAX_N = (1 << 16) + 1;
bool visited[MAX_N] = {false};

const int FLIP[16] = { 51200, 58368, 29184, 12544,
                       35968, 20032, 10016, 4880,
                       2248, 1252, 626, 305,
                       140, 78, 39, 19
                    };



int main() {
    string s; 
    int board = 0;

    for (int i=0; i<4; i++) {
        cin >> s;

        for (int j=0; j<4; j++) {
            if (s[j] == 'b') 
                board |= 1 << (4*i + j);
            else // s[j] == 'w
                continue; 
        }
    }

    if (board == 0 or board == 0xFFFF) {
        cout << 0 << endl;
        return 0; 
    }

    queue<int> q;
    q.push(board);
    int cur_step = 0;
    bool more_states = true;
    bool solvable = false;

    while (more_states and not solvable) {
        more_states = false;
        cur_step++;

        int n = q.size();
        for (int i=0; i<n; i++) {
            int state = q.front(); q.pop();

            for (int j=0; j<16; j++) {
                int new_state = state ^ FLIP[j];

                if (not visited[new_state]) {
                    more_states = true;
                    visited[new_state] = true;
                    q.push(new_state);

                    if (new_state == 0 or new_state == 0xFFFF) {
                        solvable = true; 
                        break;
                    }
                    
                }
            }
        }
    }

    if (solvable) cout << cur_step << endl;
    else cout << "Impossible" << endl;


    return 0;
}
