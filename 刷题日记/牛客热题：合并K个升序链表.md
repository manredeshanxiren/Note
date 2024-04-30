# 牛客热题：合并K个升序链表

## 题目链接：

[合并k个已排序的链表_牛客题霸_牛客网 (nowcoder.com)](https://www.nowcoder.com/practice/65cfde9e5b9b4cf2b6bafa5f3ef33fa6?tpId=295&tqId=724&ru=/exam/oj&qru=/ta/format-top101/question-ranking&sourceUrl=%2Fexam%2Foj)

## 方法一：复用2个升序链表的方法

### 思路：

- 首先我们知道如何合并两个升序链表
- 那么我们先将k个的前两个合并，然后再将和这个合并的链表和下一个链表合并...直到所有的链表都被合并

### 代码：

```cpp
 ListNode* Merge(ListNode* pHead1, ListNode* pHead2)
    {
        //申请一个哨兵位
        ListNode* head = new ListNode(0);
 
        ListNode* cur = head;
        while(pHead1 != nullptr && pHead2 != nullptr)
        {
            if(pHead1->val <= pHead2->val)
            {
                cur->next = pHead1;
                cur = cur->next;
                pHead1 = pHead1->next;
            }
            else
            {
                cur->next = pHead2;
                cur = cur->next;
                pHead2 = pHead2->next;
            }
        }
 
        //p1未完的情况
        while(pHead1 != nullptr)
        {
            cur->next = pHead1;
            cur = cur->next;
            pHead1 = pHead1->next;
        }
 
        //p2未完的情况
        while(pHead2 != nullptr)
        {
            cur->next = pHead2;
            cur = cur->next;
            pHead2 = pHead2->next;
        }
 
        return head->next;
    }
    
    ListNode* mergeKLists(vector<ListNode*>& lists) 
    {
        if(lists.size() == 0) return nullptr;
        ListNode* ret = lists[0];

        for(int i = 1; i < lists.size(); ++i)
        {
            ret = Merge(ret, lists[i]);
        }  

        return ret;
    }
```

### 复杂度：

> **时间复杂度:O($N^2$),** 但其实一般达不到O($N^2$);
>
> - 对于第一个链表:我们遍历了k-1次
> - 对于第二个链表:我们遍历了k-2次
> - ....
> - 对于最后一个链表:我们遍历了1次
>
> 由于所有的链表的长度加起来为$n$，那么平均长度为$n / k$,
>
> 每个链表最多被遍历k - 1次，我们放缩为k次，那么需要最多$n*n$次运算.
>
> **空间复杂度：O(1)**, 使用了常数个额外的空间。

## 方法二：第一种方法的分治优化-->借鉴牛客题解

### 思路：

**具体做法：**

- step 1：从链表数组的首和尾开始，每次划分从中间开始划分，划分成两半，得到左边$n/2$个链表和右边$n/2$个链表。
- step 2：继续不断递归划分，直到每部分链表数为1.
- step 3：将划分好的相邻两部分链表，按照[两个有序链表合并](https://www.nowcoder.com/practice/a479a3f0c4554867b35356e0d57cf03d?tpId=295&sfm=html&channel=nowcoder)的方式合并，合并好的两部分继续往上合并，直到最终合并成一个链表。

![合并k个升序链表](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/%E5%90%88%E5%B9%B6k%E4%B8%AA%E5%8D%87%E5%BA%8F%E9%93%BE%E8%A1%A8.gif)

### 代码：

```cpp
     ListNode* Merge(ListNode* pHead1, ListNode* pHead2)
    {
        //申请一个哨兵位
        ListNode* head = new ListNode(0);
 
        ListNode* cur = head;
        while(pHead1 != nullptr && pHead2 != nullptr)
        {
            if(pHead1->val <= pHead2->val)
            {
                cur->next = pHead1;
                cur = cur->next;
                pHead1 = pHead1->next;
            }
            else
            {
                cur->next = pHead2;
                cur = cur->next;
                pHead2 = pHead2->next;
            }
        }
 
        //p1未完的情况
        while(pHead1 != nullptr)
        {
            cur->next = pHead1;
            cur = cur->next;
            pHead1 = pHead1->next;
        }
 
        //p2未完的情况
        while(pHead2 != nullptr)
        {
            cur->next = pHead2;
            cur = cur->next;
            pHead2 = pHead2->next;
        }
 
        return head->next;
    }
    
    ListNode* DivideMerge(vector<ListNode*> & lists, int l, int r)
    {
        //不存在区间
        if(l > r) return nullptr;
        //已到达最小的区间
        if(l == r) return lists[l];

        int mid = l + r >> 1;

        return Merge(DivideMerge(lists, l, mid), DivideMerge(lists, mid + 1, r));
    }

    ListNode* mergeKLists(vector<ListNode*>& lists) 
    {
        return DivideMerge(lists, 0, lists.size() - 1);
    }
```

### 复杂度：

> - 时间复杂度：$O(nlog_2K)$,其中$n$为所有链表的总节点数，分治为二叉树型递归，最坏情况下二叉树每层合并都是O(N)个节点,因为分治一共有$O(log_2K)$
> - 空间复杂度：$O(log_2K)$,最坏的情况需要向下递归$log_2K$层，需要$log_2K$个函数栈帧

## 方法三：优先队列-->借鉴牛客题解

### 思路：

如果非要按照归并排序的合并思路，双指针不够用，我们可以直接准备*k*个指针，每次比较得出*k*个数字中的最小值，我们可以借助堆，也就是优先队列--->priority_queue来完成这一点。

### 代码：

```cpp
   struct cmp
    {
        bool operator()(ListNode* a, ListNode* b)
        {
            return a->val > b->val;
        }
    };
    ListNode* mergeKLists(vector<ListNode*>& lists) 
    {
        //小根堆
        priority_queue<ListNode*, vector<ListNode*>, cmp> pq;

        //遍历所有链表的第一个元素,并且把不为空的加入到优先队列
        for(int i = 0; i < lists.size(); ++i)
        {
            if(lists[i] != nullptr) pq.push(lists[i]);
        }

        ListNode* res = new ListNode(-1);
        ListNode* cur = res;
        while(!pq.empty())
        {
            //取出最小的元素
            ListNode* t = pq.top();
            pq.pop();
            //链接到尾部
            cur->next = t;
            cur = cur->next;
            
            //将该链表的下一个指针加入到优先队列
            if(t->next != nullptr)
            pq.push(t->next);
        }

        return res->next;
    }
```

### 复杂度：

> - 时间复杂度：$O(nlog_2K)$,其中$n$为所有链表的总节点数，每次加入优先队列的复杂度为$O(log_2K)$
> - 空间复杂度：$O(K)$，优先队列的大小为K
