# C++二叉树剪枝

# 题目链接
>[LCR 047. 二叉树剪枝 - 力扣（LeetCode）](https://leetcode.cn/problems/pOCWxh/)
# 题目描述

> 给定一个二叉树 **根节点** `root` ，树的每个节点的值要么是 `0`，要么是 `1`。请剪除该二叉树中所有节点的值为 `0` 的子树。
>
> 节点 `node` 的子树为 `node` 本身，以及所有 `node` 的后代。

# 解题思路

> 首先我们分为三步
>
> ①函数头
>
> 首先我们应该想到我们去递归解答这道题目，函数的参数非常好确认就是`TreeNode* root`即可。
>
> 函数的返回值：根据题目的意思我们要将那些全零的子树全部在树中删除，那么我们最好是返回一个`TreeNode*`即可。
>
> ②函数体
>
> 我们要实现的肯定是一个深度优先遍历`dfs`,那么
>
> （1）`dfs(root->left)`;
>
> （2）`dfs(root->right)`;
>
>  (3) `处理当前root`
>
> ③截止条件
>
> 当我们深度历到`root == nullptr`为空的时候

# 代码

```cpp
class Solution {
public:

    TreeNode* pruneTree(TreeNode* root) 
    {
        if(root == nullptr)
        return nullptr;

        root->left =  pruneTree(root->left);
        root->right = pruneTree(root->right);

        if(root->left == nullptr && root->right == nullptr && root->val == 0)
            root = nullptr;

        return root;
    }
}
```

# 复杂度分析

> **时间复杂度：**
>
> `dfs`时间复杂度为O(N);
>
> **空间复杂度：**
>
> 未使用额外的空间，空间复杂度为：O(1);