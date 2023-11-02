# C++求根节点到叶子节点数字之和

# 题目链接
>[LCR 049. 求根节点到叶节点数字之和 - 力扣（LeetCode）](https://leetcode.cn/problems/3Etpl5/description/)
# 题目描述

> 给定一个二叉树的根节点 `root` ，树中每个节点都存放有一个 `0` 到 `9` 之间的数字。
>
> 每条从根节点到叶节点的路径都代表一个数字：
>
> - 例如，从根节点到叶节点的路径 `1 -> 2 -> 3` 表示数字 `123` 。
>
> 计算从根节点到叶节点生成的 **所有数字之和** 。
>
> **叶节点** 是指没有子节点的节点。

# 解题思路

> 其实对于这种二叉树类的题目，并且又提到`根节点--->叶节点`，我们应该很容易想到`dfs`.
>
> 所以我们尝试用dfs来解答这道题目
>
> ①截止条件
>
> 戒指条件就是当我们遇到叶子节点的时候我们只需要返回`之前路径的值 * 10 + 当前节点的值`；
>
> ②中间过程
>
> 我们坚信`dfs(TreeNode* root, int presum)`这个函数可以将root中的值算出来；
>
> 所以对于一个中间节点，我们只需要：
>
> ```cpp   
>         int ret = 0;
>         if(root->left)
>         ret += dfs(root->left, presum);
>         if(root->right)
>         ret += dfs(root->right, presum);
>         return ret;
> ```
>
> 至此我们解题思路就到此为止

# 代码

```cpp
class Solution {
public:
    int sumNumbers(TreeNode* root) 
    {
        return dfs(root, 0);
    }

    int dfs(TreeNode* root, int presum)
    {
        presum = presum * 10 + root->val;
        if(root->left == nullptr && root->right == nullptr)
        {
            return presum;
        }

        int ret = 0;
        if(root->left)
        ret += dfs(root->left, presum);
        if(root->right)
        ret += dfs(root->right, presum);

        return ret;
    }
};
```

# 复杂度分析

> **时间复杂度：**
>
> 相当于深度优先遍历了二叉树,所以时间复杂度就是`O(N)`;
>
> **空间复杂度：**
>
> 额外使用了常数个变量所以空间复杂度是`O(1)`;