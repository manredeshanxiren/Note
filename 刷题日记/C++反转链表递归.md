# C++反转链表递归、

# 题目描述

> [LCR 024. 反转链表 - 力扣（LeetCode）](https://leetcode.cn/problems/UHnkqh/submissions/477052551/)
>
> 给定单链表的头节点 `head` ，请反转链表，并返回反转后的链表的头节点。
>
> ![img](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/rev1ex1.jpg)

# 解题思路

> 这里我们采用递归的思路来解决首先我们分为两个视角来查看问题：
>
> ①宏观的视角
>
> 首先我们坚信`reverseList(ListNode* head)`这个函数可以帮我们逆置链表；
>
> 所以我们第一步`reverseList(head->next)`;
>
> `head->next->next = head`;
>
> `head->next = nullptr`;
>
> 最后我们返回链表的头指针即可
>
> ②将链表看成一棵树的视角
>
> 简单说就是第一个视角的展开，我们先递归到最深层：
>
> ![image-20231025113519813](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231025113519813.png)
>
> 逆置操作：
>
> ![image-20231025113546437](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231025113546437.png)
>
> 继续：
>
> ![image-20231025113605770](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231025113605770.png)
>
> 最后：
>
> ![image-20231025113621795](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231025113621795.png)
>
> 总结：
>
> 如果我们把单链表看成一棵树我们仅需进行深度优先遍历即可。

# 代码

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) 
    {
        if(head == nullptr)
        return head;

        if(head->next == nullptr)
        return head;

        ListNode* NewHead = reverseList(head->next);
        head->next->next = head;
        head->next = nullptr;

        return NewHead;

    }
};
```

# 复杂度分析

> **时间复杂度：**
>
> 只遍历了一次相当于O(N);
>
> **空间复杂度：**
>
> 没有额外使用O(1);