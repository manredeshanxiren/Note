# 牛客热题：链表中的倒数最后K个节点

## 题目链接

[链表中倒数最后k个结点_牛客题霸_牛客网 (nowcoder.com)](https://www.nowcoder.com/practice/886370fe658f41b498d40fb34ae76ff9?tpId=295&tqId=1377477&ru=/exam/oj&qru=/ta/format-top101/question-ranking&sourceUrl=%2Fexam%2Foj)

## 方法一：两次遍历

### 思路

- 第一次遍历，获取链表的长度
- 第二次遍历，在题目给定的位置返回对应的链表指针

### 代码

```cpp
    ListNode* FindKthToTail(ListNode* pHead, int k) 
    {
        ListNode* cur = pHead;
        int res = 0;
        //第一次遍历获取对应的链表的长度
        while(cur != nullptr)
        {
            cur = cur->next;
            res++;
        } 
		//若是对应的长度小于k那么直接返回一个空链表
        if(k > res) return nullptr; 
        
        //否则的话去遍历寻找对应的倒数第k个节点
        cur = pHead;
        res = res - k;
        while(cur != nullptr)
        {
            
            if(res == 0) return cur;
            res--;
            cur = cur->next;
        }
        return nullptr;
    }
```

### 复杂度

> - 时间复杂度：O(N), 遍历了两次单链表。
> - 空间复杂度：O(1), 只使用了常数个变量。

## 方法二：双指针

### 思路

- 两个指针`l，r`
- r指针先优先l指针k步
- 同时向后移动l和r指针，当r指针指向链表尾部的时候则l指向的就是倒数第k个节点指针

### 代码

```cpp
    ListNode* FindKthToTail(ListNode* pHead, int k) 
    {
        ListNode* l = pHead;
        ListNode* r = pHead;
        //先将快指针移动k个位置
        for(int i = 0; i < k; ++i) 
        {
            if(r == nullptr) return nullptr;
            r = r->next;
        }

        while(r != nullptr)
        {
            l = l->next;
            r = r->next;
        }

        return l;
    }
```

### 复杂度

> - 时间复杂度：O(N)，遍历一次链表
> - 空间复杂度：O(1)，常数个变量