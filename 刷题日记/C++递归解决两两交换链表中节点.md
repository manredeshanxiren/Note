# C++递归解决两两交换链表中节点

# 题目描述

> [24. 两两交换链表中的节点 - 力扣（LeetCode）](https://leetcode.cn/problems/swap-nodes-in-pairs/)

# 解题思路

> 这一题的思路和上一道题反转链表的思路十分类似：
>
> 首先我们编写递归终止的条件：
>
> ```cpp
>         if(head == nullptr || head->next == nullptr)
>         return head;
> //当我们遇到一个只有一个节点的指针我们不进行交换直接返回head即可，同样的空链表也是同样的情况
> ```
>
> 首先我们坚信`ListNode* swapPairs(ListNode* head) `函数可以帮我们解决掉`head->next->next`及之后节点的交换问题：
>
> 那么我们只需要交换前两个节点并返回`head->next`即可
>
> ```cpp
> ListNode* ret = swapPairs(head->next->next);
> 
>         ListNode* hn = head->next;
>         head->next->next = head;
>         head->next = ret;
> 
>         return hn;
> ```
>
> 以上就是整道题的思路了

# 代码

```cpp
lass Solution {
public:
    ListNode* swapPairs(ListNode* head) 
    {
        if(head == nullptr || head->next == nullptr)
        return head;

        ListNode* ret = swapPairs(head->next->next);

        ListNode* hn = head->next;
        head->next->next = head;
        head->next = ret;

        return hn;
    }
};
```

# 复杂度分析

> **时间复杂度**
>
> 相当于深度优先遍历链表：O(N);
>
> **空间复杂度**
>
> 没有额外使用空间O(1);