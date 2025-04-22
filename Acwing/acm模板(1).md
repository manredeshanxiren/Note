 

# 数论



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





## 12 . 裴蜀定理

对于任意一对正整数a、b，都存在非零整数x和y，使得 a * x + b * y = GCD(a, b);

```C++
#include <iostream>


using namespace std;

//求出满足裴蜀定理的x和y
int exgcd(int a, int b, int &x, int &y) {
    if(!b) {
        x = 1;
        y = 0;
        return a;
    }
    int d = exgcd(b, a % b, y, x);
    y -= a / b * x;
    
    return d;
} 
int main() {
    
    int n;
    
    cin >> n;
    
    while(n--) {
        int x = 0;
        int y = 0;
        int a;
        int b;
        cin >> a >> b;
        exgcd(a, b, x, y);
        
        cout << x << " " << y << endl;
        
    }

    return 0;
}
```

## 13.线性同余方程

给定 $n$ 组数据 $a_i, b_i, m_i$，对于每组数求出一个 $x_i$，使其满足 $a_i \times x_i \equiv b_i \pmod{m_i}$，如果无解则输出 `impossible`。 

```C++
#include <iostream>


using namespace std;

typedef long long LL;

int exgcd(int a, int b, int &x, int &y) {
    if(!b) {
        x = 1;
        y = 0;
        return a;
    }
    int d = exgcd(b, a % b, y, x);
    y -= a / b * x;
    
    return d;
} 


int main() {
    
    int n;
    
    cin >> n;
    
    while(n--) {
        int x = 0;
        int y = 0;
        int a;
        int b;
        int m;
        cin >> a >> b >> m;
        LL d = exgcd(a, m, x, y);
        
        if(b % d) puts("impossible");
        else cout << (LL)x * (b / d) % m << endl;
    }
    

    return 0;
}
```

 ## 14. 中国剩余定理


定义 中国剩余定理是数论中的一个重要定理，用于求解一次同余方程组

设 $m_1, m_2, \cdots, m_k$ 是两两互质的正整数，即对于任意的 $i \neq j$，都有 $\gcd(m_i, m_j) = 1$

考虑如下形式的同余方程组：$ \begin{cases} x \equiv a_1 \pmod{m_1} \\ x \equiv a_2 \pmod{m_2} \\ \cdots \\ x \equiv a_k \pmod{m_k} \end{cases}  $

该定理表明，此同余方程组在模 $M = m_1m_2\cdots m_k$ 下有唯一解。 

求解步骤 

1.**计算 $M$**： 令 $M = \prod_{i = 1}^{k} m_i$，即 $M$ 为所有模数 $m_i$ 的乘积。 

2.**计算 $M_i$**： 对于每个 $i$，计算 $M_i=\frac{M}{m_i}$，也就是 $M$ 除以 $m_i$ 的商。

3.**计算 $M_i$ 的逆元 $t_i$**： 求解同余方程 $M_i t_i \equiv 1 \pmod{m_i}$，得到 $M_i$ 模 $m_i$ 下的逆元 $t_i$。这一步通常可以使用扩展欧几里得算法（Extended Euclidean Algorithm）来求解。 

4.**计算方程组的解 $x$**： 同余方程组的解 $x$ 可以通过公式 $x = \sum_{i = 1}^{k} a_i M_i t_i \pmod{M}$ 计算得出。

代码实现：

```C++
#include <iostream>

using namespace std;

typedef long long LL;

// 扩展欧几里得算法
LL exgcd(LL a, LL b, LL &x, LL &y) {
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }
    LL d = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}

int main() {
    int n;
    cin >> n;

    bool has_answer = true;

    LL a1, m1;
    cin >> a1 >> m1; // 读取第一组数据

    for (int i = 1; i < n; ++i) {
        LL a2, m2;
        cin >> a2 >> m2;

        LL k1, k2;
        LL d = exgcd(a1, a2, k1, k2);
        if ((m2 - m1) % d) {
            has_answer = false;
            break;
        }

        k1 *= (m2 - m1) / d;
        LL t = a2 / d;
        k1 = (k1 % t + t) % t;

        m1 = a1 * k1 + m1; 
        a1 = abs(a1 / d * a2);
    }

    if (has_answer) {
        cout << (m1 % a1 + a1) % a1 << endl;
    } else {
        cout << "-1" << endl;
    }

    return 0;
}
```

##  15.高斯消元

输入一个包含 $n$ 个方程 $n$ 个未知数的线性方程组。 方程组中的系数为实数。 求解这个方程组。 下图为一个包含 $m$ 个方程 $n$ 个未知数的线性方程组示例：

$  \begin{cases} a_{11}x_1 + a_{12}x_2 + \cdots + a_{1n}x_n = b_1 \\ a_{21}x_1 + a_{22}x_2 + \cdots + a_{2n}x_n = b_2 \\ \cdots\cdots\cdots\cdots\cdots\cdots\cdots\cdots\cdots\cdots \\ a_{m1}x_1 + a_{m2}x_2 + \cdots + a_{mn}x_n = b_m \end{cases}  $

```C++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cmath>

using namespace std;

const int N = 110;
const double eps = 1e-8;

int n;
double a[N][N];

int gauss()  // 高斯消元，答案存于a[i][n]中，0 <= i < n
{
    int c, r;
    for (c = 0, r = 0; c < n; c ++ )
    {
        int t = r;
        for (int i = r; i < n; i ++ )  // 找绝对值最大的行
            if (fabs(a[i][c]) > fabs(a[t][c]))
                t = i;

        if (fabs(a[t][c]) < eps) continue;

        for (int i = c; i <= n; i ++ ) swap(a[t][i], a[r][i]);  // 将绝对值最大的行换到最顶端
        for (int i = n; i >= c; i -- ) a[r][i] /= a[r][c];  // 将当前行的首位变成1
        for (int i = r + 1; i < n; i ++ )  // 用当前行将下面所有的列消成0
            if (fabs(a[i][c]) > eps)
                for (int j = n; j >= c; j -- )
                    a[i][j] -= a[r][j] * a[i][c];

        r ++ ;
    }

    if (r < n)
    {
        for (int i = r; i < n; i ++ )
            if (fabs(a[i][n]) > eps)
                return 2; // 无解
        return 1; // 有无穷多组解
    }

    for (int i = n - 1; i >= 0; i -- )
        for (int j = i + 1; j < n; j ++ )
            a[i][n] -= a[i][j] * a[j][n];

    return 0; // 有唯一解
}


int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; i ++ )
        for (int j = 0; j < n + 1; j ++ )
            scanf("%lf", &a[i][j]);

    int t = gauss();
    if (t == 2) puts("No solution");
    else if (t == 1) puts("Infinite group solutions");
    else
    {
        for (int i = 0; i < n; i ++ )
        {
            if (fabs(a[i][n]) < eps) a[i][n] = 0;  // 去掉输出 -0.00 的情况
            printf("%.2lf\n", a[i][n]);
        }
    }

    return 0;
}
```





## 16.高斯消元求解异或线性方程

输入一个包含 $n$ 个方程 $n$ 个未知数的异或线性方程组。 方程组中的系数和常数为 $0$ 或 $1$，每个未知数的取值也为 $0$ 或 $1$。 求解这个方程组。 异或线性方程组示例如下： 

``` M[1][1]x[1] ^ M[1][2]x[2] ^ ... ^ M[1][n]x[n] = B[1] ``` 

 ```M[2][1]x[1] ^ M[2][2]x[2] ^ ... ^ M[2][n]x[n] = B[2] ```

 ```... ```

 ```M[n][1]x[1] ^ M[n][2]x[2] ^ ... ^ M[n][n]x[n] = B[n] ``` 

其中 `^` 表示异或(XOR)，$M[i][j]$ 表示第 $i$ 个式子中 $x[j]$ 的系数，$B[i]$ 是第 $i$ 个方程右端的常数，取值均为 $0$ 或 $1$。 

```C++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 110;

int n;
int a[N][N];

int gauss() {
    int c, r;
    for (c = 0, r = 0; c < n; c++) {
        int t = r;
        for (int i = r; i < n; i++) {  // 找绝对值最大（这里是找首个 1 所在行，因为只有 0 和 1 ）
            if (a[i][c]) {
                t = i;
                break;
            }
        }
        if (!a[t][c]) continue;  // 该列全为 0 ，跳过
        for (int i = c; i <= n; i++) swap(a[t][i], a[r][i]);  // 将有 1 的行换到顶端

        for (int i = r + 1; i < n; i++) {
            if (a[i][c]) {
                for (int j = c; j <= n; j++) {
                    a[i][j] ^= a[r][j];
                }
            }
        }
        r++;
    }

    if (r < n) {
        for (int i = r; i < n; i++) {
            if (a[i][n]) {
                return 2;  // 无解
            }
        }
        return 1;  // 有无穷多组解
    }

    for (int i = n - 1; i >= 0; i--) {
        for (int j = i + 1; j < n; j++) {
            if (a[i][j]) {
                a[i][n] ^= a[j][n];
            }
        }
    }
    return 0;  // 有唯一解
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    int t = gauss();
    if (t == 2) puts("No solution");
    else if (t == 1) puts("Multiple sets of solutions");
    else {
        for (int i = 0; i < n; i++) {
            printf("%d\n", a[i][n]);
        }
    }
    return 0;
}
```

