#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>

using namespace std;

typedef long long LL;
typedef unsigned long long ULL;

const int N = 510;


int t;

void Solve()
{
    int n, c, d;
    cin >> n >> c >> d;
    unordered_map<int, int> hash;
    hash.reserve(n * n);              // 避免反复 rehash
    hash.max_load_factor(0.5f);       // 控制装载因子，减少冲突
    //这里不提前扩容的话会超时，原因是过程中会进行扩容，以及rehash
    int m = 1e9;
    for(int i = 0; i < n; ++i) 
        for(int j = 0; j < n; ++j) {
            int x;
            cin >> x;
            m = min(m, x);
            hash[x]++;
        }
    
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j) {
            if(hash[m + (i * c + j * d)] > 0) {
                hash[m + (i * c + j * d)]--;
            } else {
                cout << "No" << endl;
                return;
            }   
        }

    cout << "Yes" << endl;
}

int main() {

    cin >> t;

    while(t--) {
        Solve();
    }

    return 0;
}