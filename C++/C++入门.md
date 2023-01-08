# C++入门

## Ⅰ.命名空间:

> 在C/C++中，变量、函数和后面要学到的类都是大量存在的，这些变量、函数和类的名称将都存在于全局作用域中，可能会导致很多冲突。使用命名空间的目的是对标识符的名称进行本地化，以避免命名冲突或名字污染，`namespace`关键字的出现就是针对这种问题的  

例如:

> 在C语言的情景中当我们引用`<stdlib.h>`这个库的时候当我们在去定义一个名为`rand`的变量的时候这时候就会报错了:
>
> ```c++
> #include <stdio.h>
> #include <stdlib.h>
> int rand = 10;
> // C语言没办法解决类似这样的命名冲突问题，所以C++提出了namespace来解决
> int main()
> {
> 	printf("%d\n", rand);
> 	return 0;
> }
> // 编译后后报错：error C2365: “rand”: 重定义；以前的定义是“函数”
> ```
>
> 

### Ⅰ. Ⅰ 命名空间的定义:

> 定义命名空间，需要使用到`namespace`关键字，后面跟命名空间的名字，然后接一对`{}`即可，`{}`中即为命名空间的成员。

Ⅰ. Ⅰ .Ⅰ正常的命名空间定义:

```c++
namespace sxr
{
	// 命名空间中可以定义变量/函数/类型
	int rand = 10;
	int Add(int left, int right)
	{
		return left + right;
	}
	struct Node
	{
		struct Node* next;
		int val;
	};
}
```

Ⅰ. Ⅰ .Ⅱ命名空间的嵌套:

> 如下的例子就是命名空间`sxr1`嵌套命名空间`sxr2`的例子;

```c++
namespace sxr1
{
	int a;
	int b;
	int Add(int left, int right)
	{
		return left + right;
	}
	namespace sxr2
	{
		int c;
		int d;
		int Sub(int left, int right)
		{
			return left - right;
		}
	}
}
```

拓展:

> ①同一个工程中**允许存在多个相同名称的命名空间**,编译器最后会合成同一个命名空间中。
>
> ②一个命名空间就定义了一个新的作用域，命名空间中的所有内容都局限于该命名空间中    

### Ⅰ. Ⅱ命名空间的使用:

①加命名空间名称及作用域限定符`::`:

```c++
int main()
{
	printf("%d\n", N::a);
	return 0;
}
```

②使用`using`将命名空间中某个成员引入(部分):

> 如下的案例是只将命名空间`N`中的`b`变量进行了引入:
>
> ```c++
> using N::b;
> int main()
> {
> 	printf("%d\n", N::a);
> 	printf("%d\n", b);
> 	return 0;
> }
> ```
>
> 

③使用`using namespace` 命名空间名称引入(全局):

> 这种方式会将整个命名空间中的名称全部引入，不是很推荐这种方法;
>
> ```c++
> using namespce N;
> int main()
> {
> 	printf("%d\n", N::a);
> 	printf("%d\n", b);
> 	Add(10, 20);
> 	return 0;
> }
> ```

## Ⅱ. C++的输入和输出:

> 在C语言中我们通常使用`scanf`和`printf`两个函数来实现输入和输出，并且他们是包含在标准输入输出库`<stdio.h>`中;
>
> 在C++中,是采用的`cin`标准输入对象(键盘)和`cout`标准输出对象(控制台)两个,在使用的过程中需要引头文件`<iostream>`和命名空间`std`;
>
> 例如:用C++的方式我们来输出`Hello world`;
>
> ```c++
> #include<iostream>
> // std是C++标准库的命名空间名，C++将标准库的定义实现都放到这个命名空间中
> using namespace std;
> int main()
> {
> 	cout << "Hello world!!!" << endl;
> 	return 0;
> }
> ```
>
> 

拓展:

> - 使用`cout`标准输出对象(控制台)和`cin`标准输入对象(键盘)时，必须包含`<iostream>`头文件以及按命名空间使用方法使用`std`;
> - `cout`和`cin`是全局的流对象，`endl`是特殊的C++符号，表示换行输出，他们都包含在包含`<iostream>`头文件中;
> - `<<`是流插入运算符，`>>`是流提取运算符 ;
> - <font color = "red">使用C++输入输出更方便，不需要像`printf/scanf`输入输出时那样，需要手动控制格式。C++的输入输出可以自动识别变量类型  </font>

注意:

> 早期标准库将所有功能在全局域中实现，声明在.h后缀的头文件中，使用时只需包含对应头文件即可，后来将其实现在std命名空间下，为了和C头文件区分，也为了正确使用命名空间，规定C++头文件不带.h；旧编译器(vc 6.0)中还支持<iostream.h>格式，后续编译器已不支持，因
> 此推荐使用<iostream>+std的方式。  

## Ⅲ. 缺省参数:

### Ⅲ. Ⅰ 缺省参数的概念:

> 缺省参数是**声明或定义函数时**为函数的参数指定一个缺省值。在调用该函数时，如果没有指定实参则采用该形参的缺省值，否则使用指定的实参  

例如:

> ```c++
> void Func(int a = 0)
> {
> 	cout<<a<<endl;
> }
> int main()
> {
> 	Func(); // 没有传参时，使用参数的默认值
> 	Func(10); // 传参时，使用指定的实参
> 	return 0;
> }
> ```

### Ⅲ. Ⅱ 缺省参数的分类:

> ①全缺省参数 :
>
> ```c++
> void Func(int a = 10, int b = 20, int c = 30)
> {
> 	cout<<"a = "<<a<<endl;
> 	cout<<"b = "<<b<<endl;
> 	cout<<"c = "<<c<<endl;
> }
> ```
>
> ②半缺省参数:
>
> **注意**:
>
> (1)半缺省参数必须**从右往左**依次来给出，不能间隔着给;
> (2) 缺省参数不能在函数声明和定义中同时出现;
>
> <font color = 'red'>所以当函数的声明和定义分离时，我们需要在声明中去给缺省参数，而不能在定义中;</font>
>
> (3) 缺省值必须是常量或者全局变量;
> (4) C语言不支持（编译器不支持 );
>
> ```c++
> //a.h
> void Func(int a = 10);
> // a.cpp
> void Func(int a = 20)
> {}
> // 注意：如果生命与定义位置同时出现，恰巧两个位置提供的值不同，那编译器就无法确定到底该用那个缺省值
> ```

## Ⅳ 函数重载:

### Ⅳ. Ⅰ 函数重载概念:

> 函数重载：是函数的一种特殊情况，C++允许在同一作用域中声明几个功能类似的**同名函数**，<font color = 'blue'>**这些同名函数的形参列表(参数个数 或 类型 或 类型顺序)不同**</font>，常用来处理实现功能类似数据类型不同的问题 ;

## Ⅳ. Ⅱ分类:

> ①参数类型不同:
>
> ```c++
> int Add(int left, int right)
> {
> 	cout << "int Add(int left, int right)" << endl;
> 	return left + right;
> }
> double Add(double left, double right)
> {
> 	cout << "double Add(double left, double right)" << endl;
> 	return left + right;
> }
> ```
>
> ②参数个数不同:
>
> ```c++
> void f()
> {
> 	cout << "f()" << endl;
> }
> void f(int a)
> {
> 	cout << "f(int a)" << endl;
> }
> ```
>
> ③参数的类型顺序不同:
>
> ```c++
> void f(int a, char b)
> {
> 	cout << "f(int a,char b)" << endl;
> }
> void f(char b, int a)
> {
> 	cout << "f(char b, int a)" << endl;
> }
> ```

## Ⅳ. Ⅲ C++支持函数重载的原理--名字修饰(name Mangling)  

> 对于函数重载,C语言是支持的,那么C++在运行的过程中会不会导致运行速度变慢呢？
>
> 这就要解释一下函数重载的原理了！

在C/C++中，一个程序要运行起来，需要经历以下几个阶段：**预处理、编译、汇编、链接;**

①实际项目通常是由多个头文件和多个源文件构成，而通过C语言阶段学习的编译链接，我们可以知道，【当前a.cpp中调用了b.cpp中定义的Add函数时】，编译后链接前，a.o的目标文件中没有Add的函数地址，因为Add是在b.cpp中定义的，所以Add的地址在b.o中。那么怎么办呢？

②所以链接阶段就是专门处理这种问题，链接器看到a.o调用Add，但是没有Add的地址，就会到b.o的符号表中找Add的地址，然后链接到一起。	    

③那么链接时，面对Add函数，链接接器会使用哪个名字去找呢？这里每个编译器都有自己的函数名修饰规则。  

④由于Windows下vs的修饰规则过于复杂，而Linux下g++的修饰规则简单易懂，下面我们使用了`g++`演示了这个修饰后的名字  

⑤通过下面我们可以看出`gcc`的函数修饰后名字不变。而g++的函数修饰后变成【_Z+函数长度+函数名+类型首字母】

- 采用C语言编译器编译后结果 :

> 在linux下，采用gcc编译完成后，函数名字的修饰没有发生改变。  

```c++
00000000004004cd <func>:
  4004cd:	55                   	push   %rbp
  4004ce:	48 89 e5             	mov    %rsp,%rbp
  4004d1:	5d                   	pop    %rbp
  4004d2:	c3                   	retq   

00000000004004d3 <main>:
  4004d3:	55                   	push   %rbp
  4004d4:	48 89 e5             	mov    %rsp,%rbp
  4004d7:	b8 00 00 00 00       	mov    $0x0,%eax
  4004dc:	5d                   	pop    %rbp
  4004dd:	c3                   	retq   
  4004de:	66 90                	xchg   %ax,%ax

```

- 采用C++编译器编译后结果:

> 在linux下，采用g++编译完成后，函数名字的修饰发生改变，编译器将函数参数类型信息添加到修改后的名字中  

```c++
00000000004004fd <_Z4funcv>:
  4004fd:	55                   	push   %rbp
  4004fe:	48 89 e5             	mov    %rsp,%rbp
  400501:	5d                   	pop    %rbp
  400502:	c3                   	retq   

0000000000400503 <main>:
  400503:	55                   	push   %rbp
  400504:	48 89 e5             	mov    %rsp,%rbp
  400507:	b8 00 00 00 00       	mov    $0x0,%eax
  40050c:	5d                   	pop    %rbp
  40050d:	c3                   	retq   
  40050e:	66 90                	xchg   %ax,%ax

```

⑥通过这里就理解了C语言没办法支持重载，因为同名函数没办法区分。而C++是通过函数修饰规则来区分，只要参数不同，修饰出来的名字就不一样，就支持了重载。  

⑦如果两个函数函数名和参数是一样的，返回值不同是不构成重载的，因为调用时编译器没办法区分。  

## Ⅴ 引用:

### Ⅴ. Ⅰ 应用的概念:

> 引用不是新定义一个变量，而是给已存在变量取了一个别名，编译器不会为引用变量开辟内存空间，它和它引用的变量共用同一块内存空间。  

语法:`类型& 引用变量名(对象名) = 引用实体；`

```c++
void TestRef()
{
	int a = 10;
	int& ra = a; //<====定义引用类型
	printf("%p\n", &a);
	printf("%p\n", &ra);
}
```

<font color = 'red'>注意:**引用类型**必须和引用实体是**同种类型**的</font>  

### Ⅴ. Ⅱ引用特性:

> -  引用在定义时必须初始化
> -  一个变量可以有多个引用
> -  引用一旦引用一个实体，再不能引用其他实体  	

### Ⅴ. Ⅲ常引用:

> ```c++
> void TestConstRef()
> {
> 	const int a = 10;
> 	//int& ra = a; // 该语句编译时会出错，a为常量
> 	const int& ra = a;
> 	// int& b = 10; // 该语句编译时会出错，b为常量
> 	const int& b = 10;
> 	double d = 12.34;
> 	//int& rd = d; // 该语句编译时会出错，类型不同
> 	const int& rd = d;
> }
> ```

### Ⅴ. Ⅳ 使用场景:

> ①做参数:
>
> ```c++
> void Swap(int& left, int& right)
> {
> 	int temp = left;
> 	left = right;
> 	right = temp;
> }
> ```
>
>   ②做返回值 
>
> ```c++
> int& Count()
> {
> 	static int n = 0;
> 	n++;
> 	// ...
> 	return n;
> }
> ```
>
> 