/*
 * @Author: LetMeFly
 * @Date: 2022-04-01 10:59:20
 * @LastEditors: LetMeFly
 * @LastEditTime: 2022-04-01 11:06:33
 */
#include <bits/stdc++.h>
using namespace std;
#define mem(a) memset(a, 0, sizeof(a))
#define dbg(x) cout << #x << " = " << x << endl
#define fi(i, l, r) for (int i = l; i < r; i++)
#define cd(a) scanf("%d", &a)
typedef long long ll;

/*
1 2 1 2 1 1 1 2 1 -1
1 2 -1
(se1 == se2) = 1
1 2 1 1 1 1 1 1 1 2 1 1 -1
2 1 -1 
(se1 == se2) = 1
1 2 -1
2 1 -1
(se1 == se2) = 1
1 2 -1
2 -1
(se1 == se2) = 0
*/

int main() {
    set<int> se1;
    set<int> se2;
    int t;
    int stateCode = 0;
    while (cin >> t) {
        if (t == -1) {
            if (stateCode == 0)
                stateCode++;
            else if (stateCode == 1) {
                stateCode = 0;
                dbg((se1 == se2));
                se1.clear();
                se2.clear();
            }
            continue;
        }
        else {
            if (stateCode == 0) {
                se1.insert(t);
            }
            else {
                se2.insert(t);
            }
        }
    }
    return 0;
}