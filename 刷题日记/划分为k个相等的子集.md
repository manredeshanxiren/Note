# 划分为k个相等的子集

## 题目链接

[698. 划分为k个相等的子集 - 力扣（LeetCode）](https://leetcode.cn/problems/partition-to-k-equal-sum-subsets/solutions/1833777/hua-fen-wei-kge-xiang-deng-de-zi-ji-by-l-v66o/)

## 方法一：状态压缩+记忆化搜索

### 思路

`状态压缩`：s的二进制位来表示每个位置(对应到数组种的index)是否被选择，1为未选择，反之为被选择。

`记忆化搜索`: dp数组表示每种选择是否被dfs过了的状态，true为未被dfs过

这里细说下面的代码的dfs的思路：

**两个参数：**

`s`：当前选择的状态，二进制位表示--->也就是所谓的状态压缩

`p`: 表示当前dfs的子集的和

**直接返回的情况：**

①s == 0：意味着数组中所有的元素已经被选择，那么也就证明了划分为k个子集成功了

②dp[s] == false: 这里进行判断以为这这个选择情况已经被dfs过了，已经被dfs过但是却没有成功划分，那么证明这个状态已经是失败的所以直接返回false；

**for循环：**

- 首先因为已经排序，所以nums[i] + p > per的情况就没必要dfs之后元素了

- (s >> i) & 1查看当前元素是否已经被选择， 若是没有被选择：

  （1）标记为选择:s ^ (1 << i)

  （2）将nums[i]累加到p

  （3）继续dfs

- 若是dfs已经返回了一个true那么已经找到一种情况满足了划分要求，因为这里只需要返回结果，所以没必要继续dfs了直接返回true即可，否则的话继续找数组中的下一个元素进行dfs

### 代码

```c++
class Solution {
public:
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int sum = accumulate(nums.begin(), nums.end(), 0);
        //能否被划分
        if(sum % k) {
            return false;
        }
        int per = sum / k;
        //排序
        sort(nums.begin(), nums.end());
        //如果待划分数组中的最大的数字已经大于子集和直接返回
        if(nums.back() > per) {
            return false;
        }
        int n = nums.size();
        //存储状态的dp数组
        vector<bool> dp(1 << n, true);

        function<bool(int, int)> dfs = [&](int s, int p) {
            if(s == 0) {
                return true;
            }

            if(!dp[s]) {
                return false;
            }
			//表示当前的选择状态已经被dfs
            dp[s] = false;

            for(int i = 0; i < n; ++i) {
                if(nums[i] + p > per) {
                    break;
                }
                if((s >> i) & 1) {
                    if(dfs(s ^ (1 << i), (nums[i] + p) % per)) {
                        return true;
                    }
                }
            }
            return false;
        };
        return dfs((1 << n) - 1, 0);
    }
};
```

### 复杂度分析

> **时间复杂度**：O(n * $2 ^ 2$) , accumulate --> O(n), sort --> O(nlogn), 这里dfs函数一共有$2^n$中状态因为利用了记忆化搜索，所以每种状态一定只会dfs一次，每个状态的dfs最坏情况下都会遍历一遍数组，所以整体的时间复杂度为O(n * $2 ^ 2$)
>
> **空间复杂度**：O($2 ^ n$)，记忆化搜索需要的dp数组的空间为$2 ^ n$， 其中n为nums数组的长度 

