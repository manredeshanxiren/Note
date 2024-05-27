# 牛客热题：寻找第K大

## 题目链接

[寻找第K大_牛客题霸_牛客网 (nowcoder.com)](https://www.nowcoder.com/practice/e016ad9b7f0b45048c58a9f27ba618bf?tpId=295&tqId=44581&ru=/exam/oj&qru=/ta/format-top101/question-ranking&sourceUrl=%2Fexam%2Foj)

## 方法一：使用C++内置sort

### 思路

### 代码

```cpp
    template<typename T>
    struct greater
    {
        bool operator()(const T& a, const T& b)
        {
            return a > b;
        }
    };
    
    int findKth(vector<int>& a, int n, int K) 
    {
        sort(a.begin(), a.end(), greater<int>());
        return a[K - 1];
    }
```

### 复杂度

> - 时间复杂度
>
>   - `sort` 函数使用的是 C++ 标准库中的快速排序（通常为 Timsort 或者是混合了快速排序、归并排序和插入排序的算法），其平均时间复杂度为 O($n log n$)。
>   - 访问数组中的第 K 个元素的时间复杂度为 O(1)。
>
>   综合起来，函数 `findKth` 的时间复杂度为：
>   O($n log n$) + O(1) = O($nlog n$)
>
>   ### 空间复杂度
>
>   - `sort` 函数需要额外的空间来进行排序。对于 Timsort，额外的空间复杂度为 \(O(n)\)。
>   - 代码中没有使用其他显著的额外空间，因此额外的空间主要来自排序算法本身。
>
>   因此，函数 `findKth` 的空间复杂度为：
>   \[ O(n) \]
>
>   ### 总结
>
>   - **时间复杂度**： O($nlog n$)
>   - **空间复杂度**： O($n$)
>
>   这是因为 `sort` 函数在最坏情况下需要线性的额外空间，同时进行排序的平均时间复杂度为 O($n log n$)。

## 方法二：优先队列

### 思路

- **定义比较器 `greater`**：用于将优先队列构造成最小堆。
- **初始化最小堆优先队列 `pq`**。
- 遍历数组 `a` 中的每个元素：
  - 如果优先队列大小小于 K，直接推入元素。
  - 否则，比较当前元素与堆顶元素，替换堆顶元素。
- **返回结果**：优先队列顶部元素即为第 K 大的元素。

### 代码

```cpp
template<typename T>
struct greater
{
    bool operator()(const T& a, const T& b)
    {
        return a > b;
    }
};

int findKth(std::vector<int>& a, int n, int K) 
{
    std::priority_queue<int, std::vector<int>, greater<int>> pq;
    
    for(auto& aa : a)
    {
        if (pq.size() < K) 
        {
            pq.push(aa);
        } 
        else if(aa > pq.top()) 
        {
            pq.pop();
            pq.push(aa);
        }
    }

    return pq.empty() ? -1 : pq.top();
}
```

### 复杂度

> 时间复杂度：O($nlogK$),优先队列的插入和替换堆顶都是O($logK$)的时间复杂度
>
> 空间复杂度：O(K),我们要简历一个K个元素大小的堆

## 方法三：快速选择算法

### 思路

> 1. **`Partition`函数**：
>    - 类似于快速排序的分区函数。选择最右边的元素作为枢纽（pivot）。
>    - 遍历数组，将大于枢纽的元素移到左边，小于或等于枢纽的元素移到右边。
>    - 返回枢纽的位置。
> 2. **`QuickSelect`函数**：
>    - 递归地调用分区函数，并根据枢纽的位置与 K 比较，决定在哪个子数组继续查找。
>    - 如果枢纽的位置等于 K，则找到了第 K 大的元素。
>    - 否则根据 K 的位置选择继续在左子数组或右子数组查找。
> 3. **`FindKthLargest`函数**：
>    - 调用 `QuickSelect` 函数来找到第 K 大的元素。注意传入的 K 需要减 1，因为数组索引从 0 开始。

### 代码

```cpp
#include <vector>
#include <algorithm>

int partition(std::vector<int>& nums, int left, int right) {
    int pivot = nums[right];
    int i = left;
    for (int j = left; j < right; ++j) {
        if (nums[j] > pivot) {
            std::swap(nums[i], nums[j]);
            ++i;
        }
    }
    std::swap(nums[i], nums[right]);
    return i;
}

int quickSelect(std::vector<int>& nums, int left, int right, int K) {
    if (left == right) {
        return nums[left];
    }
    int pivotIndex = partition(nums, left, right);
    if (K == pivotIndex) {
        return nums[K];
    } else if (K < pivotIndex) {
        return quickSelect(nums, left, pivotIndex - 1, K);
    } else {
        return quickSelect(nums, pivotIndex + 1, right, K);
    }
}

int findKthLargest(std::vector<int>& nums, int K) {
    return quickSelect(nums, 0, nums.size() - 1, K - 1);
}

```

### 复杂度

> - **时间复杂度**：
>   - 平均时间复杂度是 *O*(*n*)，因为每次分区操作将数组划分成两部分。
>   - 最坏情况下时间复杂度是 *O*($n^2$)，当每次选择的枢纽总是最小或最大的元素时。
> - **空间复杂度**：
>   - 空间复杂度是 *O*(1)，因为是原地排序，没有使用额外的空间，递归调用的栈空间是 *O*($logn$)（平均情况）。

