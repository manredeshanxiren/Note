# C++map和set

## Ⅰ. 关联式容器

> 在初阶阶段，我们已经接触过STL中的部分容器，比如：vector、list、deque、 forward_list(C++11)等，这些容器统称为序列式容器，因为其底层为线性序列的数据结构，里面 存储的是元素本身。那什么是关联式容器？它与序列式容器有什么区别？ 关联式容器也是用来存储数据的，与序列式容器不同的是，**其里面存储的是结构的 键值对，在数据检索时比序列式容器效率更高。**

## Ⅱ. 键值对

> **用来表示具有一一对应关系的一种结构，该结构中一般只包含两个成员变量key和value，key代 表键值，value表示与key对应的信息。**比如：现在要建立一个英汉互译的字典，那该字典中必然 有英文单词与其对应的中文含义，而且，英文单词与其中文含义是一一对应的关系，即通过该应 该单词，在词典中就可以找到与其对应的中文含义。
>
> SGI-STL中关于键值对的定义：
>
> ```cpp
> template <class T1, class T2>
> struct pair
> {
> typedef T1 first_type;
> typedef T2 second_type;
> T1 first;
> T2 second;
> pair(): first(T1()), second(T2())
> {}
> pair(const T1& a, const T2& b): first(a), second(b)
> {}
> };
> ```

## Ⅲ. 树形结构的关联式容器

> 根据应用场景的不同，STL总共实现了两种不同结构的管理式容器：树型结构与哈希结构。**树型结 构的关联式容器主要有四种：map、set、multimap、multiset。**这四种容器的共同点是：使 用平衡搜索树(即红黑树)作为其底层结果，容器中的元素是一个有序的序列。下面一依次介绍每一 个容器。

### Ⅲ. Ⅰ . set

> 概念：
>
> [set官方文档](https://cplusplus.com/reference/set/set/?kw=set)
>
> ①set是按照一定次序存储元素的容器
>
> ②在set中，元素的value也标识它(value就是key，类型为T)，并且每个value必须是唯一的。 set中的元素不能在容器中修改(元素总是const)，但是可以从容器中插入或删除它们。
>
> ③在内部，set中的元素总是按照其内部比较对象(类型比较)所指示的特定严格弱排序准则进行 排序。
>
> ④set容器通过key访问单个元素的速度通常比unordered_set容器慢，但它们允许根据顺序对 子集进行直接迭代。
>
> ⑤set在底层是用二叉搜索树(红黑树)实现的。
>
> 注意：
>
> ①与map/multimap不同，map/multimap中存储的是真正的键值对，set中只放 value，但在底层实际存放的是由构成的键值对。
>
> ②set中插入元素时，只需要插入value即可，不需要构造键值对。
>
> ③set中的元素不可以重复(因此可以使用set进行去重)。
>
> ④使用set的迭代器遍历set中的元素，可以得到有序序列
>
> ⑤set中的元素默认按照小于来比较
>
> ⑥set中查找某个元素，时间复杂度为：$log_2 n$
>
> ⑦set中的元素不允许修改(为什么?会破坏搜索二叉树的结构)
>
> ⑧set中的底层使用二叉搜索树(红黑树)来实现。

### Ⅲ. Ⅱ set的使用

> 1. set的模板参数列表
>
>    ![image-20230807220658315](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230807220658315.png)
>
>    - T: set中存放元素的类型，实际在底层存储的键值对。 
>    - Compare：set中元素默认按照小于来比较 
>    - Alloc：set中元素空间的管理方式，使用STL提供的空间配置器管理
>
>    关于set的构造，迭代器，容量修改操作直接去官网文档中查看。
>
>    这里我们距离一个set的使用
>
> 2. set的举例使用
>
>    ```cpp
>    #include <set>
>    void TestSet()
>    {
>        // 用数组array中的元素构造set
>        int array[] = { 1, 3, 5, 7, 9, 2, 4, 6, 8, 0, 1, 3, 5, 7, 9, 2, 4, 
>    6, 8, 0 };
>     set<int> s(array, array+sizeof(array)/sizeof(array));
>     cout << s.size() << endl;
>     // 正向打印set中的元素，从打印结果中可以看出：set可去重
>     for (auto& e : s)
>     cout << e << " ";
>     cout << endl;
>     // 使用迭代器逆向打印set中的元素
>     for (auto it = s.rbegin(); it != s.rend(); ++it)
>     cout << *it << " ";
>     cout << endl;
>     // set中值为3的元素出现了几次
>     cout << s.count(3) << endl;
>    }
>    ```

### Ⅲ. Ⅲ map

> **map的介绍**
>
> [map官方文档](https://cplusplus.com/reference/map/map/?kw=map)
>
> ①map是关联容器，它按照特定的次序(按照key来比较)存储由键值key和值value组合而成的元素。
>
> ②在map中，键值key通常用于排序和惟一地标识元素，而值value中存储与此键值key关联的 内容。键值key和值value的类型可能不同，并且在map的内部，key与value通过成员类型 value_type绑定在一起，为其取别名称为pair:` typedef pair value_type;`
>
> ③在内部，map中的元素总是按照键值key进行比较排序的。
>
> ④ map中通过键值访问单个元素的速度通常比unordered_map容器慢，但map允许根据顺序 对元素进行直接迭代(即对map中的元素进行迭代时，可以得到一个有序的序列)。
>
> ⑤ map支持下标访问符，即在[]中放入key，就可以找到与key对应的value。
>
> ⑥ map通常被实现为二叉搜索树(更准确的说：平衡二叉搜索树(红黑树))。

### Ⅲ. Ⅳ map的使用

> 1. map的模板参数说明
>    ![image-20230807223356836](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230807223356836.png)
>    key: 键值对中key的类型 
>
> T： 键值对中value的类型 
>
> Compare: 比较器的类型，map中的元素是按照key来比较的，缺省情况下按照小于来比 较，一般情况下(内置类型元素)该参数不需要传递，如果无法比较时(自定义类型)，需要用户 自己显式传递比较规则(一般情况下按照函数指针或者仿函数来传递) 
>
> Alloc：通过空间配置器来申请底层空间，不需要用户传递，除非用户不想使用标准库提供的 空间配置器 
>
> 注意：在使用map时，需要包含头文件。
>
> 2. 问题：当key不在map中时，通过operator获取对应value时会发生什么问题？
>
>    注意：在元素访问时，有一个与operator[]类似的操作at()(该函数不常用)函数，都是通过 key找到与key对应的value然后返回其引用，不同的是：当key不存在时，operator[]用默认 value与key构造键值对然后插入，返回该默认value，at()函数直接抛异常。
>
> 3. map使用举例
>
>    ```cpp
>    #include <string>
>    #include <map>
>    void TestMap()
>    {
>     map<string, string> m;
>     // 向map中插入元素的方式：
>     // 将键值对<"peach","桃子">插入map中，用pair直接来构造键值对
>     m.insert(pair<string, string>("peach", "桃子"));
>     // 将键值对<"peach","桃子">插入map中，用make_pair函数来构造键值对
>     m.insert(make_pair("banan", "香蕉"));
>        
>     // 借用operator[]向map中插入元素
>        /*
>     operator[]的原理是：
>      用<key, T()>构造一个键值对，然后调用insert()函数将该键值对插入到map中
>      如果key已经存在，插入失败，insert函数返回该key所在位置的迭代器
>      如果key不存在，插入成功，insert函数返回新插入元素所在位置的迭代器
>      operator[]函数最后将insert返回值键值对中的value返回
>     */
>        // 将<"apple", "">插入map中，插入成功，返回value的引用，将“苹果”赋值给该引
>    用结果，
>     m["apple"] = "苹果";
>     // key不存在时抛异常
>     //m.at("waterme") = "水蜜桃";
>     cout << m.size() << endl;
>     // 用迭代器去遍历map中的元素，可以得到一个按照key排序的序列
>     for (auto& e : m)
>     cout << e.first << "--->" << e.second << endl;
>     cout << endl;
>     // map中的键值对key一定是唯一的，如果key存在将插入失败
>     auto ret = m.insert(make_pair("peach", "桃色"));
>     if (ret.second)
>     cout << "<peach, 桃色>不在map中, 已经插入" << endl;
>     else
>     cout << "键值为peach的元素已经存在：" << ret.first->first << "--->"
>    << ret.first->second <<" 插入失败"<< endl;
>     // 删除key为"apple"的元素
>     m.erase("apple");
>     if (1 == m.count("apple"))
>     cout << "apple还在" << endl;
>     else
>     cout << "apple被吃了" << endl;
>    }
>       
>    ```
>
>    [总结]
>
>    ①map中的的元素是键值对
>
>    ②map中的key是唯一的，并且不能修改
>
>    ③默认按照小于的方式对key进行比较
>
>    ④map中的元素如果用迭代器去遍历，可以得到一个有序的序列
>
>    ⑤map的底层为平衡搜索树(红黑树)，查找效率比较高$O(log_2 N)$
>
>    ⑥支持[]操作符，operator[]中实际进行插入查找。

### Ⅲ. Ⅴ multiset

> multiset的介绍
>
> [multiset官方文档](https://cplusplus.com/reference/set/multiset/?kw=multiset)
>
> ①multiset是按照特定顺序存储元素的容器，其中元素是可以重复的。
>
> ②在multiset中，元素的value也会识别它(因为multiset中本身存储的就是组成 的键值对，因此value本身就是key，key就是value，类型为T). multiset元素的值不能在容器 中进行修改(因为元素总是const的)，但可以从容器中插入或删除。
>
> ③在内部，multiset中的元素总是按照其内部比较规则(类型比较)所指示的特定严格弱排序准则 进行排序。
>
> ④multiset容器通过key访问单个元素的速度通常比unordered_multiset容器慢，但当使用迭 代器遍历时会得到一个有序序列。
>
> ⑤multiset底层结构为二叉搜索树(红黑树)。
>
> 注意：
>
> ①multiset中再底层中存储的是的键值对
>
> ②multiset的插入接口中只需要插入即可
>
> ③与set的区别是，multiset中的元素可以重复，set是中value是唯一的
>
> ④使用迭代器对multiset中的元素进行遍历，可以得到有序的序列
>
> ⑤multiset中的元素不能修改
>
> ⑥在multiset中找某个元素，时间复杂度为$O(log_2 N)$
>
> ⑦multiset的作用：可以对元素进行排序

### Ⅲ. Ⅵ multiset的使用

> 此处只简单演示set与multiset的不同，其他接口接口与set相同，同学们可参考set。
>
> ```cpp
> #include <set>
> void TestSet()
> {
>   int array[] = { 2, 1, 3, 9, 6, 0, 5, 8, 4, 7 };
>  
>  // 注意：multiset在底层实际存储的是<int, int>的键值对
>  multiset<int> s(array, array + sizeof(array)/sizeof(array[0]));
>  for (auto& e : s)
>  cout << e << " ";
>  cout << endl;
>  return 0;
> }
> ```

### Ⅲ. Ⅶ multimap

> multimap的介绍
>
> [multimap的官方文档](https://cplusplus.com/reference/map/multimap/?kw=multimap)
>
> ①Multimaps是关联式容器，它按照特定的顺序，存储由key和value映射成的键值对，其中多个键值对之间的key是可以重复的。
>
> ②在multimap中，通常按照key排序和惟一地标识元素，而映射的value存储与key关联的内 容。key和value的类型可能不同，通过multimap内部的成员类型value_type组合在一起， value_type是组合key和value的键值对:
>
> `typedef pair value_type;`
>
> ③在内部，multimap中的元素总是通过其内部比较对象，按照指定的特定严格弱排序标准对 key进行排序的。
>
> ④multimap通过key访问单个元素的速度通常比unordered_multimap容器慢，但是使用迭代 器直接遍历multimap中的元素可以得到关于key有序的序列。
>
> ⑤multimap在底层用二叉搜索树(红黑树)来实现。
>
> **注意：**
>
> **multimap和map的唯一不同就是：map中的key是唯一的，而multimap中key是可以 重复的。**

### Ⅲ. Ⅷ multimap的使用

> multimap中的接口可以参考map，功能都是类似的。
>
>  注意： 
>
> 1.  multimap中的key是可以重复的。 
> 2.  multimap中的元素默认将key按照小于来比较 
> 3.  multimap中没有重载operator[]操作(同学们可思考下为什么?)。 
> 4. 使用时与map包含的头文件相同：`#include<map>`
