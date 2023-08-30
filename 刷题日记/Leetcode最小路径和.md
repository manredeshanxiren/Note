# Leetcode最小路径和

# Ⅰ.题目解析

题目链接：[LCR 099. 最小路径和 - 力扣（LeetCode）](https://leetcode.cn/problems/0i0mDW/)

> 首先我们明确这道题目可以用动态规划的思路进行解题，那么我们解答动态规划主要分为以下五部
>
> ![image-20230827165356891](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230827165356891.png)

## Ⅰ.Ⅰ状态表示

> 首先状态表示的确定我们根据<font color = green>**经验+题目**</font>要求两点来进行判断
>
> 所谓经验就是我们一般会采用当前状态的前一个状态或者后一个状态来确定当前的状态。
>
> 那么这道题目因为机器人每次只能向下或者向右移动一步那么我们每次当前的状态就可以由左边的和上面的状态来确定
>
> 也就是我们用`dp[ i ] [ j ]`表示当前的状态，那么`dp[i - 1][j]` 和 `dp[i][j - 1]` 来共同决定当前的状态。

## Ⅰ.Ⅱ状态转移方程

> 题目要求最小的路径和那么我们就是选取前一个状态中较小的状态来确定：
> $$
> dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j]
> $$
> 

## Ⅰ.Ⅲ初始化

> 为了防止越界的情况所以`dp`数组会多开一行一列，因为我们寻找的是最小的路径，所以我们新添加的一行一列一定不能影响到我们的结果，但是我们不能将`dp[0][1]和dp[1][0]`初始化成无穷，这样的话我们得到的结果也会是无穷，所以我们将他们处理成`0`;
>
> <img src="C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230827171121275.png" alt="image-20230827171121275" style="zoom: 50%;" />

## Ⅰ.Ⅳ填表顺序

> 填表顺序的话我们采用从左到右从上到下的填表顺序。

## Ⅰ.Ⅴ返回值

> 返回值的确定，无论我们怎么计算那么最终的计算结果一定会存储在`dp[i][j]`中，所以我们只需要返回`dp`数组的最后一个元素即可。

# Ⅱ. 代码

> ```cpp
>     int minPathSum(vector<vector<int>>& grid) 
>     {
>         int r = grid.size(), c = grid[0].size();
>         vector<vector<int>> dp(r + 1, vector<int>(c + 1, 0x3f3f3f3f)); 
>         dp[0][1] = dp[1][0] = 0;
>         for(int i = 1; i < r + 1; ++i)
>             for(int j = 1; j < c + 1; ++j)
>                 dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i - 1][j - 1];
>         return dp[r][c];
>     }
> ```
>
> 

