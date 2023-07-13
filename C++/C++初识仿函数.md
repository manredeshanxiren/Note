# C++初识仿函数

## 1.仿函数的介绍

> 我们可以从名字中看出仿函数就是一个仿造的函数，实际上也是如此，它并非一个真正的函数。而是通过一个类重载`()`运算符实现的。
>
> 接下来给出定义：
>
> - 仿函数是定义了一个含有**operator()**成员函数的对象，可以视为一个一般的函数，只不过这个函数功能是在一个类中的运算符operator()中实现，是一个函数对象，它将函数作为参数传递的方式来使用。

## 2.仿函数的优缺点

> 优点：
>
> - 仿函数比函数指针的执行速度快，函数指针时通过地址调用，而仿函数是对运算符operator进行自定义来提高调用的效率。
> - 仿函数比一般函数灵活，可以同时拥有两个不同的状态实体(即采用匿名对象和非匿名对象来调用)，一般函数不具备此种功能
> - 仿函数可以作为模板参数使用，因为每个仿函数都拥有自己的类型
>
> 缺点：
>
> - 需要单独实现一个类。
> - 定义形式比较复杂。

## 3.简单实现一个仿函数的例子

> 通俗来讲，仿函数就是使用一个类通过重载`()`这个运算符实现的。
>
> ```cpp
> template<class T>
>     //实现
> 	struct less
> 	{
> 		bool operator()(T& a, T& b)
> 		{
> 			return a < b;
> 		}
> 	};
> 
> 	template<class T>
> 	struct greater
> 	{
> 		bool operator()(T& a, T& b)
> 		{
> 			return a > b;
> 		}
> 	};
> //采用匿名对象的方式调用
> void AdjustUp(int child)
> 		{
> 			int parent = (child - 1) >> 1;
> 			while (child)
> 			{
> 				//小堆
> 				//if (_con[child] < _con[parent])
> 				if(Compare()(_con[child], _con[parent]))
> 				{
> 					swap(_con[parent], _con[child]);
> 					child = parent;
> 				}
> 				else
> 				{
> 					return;
> 				}
> 			}
> 		}
> 
> 		void AdjustDown(int parent)
> 		{
> 			//小堆
> 			int child = parent * 2 + 1;
> 			while (child < _con.size())
> 			{
> 				//找到两个孩子节点之中较小的那个
> 				//if (child + 1 < _con.size() && _con[child] > _con[child + 1])
> 				if (child + 1 < _con.size() && Compare()(_con[child], _con[child + 1]))
> 				{
> 					child += 1;
> 				}
> 				//if (_con[parent] > _con[child])
> 				if(Compare()(_con[parent], _con[child]))
> 				{
> 					swap(_con[parent], _con[child]);
> 					parent = child;
> 					child = parent * 2 + 1;
> 				}
> 				else
> 				{
> 					return;
> 				}
> 			}
> 		}
> //采用实例化对象的方式调用
> 		void AdjustUp(int child)
> 		{
> 			Compare _com;
> 			int parent = (child - 1) >> 1;
> 			while (child)
> 			{
> 				//小堆
> 				//if (_con[child] < _con[parent])
> 				if(_com(_con[child], _con[parent]))
> 				{
> 					swap(_con[parent], _con[child]);
> 					child = parent;
> 				}
> 				else
> 				{
> 					return;
> 				}
> 			}
> 		}
> 
> 		void AdjustDown(int parent)
> 		{
> 			Compare _com;
> 			//小堆
> 			int child = parent * 2 + 1;
> 			while (child < _con.size())
> 			{
> 				//找到两个孩子节点之中较小的那个
> 				//if (child + 1 < _con.size() && _con[child] > _con[child + 1])
> 				if (child + 1 < _con.size() && _com(_con[child], _con[child + 1]))
> 				{
> 					child += 1;
> 				}
> 				//if (_con[parent] > _con[child])
> 				if(_com(_con[parent], _con[child]))
> 				{
> 					swap(_con[parent], _con[child]);
> 					parent = child;
> 					child = parent * 2 + 1;
> 				}
> 				else
> 				{
> 					return;
> 				}
> 			}
> 		}
> ```















