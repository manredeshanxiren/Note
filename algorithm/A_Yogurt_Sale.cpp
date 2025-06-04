#include <iostream>
#include <algorithm>

using namespace std;

typedef long long LL;
typedef unsigned long long ULL;

int t;

void solve() {
    int n, a, b;

    cin >> n >> a >> b;

    if(b >= a * 2) {
        cout << n * a << endl;
        return;
    } else {
        cout << (n / 2) * b + (n % 2) * a << endl;
        return;
    }

}

int main() {

    cin >> t;
    
    while(t -- ) {
        solve();
    }


    return 0;
}