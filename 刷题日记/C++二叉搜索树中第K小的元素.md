# C++二叉搜索树中第K小的元素

## 题目链接

> [230. 二叉搜索树中第K小的元素 - 力扣（LeetCode）](https://leetcode.cn/problems/kth-smallest-element-in-a-bst/description/)

## 题目描述

> 给定一个二叉搜索树的根节点 `root` ，和一个整数 `k` ，请你设计一个算法查找其中第 `k` 个最小元素（从 1 开始计数）。

## 解题思路

> 我们采用中序遍历+计数器剪枝的思路
>
> **算法流程：**
>
> 1.定义⼀个全局的变量count，在主函数中初始化为k的值（不⽤全局也可以，当成参数传⼊递归过
> 程中）；  
>
> **递归函数的设计：****`void dfs(TreeNode* root)`**:
>
> - 返回值为第k个节点；
>
> **递归函数流程(中序遍历)：**
>
> 1. 递归出⼝：空节点和`count == 0`直接返回，说明没有找到； 
>
> 2. 去左⼦树上查找结果  `dfs(root->left);`
>
> 3. 判断是否找到了第k个节点：
>
>    ```cpp
>            count--;
>            if(count == 0) ret = root->val;
>    ```
>
> 4. 去右⼦树上查找结果  `dfs(root->right);`

## 代码：

> ```cpp
> class Solution {
>     int count = 0;
>     int ret = 0;
> public:
>     int kthSmallest(TreeNode* root, int k) 
>     {
>         count = k;
> 
>         dfs(root);
>         return ret; 
>     }
>     void dfs(TreeNode* root)
>     {
>         //截止条件
>         if(root == nullptr ||count == 0) return;
>         //中序遍历
>         dfs(root->left);
>         count--;
>         if(count == 0) ret = root->val;
>         dfs(root->right);
>     }
> };
> ```
>
> 