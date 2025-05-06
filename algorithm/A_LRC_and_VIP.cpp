#include <iostream>
#include <algorithm>

using namespace std;

using i64 = long long int; // 定义长整型别名

typedef long long LL;
typedef unsigned long long ULL;

const int N = 110; // 常量定义

int a[N];
int idx[N];

int t, n;

//朴素gcd
i64 gcd(i64 a, i64 b)
{
	return b ? gcd(b, a % b) : a;
}

void solve() {

    cin >> n;
    int maxIdx = 0;
    int minIdx = 0;
    for(int i = 0; i < n; ++i) {
        cin >> a[i];
        if(a[maxIdx] < a[i]) maxIdx = i;
        if(a[minIdx] > a[i]) minIdx = i;
    }
    int r = a[maxIdx];
    int l = a[minIdx];
    for(int i = 1; i < n - 1; ++i) {
        if(a[i] < a[maxIdx]) l = gcd(l, a[i]);
    }

    if(r != l) {
        cout << "YES" << endl;

        for(int i = 0; i < n; ++i) {
            if(a[i] == a[maxIdx]) {
                cout << 1 << " ";
            } else {
                cout << 2 << " ";
            }
        }
        puts("");
    } else {
        cout << "NO" << endl;
    }


}

int main() {

    cin >> t;
    while(t--) {
        solve();
    }
    return 0;
}