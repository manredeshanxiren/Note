#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

typedef long long LL;
typedef unsigned long long ULL;

LL d(LL x) {
    LL res = 0;
    while(x) {
        res += x % 10;
        x /= 10;
    }
    if(res > 9) return d(res);
    return res;
}


void solve() {
    LL n;

    cin >> n;
    LL res = 0;

    vector<LL> cnt(10, 0);

    for(LL i = 1; i <= n; ++i) {
        cnt[d(i)] ++;
    }

    //先计算一下树根的总数

    LL total = 0;

    for(LL i = 1; i <= 9; ++i) {
        if(cnt[i] == 0) continue;
        for(LL j = 1; j <= 9; ++j) {
            if(cnt[j] == 0) continue;

            LL c = d(i * j);

            total += cnt[i] * cnt[j] * cnt[c];
        }
    }

    LL actual = 0;
    LL last = 0;
    //再计算实际的数量
    for (LL a = 1; a <= n; a = last + 1){
        LL b = n / a;
        last = n / b;  
        actual += b * (last - a + 1);
    }

    cout << total - actual;
}

int main() {

    solve();


    return 0;
}