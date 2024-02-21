# C++类型转换

## 1.为什么C++需要四种类型转换

> C风格的转换格式很简单，但是有不少缺点的：
>
> 1.隐式类型转化有些情况下可能会出问题：比如数据精度丢失
>
> 2.显式类型转换将所有情况混合在一起，代码不够清晰
> 因此C++提出了自己的类型转化风格，注意因为C++要兼容C语言，所以C++中还可以使用C语言的
> 转化风格。

## 2.C++的强制类型转换

> 标准C++为了加强类型转换的可视性，引入了四种命名的强制类型转换操作符：
> static_cast、reinterpret_cast、const_cast、dynamic_cast  

### 2.1static_cast

> **static_cast用于非多态类型的转换（静态转换），编译器隐式执行的任何类型转换都可用**
> **static_cast，但它不能用于两个不相关的类型进行转换**  
>
> ```c++
> int main()
> {
> 	double d = 12.34;
> 	int a = static_cast<int>(d);
> 	cout<<a<<endl;
> 	return 0;
> }
> ```

### 2.2reinterpret_cast

> reinterpret_cast操作符通常为操作数的位模式提供较低层次的重新解释，用于将一种类型转换
> 为另一种不同的类型。
>
> ```c++
> int main()
> {
> 	double d = 12.34;
> 	int a = static_cast<int>(d);
> 	cout << a << endl;
> 	// 这里使用static_cast会报错，应该使用reinterpret_cast
> 	//int *p = static_cast<int*>(a);
> 	int *p = reinterpret_cast<int*>(a);
> 	return 0;
> }
> ```

### 2.3const_cast

> const_cast最常用的用途就是删除变量的const属性，方便赋值  
>
> ```c++
> void Test ()
> {
> 	const int a = 2;
> 	int* p = const_cast<int*>(&a);
> 	*p = 3;
> 	cout<<a <<endl;
> }
> ```

### 2.4dynamic_cast

> dynamic_cast用于将一个父类对象的指针/引用转换为子类对象的指针或引用(动态转换)
> **向上转型**：子类对象指针/引用->父类指针/引用(不需要转换，赋值兼容规则)
> **向下转型**：父类对象指针/引用->子类指针/引用(用dynamic_cast转型是安全的)
> 注意：
> 1.dynamic_cast只能用于父类含有虚函数的类  
>
> 2.dynamic_cast会先检查是否能转换成功，能成功则转换，不能则返回0  
>
> ```c++
> #include<iostream>
> 
> using namespace std;
> 
> class A
> {
> public:
> 	virtual void f() {}
> };
> class B : public A
> {};
> void fun(A* pa)
> {
> 	// dynamic_cast会先检查是否能转换成功，能成功则转换，不能则返回
> 	B* pb1 = static_cast<B*>(pa);
> 	B* pb2 = dynamic_cast<B*>(pa);
> 	cout << "pb1:" << pb1 << endl;
> 	cout << "pb2:" << pb2 << endl;
> }
> int main()
> {
> 	A a; //父类对象
> 	B b; //子类对象
> 	fun(&a); //父类对象是不能转换成子类对象的，所以pb2返回的是0
> 	fun(&b); //子类对象是可以转换成父类对象的，所以均为有效的指针
> 	return 0;
> }
> ```

## 3.常见的面试题

> ①说说四种类型转换的应用场景
>
> ②四种类型转换