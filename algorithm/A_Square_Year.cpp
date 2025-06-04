#include <iostream>
#include <algorithm>
#include <math.h>
#include <string>

using namespace std;

typedef long long LL;
typedef unsigned long long ULL;


void solve() {
    string s;
    cin >> s;

    int n = stoi(s);

    int temp = sqrt(n);

    if(temp * temp == n) {
        cout << 0 << " " <<temp << endl;
    } else {
        cout << -1 << endl;
    }
}

int main() {

    int t = 0;
    cin >> t;

    while(t--) {
        solve();
    }


    return 0;
}