# C++二叉树的所有路径

## 题目链接

> [257. 二叉树的所有路径 - 力扣（LeetCode）](https://leetcode.cn/problems/binary-tree-paths/description/)

## 题目描述

> 给你一个二叉树的根节点 `root` ，按 **任意顺序** ，返回所有从根节点到叶子节点的路径。
>
> **叶子节点** 是指没有子节点的节点。

## 解题思路

> 使⽤深度优先遍历（DFS）求解。  
>
> 路径以字符串形式存储，从根节点开始遍历，每次遍历时将当前节点的值加⼊到路径中，如果该节点为叶⼦节点，将路径存储到结果中。否则，将"->"加⼊到路径中并递归遍历该节点的左右⼦树。  
>
> 定义⼀个结果数组，进⾏递归。递归具体实现⽅法如下：  
>
> 1.如果当前节点不为空，就将当前节点的值加⼊路径path中，否则直接返回；  
>
> 2.判断当前节点是否为叶⼦节点，如果是，则将当前路径加⼊到所有路径的存储数组paths中；  
>
> 3.否则，将当前节点值加上"->"作为路径的分隔符，继续递归遍历当前节点的左右⼦节点。  
>
> 4.返回结果数组。  
>
> - 特别地，我们可以只使⽤⼀个字符串存储每个状态的字符串，在递归回溯的过程中，需要将路径中的当前节点移除，以回到上⼀个节点。  
>
> 具体实现方法：
>
> 1.定义⼀个结果数组和⼀个路径数组。  
>
> 2.从根节点开始递归，递归函数的参数为当前节点、结果数组和路径数组  。
>
> ​	a.如果当前节点为空，返回。
>
> ​	b.将当前节点的值加⼊到路径数组中。
>
> ​	c.递归遍历当前节点的左⼦树。  
>
> ​	d.如果当前节点为叶⼦节点，将路径数组中的所有元素拼接成字符串，并将该字符串存储到结果数组中。
>
> ​	e.递归遍历当前节点的右⼦树。  
>
> ​	f.回溯，将路径数组中的最后⼀个元素移除，以返回到上⼀个节点。  
>
> 3.返回结果数组。

## 代码：

> ```cpp
> class Solution {
>     vector<string> vs;
> public:
>     vector<string> binaryTreePaths(TreeNode* root) 
>     {
>         string path;
>         dfs(root, path);
>         return vs;
>     }
> 
>     void dfs(TreeNode* root, string path)
>     {
> 
>         if(root == nullptr) return;
> 
>         //非叶子节点
>         string tmp = path + to_string(root->val) + "->";
>         dfs(root->left, tmp);
>         //叶子节点
>         if(root->left == nullptr && root->right == nullptr)
>         {
>             path.append(to_string(root->val));
>             vs.push_back(path);
>         }
>         dfs(root->right, tmp);
>     }
> };
> ```
>
> 

