#include <iostream>
#include <algorithm>
 
using namespace std;
 
typedef long long LL;
typedef unsigned long long ULL;
 
const int N = 200010; // 常量定义
 
long long a[N];
 
long long preSum[N];
long long sufSum[N];
 
int t;
 
void solve() {
    long long n, k;
 
    cin >> n >> k;
 
    preSum[0] = 0;
    sufSum[n+1] = 0;   
 
    for(int i = 1; i <= n; ++i) cin >> a[i];
 
    //计算前缀和
    for(int i = 1; i <= n; ++i) preSum[i] = preSum[i - 1] + a[i];
    
 
    //极端情况
    if(k >= preSum[n]) {
        cout << n << endl;
        return ;
    }
 
    int res = 0;
 
    LL l = k / 2 + (k % 2);
    LL r = k / 2;
    int i = 1;
    for (i = 1; preSum[i] < l && i <= n; ++i)
    {
        res++;
        a[i] = 0;
    }
    if (preSum[i] == l)
    {
        res++;
        a[i] = 0;
    }
    else
    {
        a[i] -= (l - preSum[i - 1]);
    }
    // 计算后缀和
    for (int i = n; i >= 1; --i)
        sufSum[i] = sufSum[i + 1] + a[i];
    int j = n;
    for (j = n; sufSum[j] < r; j--)
        res++;
    if (sufSum[j] == r)
        res++;
 
    cout << (res > n ? n : res) << endl;
}
 
int main() {
 
    cin >> t;
    while(t --) {
        solve();
    }
    return 0;
}