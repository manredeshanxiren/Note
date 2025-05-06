#include <iostream>
#include <algorithm>

using namespace std;

typedef long long LL;
typedef unsigned long long ULL;

const int N = 200010; // 常量定义

int t, n, k;

int a[N];

void solve() {
    int maxIdx = 0;
    int minIdx = 0;
    cin >> n >> k;

    int x = 0;

    for(int i = 0; i < n; ++i) {
        cin >> a[i];
        if(a[maxIdx] < a[i]) maxIdx = i;
        if(a[minIdx] > a[i]) minIdx = i;
        x ^= a[i];
    }

    a[maxIdx]--;

    sort(a, a + n);

    if(a[n - 1] - a[0] > k) {
        cout << "Jerry" << endl;
        return ;
    }

    if(x % 2) {
        //先手赢
        cout << "Tom" << endl;
    } else {
        //先手输
        cout << "Jerry" << endl;
    }
}

 int main() {

    cin >> t;

    while(t --) {
        solve();
    }

    return 0;
}