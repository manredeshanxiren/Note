#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

typedef long long LL;
typedef unsigned long long ULL;



int check(map<int, int>& am, map<int, int>& bm)
{
    int count = 0;
    for(auto e : am)
    {
        if(bm.count(e.first))
        {
            count += min(e.second, bm[e.first]);
        }
    }
    return count;
}

void solve() {
    int n, m, k;

    long long res = 0;
    cin >> n >> m >> k;
    vector<int> a(n + 1, 0);
    vector<int> b(m + 1, 0);
    map<int, int> am;
    map<int, int> bm;
    for(int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }
    for(int i = 1; i <= m ; i++)
    {
        cin >> b[i];
        bm[b[i]]++;
    }
    //将前m个数放入哈希中
    for(int i = 1; i <= m; i++)
    {
        am[a[i]]++;
    }
    int count = check(am, bm);

    if(count >= k) res++;
    //cout << "count :" << count << endl;
    for(int i = m + 1 ; i <= n; i++)
    {
        am[a[i - m]]--;
        if(am[a[i - m] == 0]) am.erase(am[a[i - m]]);
        if(bm.count(a[i - m]))
        {
            if(bm[a[i - m]] > am[a[i - m]])
            {
                count--;
            }
        }

        am[a[i]]++;
        if(bm.count(a[i]))
        {
            if(bm[a[i]] >= am[a[i]])
            {
                count++;
            }
        }
        if(count >= k) res++;
        //cout << "count :" << count << endl;
    }

    cout << res << endl;

}

int main() {

    int t;
    
    cin >> t;

    while(t--) {
        solve();
    }
    return 0;
}