# C++反向迭代器

## 1.反向迭代器的概念

> 我们通过学习`stl`库我们了解到迭代器分为正向迭代器和反向迭代器。正向迭代器适用于正向遍历该数据类型的中的所有元素。
>
> 所以反向的迭代器顾名思义，就是反向遍历该数据类型中的所有元素。
>
> 接下来我将详细讲解如何自己实现一个反向迭代器。

## 2.反向迭代器的模拟实现

> 反向迭代器的实现思路有两个：
>
> - 通过对正向迭代器的代码进行修改实现。
> - 采用适配器的写法原理将正向迭代器作为一个模板参数来实现。
>
> 对于上述两个方法来说：
>
> ​	第一种方法只适用于对应类型的正向迭代器，而第二种的实现方式可以实现一个通用的模板适用于**所有的正向迭代器是双向迭代器的类型**,因此我们接下来用第二种方法来实现反向迭代器。
>
> 以下是源代码：
>
> ```cpp
> //iterator.h
> #pragma once
> 
> namespace xupt
> {
> 	 //这里我们采用将正向迭代器作为一个模板参数
> 	  template<class Iterator,class Ref, class Ptr>
> 	  struct Reverse_Iterator
> 	  {
> 		  Iterator _cur;
> 		  typedef Reverse_Iterator<Iterator, Ref, Ptr> Self;
> 
> 		  Reverse_Iterator(const Iterator& it)
> 			  :_cur(it)
> 		  {}
> 
> 		  Ref operator*()
> 		  {
> 			  Iterator tmp = _cur;
> 			  --tmp;
> 			  return *tmp;
> 		  }
> 			//反向迭代器的++就是正向迭代器的--
> 		  Self& operator++()
> 		  {
> 			  --_cur;
> 			  return *this;
> 		  }
> 
> 		  Self& operator--()
> 		  {
> 			  ++_cur;
> 			  return *this;
> 		  }
> 
> 		  bool operator!=(const Self& s)
> 		  {
> 			  return _cur != s._cur;
> 		  }
> 
> 	  };
> 
> 
> 
> };
> 
> 
> //list.h
> 	template<class T> 
> 	struct list
> 	{
> 		typedef list_node<T> node;
> 		
> 	public:
> 		typedef _list_iterator<T, T&, T*> iterator;
> 		typedef _list_iterator<T, const T&, const T*> const_iterator;
> 		typedef Reverse_Iterator<iterator, T&, T*> reverse_iterator;
> 		typedef Reverse_Iterator<iterator, const T&, const T*> const_reverse_iterator;
> 
> 
> 		reverse_iterator rbegin()
> 		{
> 			return reverse_iterator(end());
> 		}
> 		reverse_iterator rend()
> 		{
> 			return reverse_iterator(begin());
> 		}
> 		const_reverse_iterator rbegin() const
> 		{
> 			return const_reverse_iterator(end());
> 		}
> 		const_reverse_iterator rend() const
> 		{
> 			return const_reverse_iterator(begin());
> 		}
> 
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
> 
> ```
>
> 【注意】
>
> ​	这里我们看到解引用操作符的重载实现方式如下：
>
> ```cpp
> Ref operator*()
> {
> 	Iterator tmp = _cur;
> 	--tmp;
> 	return *tmp;
> }
> ```
>
> 为什么这里是先进行了--再进行解引用呢？
>
> 如下如所示我们可以看到因为我们采用的是rbegin对应end，rend对应begin的方法。所以我们当前迭代器指向的元素并不是我们实际想解引用的元素。
>
> 所以我们先进行了`--`操作才进行解引用，这样就可以得到我们想得到的元素内容了！
>
> <img src="C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230528163624973.png" alt="image-20230528163624973" style="zoom: 35%;" />

