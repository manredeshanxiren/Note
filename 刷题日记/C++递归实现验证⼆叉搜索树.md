# C++递归实现验证⼆叉搜索树  

## 题目链接

> [98. 验证二叉搜索树 - 力扣（LeetCode）](https://leetcode.cn/problems/validate-binary-search-tree/submissions/478753962/)

## 题目描述

> 给你⼀个⼆叉树的根节点root，判断其是否是⼀个有效的⼆叉搜索树。  
>
> 有效⼆叉搜索树定义如下：  
>
> - 节点的左⼦树只包含⼩于当前节点的数。  
> - 节点的右⼦树只包含⼤于当前节点的数。  
> - 所有左⼦树和右⼦树⾃⾝必须也是⼆叉搜索树。  

## 解题思路

> **利用中序遍历；**
>
> 后序遍历按照左⼦树、根节点、右⼦树的顺序遍历⼆叉树的所有节点，通常⽤于⼆叉搜索树相关题⽬。
>
> **算法思路：**
>
> 如果⼀棵树是⼆叉搜索树，那么它的中序遍历的结果⼀定是⼀个严格递增的序列。  
>
> 因此，我们可以初始化⼀个**⽆穷⼩**的全区变量，⽤来记录中序遍历过程中的**前驱结点**。那么就可以在
> 中序遍历的过程中，先判断是否和前驱结点构成递增序列，然后修改前驱结点为当前结点，传⼊下⼀
> 层的递归中。    
>
> **算法流程：**
>
> 1. 初始化⼀个全局的变量**`prev`**，⽤来记录中序遍历过程中的前驱结点的**`val`**； 
>
> 2.  **中序遍历的递归函数中** ：
>
>    a.设置递归出⼝：**`root==nullptr`**的时候，返回**`true`**； 
>
>    b. 先递归判断左⼦树是否是⼆叉搜索树，⽤**`retleft`**标记；  
>
>    c.然后判断当前结点是否满⾜⼆叉搜索树的性质，⽤**`retcur`**标记：  
>
>    - 如果当前结点的**`val`**⼤于**`prev`**，说明满⾜条件，**`retcur`**改为**`true`**；  
>    - 如果当前结点的val⼩于等于**`prev`**，说明不满⾜条件，**`retcur`**改为**`false`**；  
>
>    d.最后递归判断右⼦树是否是⼆叉搜索树，⽤**`retright`**标记；
>
> 3. 只有当**`retleft、retcur和retright`**都是**`true`**的时候，才返回**`true`**。  

## **C++算法代码：**

> ```cpp
> class Solution
> {
> long prev = LONG_MIN;
> public:
> bool isValidBST(TreeNode* root)
> {
> if(root == nullptr) return true;
> bool left = isValidBST(root->left);
> // 剪枝
> if(left == false) return false;
> bool cur = false;
> if(root->val > prev)
> cur = true;
> // 剪枝
> if(cur == false) return false;
> prev = root->val;
> bool right = isValidBST(root->right);
> return left && right && cur;
> }
> };
> ```

