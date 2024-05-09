# 牛客热题：删除链表中的重复元素-I

## 题目链接

[删除有序链表中重复的元素-I_牛客题霸_牛客网 (nowcoder.com)](https://www.nowcoder.com/practice/c087914fae584da886a0091e877f2c79?tpId=295&tqId=664&ru=/exam/oj&qru=/ta/format-top101/question-ranking&sourceUrl=%2Fexam%2Foj)

## 方法一：简单遍历

### 思路

- 简单遍历一遍链表
- 遍历的过程中，发现有重复的值，直接与该节点重复的值的两个节点中的后一个节点删除

### 代码

```cpp
ListNode* deleteDuplicates(ListNode* head) 
    {
        //空链表的情况
        if(head == nullptr || head->next == nullptr) return head;

        ListNode* cur = head;
        while(cur != nullptr && cur->next != nullptr)
        {
            if(cur->val == cur->next->val) 
            cur->next = cur->next->next;
            else
            cur = cur->next;
        }

        return head;
    }
```

### 复杂度

> 时间复杂度：O(N)，遍历了一遍链表
>
> 空间复杂度：O(1), 使用了常熟个变量