# 牛客热题：删除链表中的重复元素-II

## 题目链接

[删除有序链表中重复的元素-II_牛客题霸_牛客网 (nowcoder.com)](https://www.nowcoder.com/practice/71cef9f8b5564579bf7ed93fbe0b2024?tpId=295&tqId=663&ru=/exam/oj&qru=/ta/format-top101/question-ranking&sourceUrl=%2Fexam%2Foj)

## 方法一：哈希表+辅助数组

### 思路

- 将链表中的元素映射到哈希表，
- 遍历哈希表中只存在一个该值的映射，那么我们就把这个值放入链表

细节：

- 因为原链表是有序链表，为了保证答案是有序的所以我们使用`map`.

### 代码

```cpp
    ListNode* deleteDuplicates(ListNode* head) 
    {
        map<int, int> hash;
        ListNode* cur = head;

        while(cur != nullptr)
        {
            hash[cur->val]++;
            cur = cur->next;
        }
        
        ListNode* header = new ListNode(-1);
        header->next = head;
        ListNode* c = header;
        for(auto& h : hash)
        {
            if(h.second == 1)
            {
                c = c->next;
                c->val = h.first;
            }
        }
        c->next = nullptr;

        return header->next;
    }
```

### 复杂度

> 时间复杂度：O(N), 遍历了一遍链表，和一遍数组
>
> 空间复杂度：O(N), 使用了哈希表

## 方法二：双指针

### 思路

- 创建一个哨兵位节点

- 维护三个指针pre，cur，ne

  分别表示前一个节点，当前节点，和下一个不重复的节点

- 首先我们判断cur的val和ne的val是否相等：

  相等：则ne向后遍历，寻找不等于cur的节点，或者直到链表尾部。

  不相等：这三个指针都向后移动一个节点。

### 代码

```cpp
    ListNode* deleteDuplicates(ListNode* head) 
    {
        if(head == nullptr || head->next == nullptr) return head;
        
        auto header = new ListNode(-1);
        header->next = head;
        ListNode* cur = head;
        ListNode* pre = header;
        ListNode* ne = cur->next;

        while (ne != nullptr) 
        {
            if (cur->val == ne->val) 
            {
                //ne寻找不重复的节点
                while (ne != nullptr && ne->val == cur->val) ne = ne->next;
                //删除cur到ne之前的节点包括cur
                pre->next = ne;
                cur = ne;
                if (ne != nullptr) ne = ne->next;
            }
            else 
            {
                pre = cur;
                cur = ne;
                ne = ne->next;
            }
        }

        return header->next;
    }
```

### 复杂度

> 时间复杂度：O(N), 遍历了一次链表
>
> 空间复杂度：O(1), 使用了常数个变量

