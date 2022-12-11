#include <iostream>
#include <cmath>
#include <algorithm>
#include <float.h>

/**
 * Transform the problem from 2D into 1D by computing the boundary of each 
 * island where it can receive signals given the radius. Then sort the boundaries 
 * and do a greedy search over them. 
 * 
 * Note that sorting has to be ordered by right boundary and, if they are equal, by 
 * left boundary in reverse order. The reason that it cannot be ordered by left boundary 
 * is due to the following corner case. Suppose there are two boundaries, [l1, r1] and 
 * [l2, r2], where l1 < l2 < r2 < r1. If we order them by left, the radar will be installed 
 * at r1, and the second island is not covered. 
*/
using namespace std;

const int MAX_N = 1010;

struct Boundary {
    double left; 
    double right; 
} boundary[MAX_N];

bool cmp(const Boundary &lhs, const Boundary &rhs) {
    return lhs.right == rhs.right ? lhs.left > rhs.left : lhs.right < rhs.right;
}

int main() { 
    int n;
    int d;
    double x, y;
    int case_num = 1;

    while (true) {
        cin >> n >> d;

        if (n == 0) break;

        bool unreachable = false;
        for (int i=0; i<n; i++) {
            cin >> x >> y;

            if (y > d or y < 0) 
                unreachable = true;

            double x_delta = sqrt((double)d*d - y*y);
            boundary[i].left = x - x_delta;
            boundary[i].right = x + x_delta;
        }

        if (unreachable or d < 0) {
            cout << "Case " << (case_num++) << ": -1" << endl; 
            continue;
        }

        sort(boundary, boundary+n, cmp);
        

        int count = 1;
        double last_radar = boundary[0].right;
        for (int i=1; i<n; i++) {
            if (last_radar < boundary[i].left) {
                last_radar = boundary[i].right; 
                count++;
            }

        }

        cout << "Case " << (case_num++) << ": " << count << endl;
    }

    return 0;

}