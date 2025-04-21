 

## 1.判断质数



```C++
//输入一个数返回是否是质数
bool is_prim(int num) {
    if(num == 1) {
        return false;
    }
    
    for(int i = 2; i <= num / i; ++i) {
        if(num % i == 0) {
            return false;
        }
    }
    return true;
}

```

 

## 2.分解质因数

```C++
void divide(int n) {
    for(int i = 2; i <= n / i; ++i) {
        if(n % i == 0) {
            int s = 0; 
            while(n % i == 0) {
                n /= i;
                s++;
            }
            printf("%d %d\n", i, s);
        }
    }
    if(n > 1) {
        printf("%d %d\n", n, 1);
    }
    puts("");
}
```

#### 输入样例：

```
2
6
8
```

#### 输出样例：

```
2 1
3 1

2 3
```



## 3.筛质数

给出n，找出1-n的质数的个数

(1)埃氏筛法

**O(N * LOG LOG N)**

```C++
#include <iostream> 

using namespace std;

#define N 1000010

int primes[N];
bool st[N];

int cnt = 0;

void cnt_prim(int n) {
    
    for(int i = 2; i <= n; ++i) {
        if(!st[i]){
            primes[cnt++] = n;
            for(int j = i + i; j <= n; j += i) st[j] = true;
        }
    }
}


```

（2）线性筛法

**O(N)**

```C++
#include <iostream> 

using namespace std;

#define N 1000010

int primes[N];
bool st[N];

int cnt = 0;

void cnt_prim(int n) {
    for(int i = 2; i <= n; ++i) {
        if(!st[i]) primes[cnt++] = i;
        for(int j = 0; primes[j] <= n / i; ++j) {
            st[primes[j] * i] = true;
            if(i % primes[j] == 0) break;
        }
    }
}
```



## 4.试除法找约数

**O(sqrt(n))**

```C++
#include <algorithm>
#include <vector>

using namespace std;

vector<int> get_divisors(int n) {
    vector<int> res;
    for(int i = 1; i <= n / i; ++i) {
        if(n % i == 0) {
            res.push_back(i);
            if(i != n / i) res.push_back(n / i);
        }
    }
    
    sort(res.begin(), res.end());
    return res;
}
```



## 5.数组所有元素乘积的约数个数



![image-20250420223149412](https://gitee.com/Green-Forgetting-the-River/typora-chart-bed/raw/master/2024_3/202504202231698.png)

 ***O*(n*m)，其中 `m` 是输入整数中的最大值。**

**空间复杂度为 *O*(log*m*)。**

```c++
#include <iostream>
#include <unordered_map>

using namespace std;

typedef long long LL;

const int mod = 1e9 + 7;

int main() {
    
    int n;
    int x;
    cin >> n;
    unordered_map<int, int> primes;
    
    while(n--) {
        cin >> x;
        for(int i = 2; i <= x / i; ++i) {
            while(x % i == 0) {
                primes[i]++;
                x /= i;
            }
        }
        if(x > 1) primes[x]++;
    }
    
    LL res = 1;
    
    for(auto p : primes) res = res * (p.second + 1) % mod;
    
    cout << res;
    return 0;
}
```

