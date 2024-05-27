# 牛客热题：最小的k个数

## 题目链接

[最小的K个数_牛客题霸_牛客网 (nowcoder.com)](https://www.nowcoder.com/practice/6a296eb82cf844ca8539b57c23e6e9bf?tpId=295&tqId=23263&ru=/exam/oj&qru=/ta/format-top101/question-ranking&sourceUrl=%2Fexam%2Foj)

## 方法一：大根堆

### 思路

采用大根堆的方式

- 堆顶的元素为堆中最大的元素

  step1：堆中不够k个元素就向堆中插入数据

  step2：堆顶的元素大于当前遍历的元素，那么就将堆顶pop，然后将当前元素入堆

- 最后将堆中的元素全部放入到答案数组

### 代码

```cpp
 vector<int> GetLeastNumbers_Solution(vector<int>& input, int k) 
    {
        if(k > input.size() || k == 0) return {};
        priority_queue<int> pq;
        vector<int> res;
        for(auto& num : input)
        {
            //如果不够k个数就入队
            if(pq.size() < k) pq.push(num);
            //如果堆顶元素大于当前的元素，堆顶出队，然后将当前元素入队
            else if(pq.top() > num)
            {
                pq.pop();
                pq.push(num);
            }
        }

        while(!pq.empty())
        {
            res.push_back(pq.top());
            pq.pop();
        }
        return res;
    }
```

### 复杂度

> 时间复杂度：O($NlogK$), 插入容量为k的大根堆时间复杂度为O(longk), 一共遍历n个元素
>
> 空间复杂度：O(K), 容量为K的优先队列

## 方法二：排序

### 思路

- 将输入数组按从小到大排序后
- 用前k个数据构造答案数组即可

最后返回答案数组

### 代码

```cpp
    vector<int> GetLeastNumbers_Solution(vector<int>& input, int k) 
    {
        if(k > input.size() || k == 0) return {};
        //排序
        sort(input.begin(), input.end());
        //用前k个元素去构造答案数组
        vector<int> res(input.begin(), input.begin() + k);
        
        return res;
    }
```

### 复杂度

> 时间复杂度：O($NlogN$) , sort的时间复杂度
>
> 空间复杂度：O(N), 答案数组的长度