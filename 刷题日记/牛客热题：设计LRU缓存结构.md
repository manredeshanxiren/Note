# 牛客热题：设计LRU缓存结构

## 题目链接

[设计LRU缓存结构_牛客题霸_牛客网 (nowcoder.com)](https://www.nowcoder.com/practice/5dfded165916435d9defb053c63f1e84?tpId=295&tqId=2427094&ru=/exam/oj&qru=/ta/format-top101/question-ranking&sourceUrl=%2Fexam%2Foj)

## 方法一：map+双向链表

### 思路

`set`：

​	step1：查看对应的key是否已经被set过

​	step2：如果set过就将对应的节点移动到链表头，并且在哈希中改变其val的值

​	step3：如果没有set过就构建一个新的节点在哈希中创建映射，然后将新的节点插入到链表的头部

​	step4：查看缓存空间是否溢出

​	step5:  如果溢出就删除对应的链表尾部的节点，然后将哈希中的映射删除掉，并释放掉对应的节点

`get`：

​	step1：查看是否在哈希表中出现过

​	step2：如果出现过，将其在链表中的节点移动到链表头的位置，并返回对应的val

​	step3：如果没出现过,那么直接返回-1

### 代码

```cpp
class Node
{
public:
    int _key;
    int _val;
    Node* _prev;
    Node* _next;
    //初始化
public:
    Node(int key, int val)
    {
        _key = key;
        _val = val;
        _prev = nullptr;
        _next = nullptr;
    }
};

class Solution {
public:
 Solution(int capacity)
 {
    size = capacity;
    head = new Node(0, 0);
    tail = new Node(0, 0);
    //将头节点和尾部节点连上
    head->_next = tail;
    tail->_prev = head;
 }
 
 int get(int key) 
 {
    if(hash.count(key))
    {
        MoveToHead(hash[key]);
        return hash[key]->_val;
    }
    else
    {
        return -1;
    } 
 }
 
 void set(int key, int value)
 {
    //1.是否已经被set过
    if(hash.count(key))
    {
        //2.将其节点移动到链表头
        MoveToHead(hash[key]);
        hash[key]->_val = value;
        cout << 2 << endl;
    }
    else 
    {
        //3.将新节点插入到链表的头部
        Node* NewNode = new Node(key, value);
        InsertToHead(NewNode);
        hash[key] = NewNode;

        //4.如果空间不够
        if(size <= 0)
        {
            //将链表尾部的节点去掉
            EraseLast();
            size++;
        }
        //5.减小空间
        size--;
    }
 }

void EraseLast()
{
    //先在哈希表中删除映射
    hash.erase(tail->_prev->_key);
    //在链表中删除
    Node* cur = tail->_prev;
    tail->_prev->_prev->_next = tail;
    tail->_prev = tail->_prev->_prev;
    delete cur;
    cur = nullptr;
}

void MoveToHead(Node* node)
{
    //已经是头部
    if(head == node->_prev)
    {
        return ;
    }
    //先将这个节点拆下来
    node->_prev->_next = node->_next;
    node->_next->_prev = node->_prev;
    
    InsertToHead(node);
}

void InsertToHead(Node* node)
{
    node->_next = head->_next;
    node->_prev = head;
    head->_next->_prev = node;
    head->_next = node;
}

private:
    int size;
    Node* head;
    Node* tail;
    map<int, Node*> hash;
};
```

### 复杂度

> 时间复杂度:O(1),不论是set操作还是get操作均在O(1)的时间复杂度
>
> 空间复杂度:O(N), 利用到了长度等于size+2的链表长度和对应的map