#include <bits/stdc++.h>
using namespace std;
using ll = signed long long;

#define all(x) (x).begin(), (x).end()

// 解决单个测试用例：给定二进制字符串 s，找出最大的 k
// 使得可以通过若干次长度为 k 的区间翻转把 s 全部变为 '1'
void solve() {
    int n;
    string s;
    cin >> n >> s;  // 读入字符串长度和内容

    // 从最大可能的 k（n）向下枚举
    for (int k = n; k > 0; --k) {
        // t[i] 保存当前位置在应用所有之前翻转操作后得到的 0/1 值
        // end[i] 保存在位置 i 需要“结束”一次翻转（差分技巧）
        vector<char> t(n), end(n + 1);

        // 初始化 t：直接把字符 '0'/'1' 转换为数值 0/1
        for (int i = 0; i < n; ++i) {
            t[i] = s[i] - '0';
        }

        int cnt = 0;  // 当前生效的翻转操作计数
        // 扫描字符串每一位，维护差分翻转
        for (int i = 0; i < n; ++i) {
            // 如果有翻转在 i 处到期，则减去它的影响
            cnt -= end[i];
            // 根据当前 cnt 的奇偶，决定 t[i] 要不要被翻转一次
            t[i] ^= (cnt & 1);

            // 如果此时 t[i] 仍为 0，说明需要在这里发起一次长度 k 的翻转
            if (t[i] == 0) {
                // 只有当 [i, i+k) 在范围内才允许翻转
                if (i + k <= n) {
                    // 在 i+k 处结束这次翻转的影响
                    ++end[i + k];
                    // 从当前位置开始，这次翻转立即生效
                    ++cnt;
                    // 手动把当前位置设为 1（等同于翻转操作的效果）
                    t[i] = 1;
                } else {
                    // 剩余不足 k 位，无法再翻，跳出
                    break;
                }
            }
        }

        // 如果经过上述所有操作后，t 中最小值为 1，则表示全部变成了 1
        if (*min_element(all(t)) == 1) {
            cout << k << '\n';
            return;  // 输出答案并结束本用例
        }
    }

    // 理论上不会到这里
    assert(false);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) solve();
    return 0;
}

