# C++ list

​	

## Ⅰ. list的介绍和使用

### Ⅰ. Ⅰlist的介绍
>
> [list 介绍文档](https://legacy.cplusplus.com/reference/list/list/)
>
> ①list是可以在常数范围内在任意位置进行插入和删除的序列式容器，并且该容器可以前后双向迭代。
>
> ②list的**<font color='orange'>底层是双向链表结构</font>**，双向链表中每个元素存储在互不相关的独立节点中，在节点中通过指针指向 其前一个元素和后一个元素。
>
> ③list与forward_list非常相似：最主要的不同在于forward_list是单链表，只能朝前迭代，已让其更简单高效。
>
> ④与其他的序列式容器相比(array，vector，deque)，list通常在任意位置进行插入、移除元素的执行效率更好。
>
> ⑤与其他序列式容器相比，list和forward_list最大的缺陷是不支持任意位置的随机访问，比如：要访问list 的第6个元素，必须从已知的位置(比如头部或者尾部)迭代到该位置，在这段位置上迭代需要线性的时间 开销；list还需要一些额外的空间，以保存每个节点的相关联信息(对于存储类型较小元素的大list来说这 可能是一个重要的因素)



### Ⅰ. Ⅱ list的使用
>
> 在上面的介绍中我们了解到list的底层是使用双向循环链表实现的。list的接口非常的丰富，和vector有些类似，但不相同。以下是list一些常用的接口。



#### Ⅰ. Ⅱ . Ⅰlist的构造
>
>| 构造函数([construct](https://cplusplus.com/reference/list/list/list/)) | 接口说明                            |
>| ------------------------------------------------------------ | ----------------------------------- |
>| list (size_type n, const value_type& val = value_type())     | 构造的list中包含n个值为val的元素    |
>| list()                                                       | 构造空的list                        |
>| list (const list& x)                                         | 拷贝构造函数                        |
>| list (InputIterator first, InputIterator last)               | 用[first, last)区间中的元素构造list |
>
>```cpp
>		list()
>		{
>			empty_init();
>		}
>
>		template<class Iterator>
>		list(Iterator first, Iterator last)
>		{
>			empty_init();
>			Iterator it = first;
>			while (it != last)
>			{
>				push_back(*first);
>				++it;
>			}
>		}
>
>		void swap(list<T>& tmp)
>		{
>			std::swap(_head, tmp._head);
>		}
>
>		list(const list<T>& it)
>		{
>			//empty_init();
>			//for (auto e : it)
>			//{
>			//	push_back(e);
>			//}
>			list<T> tmp(it.begin(), it.end());
>			swap(tmp);
>		}
>```
>
>


#### Ⅰ. Ⅱ . Ⅱlist的使用
>
> 此处，大家可暂时将迭代器理解成一个指针，该指针指向list中的某个节点。
>
> |                           函数声明                           |                           函数接口                           |
> | :----------------------------------------------------------: | :----------------------------------------------------------: |
> | [begin](https://cplusplus.com/reference/list/list/begin/)+[end](https://cplusplus.com/reference/list/list/end/) | 返回第一个元素的迭代器+返回**<font color='orange'>最后一个元素下一个位置</font>**的迭代器 |
> | [rbegin]()+[rend](https://legacy.cplusplus.com/reference/list/list/rend/) | 返回第一个元素的reverse_iterator,即end位置，返回<font color='orange'>**最后一个元素下一个位置**</font>的 reverse_iterator,即begin位置 |
>
> 【注意】
>
> ①begin与end为正向迭代器，对迭代器执行++操作，迭代器向后移动
>
> ②rbegin(end)与rend(begin)为反向迭代器，对迭代器执行++操作，迭代器向前移动
>
> ```cpp
> template<class T, class Ref, class Ptr>
> 	struct _list_iterator
> 	{
> 		typedef list_node<T>  node;
> 		typedef _list_iterator<T, Ref, Ptr>  self;
> 		node* _node;
> 		_list_iterator(node* n)
> 			:_node(n)
> 		{}
> 		Ref operator *()
> 		{
> 			return _node->_data;
> 		}
> 
> 		Ptr operator ->()
> 		{
> 			return &_node->_data;
> 		}
> 		self& operator ++()
> 		{
> 			_node = _node->_next;
> 			return *this;
> 		}
> 		self& operator --()
> 		{
> 			_node = _node->_prev;
> 			return *this;
> 		}
> 		bool operator != (const self& s)
> 		{
> 			return _node != s._node;
> 		}
> 		bool operator == (const self& s)
> 		{
> 			return _node == s._node;
> 		}
> 	};
> 
> 
> 
> //list类内的代码：
> 
> 	public:
> 		typedef _list_iterator<T, T&, T*> iterator;
> 		typedef _list_iterator<T, const T&, const T*> const_iterator;
> 
> 		iterator begin()
> 		{
> 			return iterator(_head->_next);
> 		}
> 		iterator end()
> 		{
> 			return iterator(_head);
> 		}
> 		const_iterator begin() const
> 		{
> 			return const_iterator(_head->_next);
> 		}
> 		const_iterator end() const
> 		{
> 			return const_iterator(_head);
> 		}
> ```



#### Ⅰ. Ⅱ . Ⅲ list capacity

> | 函数声明                                                     | 接口说明                                    |
> | ------------------------------------------------------------ | ------------------------------------------- |
> | [bool empty() const;](https://cplusplus.com/reference/list/list/empty/) | 检测list是否为空，是返回true，否则返回false |
> | [size_type size() const;](https://cplusplus.com/reference/list/list/size/) | 返回list中有效节点的个数(即不包含哨兵节点)  |



#### Ⅰ. Ⅱ . Ⅳ list element access

> | 函数声明                                                     | 接口说明                         |
> | ------------------------------------------------------------ | -------------------------------- |
> | [reference front();const_reference front() const;](https://cplusplus.com/reference/list/list/front/) | 返回list的第一个节点中值的引用   |
> | [ reference back();const_reference back() const;](https://cplusplus.com/reference/list/list/back/) | 返回list的最后一个节点中值的引用 |



#### Ⅰ. Ⅱ . Ⅴ list modifiers

> | 函数声明                                                     | 接口说明                                |
> | ------------------------------------------------------------ | --------------------------------------- |
> | [void push_front (const value_type& val);](https://cplusplus.com/reference/list/list/push_front/) | 在list首元素前插入值为val的元素(头插)   |
> | [void pop_front();](https://cplusplus.com/reference/list/list/pop_front/) | 删除list中第一个元素(头删)              |
> | [void push_back (const value_type& val);](https://cplusplus.com/reference/list/list/push_back/) | 在list尾部插入值为val的元素(尾插)       |
> | [void pop_back();](https://cplusplus.com/reference/list/list/pop_back/) | 删除list中最后一个元素(尾删)            |
> | [insert](https://cplusplus.com/reference/list/list/insert/)  | 在list position 位置中插入值为val的元素 |
> | [erase](https://cplusplus.com/reference/list/list/erase/)    | 删除list position位置的元素             |
> | [swap](https://cplusplus.com/reference/list/list/swap/)      | 交换两个list中的元素                    |
> | [clear](https://cplusplus.com/reference/list/list/clear/)    | 清空list中的有效元素                    |
>
> ```cpp
>     int array[] = { 1, 2, 3 };
>     list<int> L(array, array + sizeof(array) / sizeof(array[0]));
> 
>     // 在list的尾部插入4，头部插入0
>     L.push_back(4);
>     L.push_front(0);
>     PrintList(L);
> 
>     // 删除list尾部节点和头部节点
>     L.pop_back();
>     L.pop_front();
>     PrintList(L);
> ```



#### <font color='orange'>Ⅰ. Ⅱ . Ⅵ list 迭代器失效</font>

> 前面说过，此处大家可将迭代器暂时理解成类似于指针，迭代器失效即迭代器所指向的节点的无效，即该节点被删除了。因为list的底层结构为带头结点的双向循环链表，因此在list中进行插入时是不会导致list的迭代 器失效的，只有在删除时才会失效，并且失效的只是指向被删除节点的迭代器，其他迭代器不会受到影响。
>
> ```cpp
> void TestListIterator1()
> {
>  int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
>  list<int> l(array, array+sizeof(array)/sizeof(array[0]));
>  auto it = l.begin();
>  while (it != l.end())
>  {
>  // erase()函数执行后，it所指向的节点已被删除，因此it无效，在下一次使用it时，必须先给
> 其赋值
>  l.erase(it); 
>  ++it;
>  }
> }
> // 改正
> void TestListIterator()
> {
>  int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
>  list<int> l(array, array+sizeof(array)/sizeof(array[0]));
>  auto it = l.begin();
>  while (it != l.end())
>  {
>  l.erase(it++); // it = l.erase(it);
>  }
> }
> 
> ```



## Ⅱ list的模拟实现

> [源代码：list]([learn_c/_list at main · manredeshanxiren/learn_c (github.com)](https://github.com/manredeshanxiren/learn_c/tree/main/_list))
