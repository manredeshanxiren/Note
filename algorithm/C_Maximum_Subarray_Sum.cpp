#include <iostream>
#include <algorithm>

using namespace std;


const int N = 200010; // 常量定义

long long n, k, t;

long long a[N]; // 数组定义

string s;

void solve() {
    cin >> n >> k;
    cin >> s;
    int pos = -1;
    for(int i = 0; i < n; ++i) {
        cin >> a[i];
        if(s[i] == '0') {
            pos = i;
            a[i] = -1e13;
        }
    }

    //计算当前最大区间和
    long long cur = 0;
    long long mx = 0;
    for(int i = 0; i < n; ++i) {
        cur = max(cur + a[i], a[i]);
        mx = max(mx, cur);
    }

    //无解情况
    if(mx > k || (mx != k && pos == -1)) {
        cout << "No" << endl;
        return;
    }

    if(pos != -1) {
        // 计算前缀后缀和-->从pos位置开始
        long long L = 0;
        cur = 0;
        for (int i = pos - 1; i >= 0; --i)
        {
            cur += a[i];
            L = max(L, cur);
        }

        long long R = 0;
        cur = 0;
        for (int i = pos + 1; i < n; ++i)
        {
            cur += a[i];
            R = max(R, cur);
        }

        a[pos] = k - L - R;
    }

    cout << "Yes" << endl;
    for(int i = 0; i < n; ++i) {
        printf("%lld ", a[i]);
    }
    puts("");
}

int main() {

    cin >> t;

    while(t--) {
        solve();
    }

    return 0;
}