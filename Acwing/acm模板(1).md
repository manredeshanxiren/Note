 

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

输入样例：

```
2
6
8
```

输出样例：

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

## 6.数组中所有数字乘积的约数之和

 *O*(*N*)

```C++
#include <iostream>
#include <unordered_map>
#include <cmath>

using namespace std;

typedef long long LL;

const int mod = 1e9 + 7;

int main() {
    
    int n;
    int x;
    cin >> n;
    unordered_map<int, int> primes;
    //分解质因数
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
    //求解约数之和
    for(auto p : primes)  {
        int m = p.first;
        int a = p.second;
        LL t = 1;
        while(a--) t = (t * m + 1) % mod;
        res = res * t % mod;
    }
    
    cout << res;
    return 0;
}
```

以上两个例子的公式：

$ N = \prod_{i = 1}^{k} p_{i}^{a_{i}} = p_{1}^{a_{1}} \cdot p_{2}^{a_{2}} \cdots p_{k}^{a_{k}}  $

约数个数：$  \prod_{i = 1}^{k} (a_{i} + 1) = (a_{1} + 1)(a_{2} + 1)\cdots(a_{k} + 1)  $

约数之和： $ \begin{align*} \prod_{i = 1}^{k} \sum_{j = 0}^{a_{i}} p_{i}^{j} &= \prod_{i = 1}^{k} (p_{i}^{0} + p_{i}^{1} + \cdots + p_{i}^{a_{i}})\\ &= (p_{1}^{0} + p_{1}^{1} + \cdots + p_{1}^{a_{1}})(p_{2}^{0} + p_{2}^{1} + \cdots + p_{2}^{a_{2}})\cdots(p_{k}^{0} + p_{k}^{1} + \cdots + p_{k}^{a_{k}}) \end{align*} $



## 7.最大公约数

辗转相除法（欧几里得算法）：

（a， b） = (b, a mod b);

```C++
int gcd(int a, int b) {
    
    return b ? gcd(b, a % b) : a;
}
```



## 8. 欧拉函数

> 给定 n 个正整数 ai，请你求出每个数的欧拉函数。
>
> 1～N 中与 N 互质的数的个数被称为欧拉函数，记为 $\phi(N)$。 若在算数基本定理中，$N = p_1^{a_1}p_2^{a_2}\cdots p_m^{a_m}$，则： $\phi(N) = N\times\frac{p_1 - 1}{p_1}\times\frac{p_2 - 1}{p_2}\times\cdots\times\frac{p_m - 1}{p_m}$ 

时间复杂度：**O(sqrt(N))**

```C++
int Phi(int a) {
    int res = a;
    for(int i = 2; i <= a / i; ++i) {
        if(a % i == 0) {
            
            res = res / i * (i - 1);
            while(a % i == 0) a /= i;
        }
    }
    if(a > 1) res = res / a * (a - 1);
    
    return res;
}
```

## 9. 筛法求欧拉函数

给定一个正整数 n，求 1∼n 中每个数的欧拉函数之和。

时间复杂度：**O(N)**

```c++
#include <iostream>


using namespace std;

typedef long long LL;

#define N 1000010

int primes[N], cnt;

int phi[N];

bool st[N];

LL getEuLes(int n) {
    phi[1] = 1;
    for(int i = 2; i <= n; ++i) {
        if(!st[i]) {
            primes[ cnt ++ ] = i;
            phi[i] = i - 1;
        }
        for(int j = 0; primes[j] <= n / i; ++j) {
            st[primes[j] * i] = true;
            if(i % primes[j] == 0) {
                phi[primes[j] * i] = phi[i] * primes[j];
                break;
            }
             phi[primes[j] * i] = phi[i] * (primes[j] - 1);
        }
    }
    LL res = 0;
    for(int i = 1; i <= n; ++i) res += phi[i];
    return res;
}
```





## 10 快速幂

应用：a的b次方mod p

O(logb)

```C++
#include <iostream>

using namespace std;

typedef long long LL; 

int qmi(int a, int b, int p) {
    int res = 1;
    
    while(b) {
        if(b & 1) res = (LL)res * a % p;
        b >>= 1;
        a =(LL)a * a % p;
    }
    
    return res;
}
```





## 11.快速幂求逆元

给定 *n* 组 *ai*,*pi*，其中 *pi* 是质数，求 *ai* 模 *pi* 的乘法逆元，若逆元不存在则输出 `impossible` 。

**注意**：请返回在 0∼*p*−1 之间的逆元。

```C++
#include <iostream>

using namespace std;

typedef long long LL; 

int qmi(int a, int b, int p) {
    int res = 1;
    
    while(b) {
        if(b & 1) res = (LL)res * a % p;
        b >>= 1;
        a =(LL)a * a % p;
    }
    
    return res;
}

int main() {
    int n;
    
    cin >> n;
    
    while(n--) {
        int a, b;
        
        cin >> a >> b;
        
        LL res = qmi(a, b - 2, b);
        
        if(a % b) cout << res << endl;
        else cout << "impossible" << endl;
        
    }
}
```

