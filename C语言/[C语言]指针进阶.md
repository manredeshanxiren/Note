# [C语言]指针进阶

## 1.字符指针：

> 📣在指针的类型中我们知道有一种指针类型为字符指针 char*

一般用法：

```c
int main()
{
		char ch = 'w';
		char *pc = &ch;
		*pc = 'w';
				return 0;
}
```

另一种使用方法：

> 📣代码 const char* pstr = "hello bit."; 这里只是将首字符的地址放到了pstr中

```c
int main()
{
		const char* pstr = "hello bit.";//这里是把一个字符串放到pstr指针变量里了吗？
		printf("%s\n", pstr);
			return 0;
}
```

所以有如下的面试题：

```c
int main()
{
		char str1[] = "hello bit.";
		char str2[] = "hello bit.";
		const char *str3 = "hello bit.";
		const char *str4 = "hello bit.";
			if(str1 ==str2)
			printf("str1 and str2 are same\n");
			else
			printf("str1 and str2 are not same\n");
			if(str3 ==str4)
			printf("str3 and str4 are same\n");
			else
			printf("str3 and str4 are not same\n");
	return 0;
}
```

输出的结果：

```c
str1 and str2 are not same
str3 and str4 are same
```

## 2.指针数组：

> 📣顾名思义存放指针的数组

例如：

```c
int* arr1[10]; //整形指针的数组
char *arr2[4]; //一级字符指针的数组
char **arr3[5];//二级字符指针的数组
```

##  3.数组指针：

> 数组指针的定义：指向数组的指针

下面代码哪个是数组指针？

```c
int *p1[10];
int (*p2)[10];
//p1, p2分别是什么？
```

> - p1则是int型的指针数组
> -  p2则是int型的数组指针
> -  原理是[]的优先级比*高所以不带括号的情况下就会被识别成数组 
> - *带上括号就会和解引用操作符*结合所以就是数组指针了

## 4.&数组名VS数组名:

> 结论：打印地址的话数值上是相同的，实际上是两种不同的类型所以在进行加一时步长不一样；

(1)相同的点：

打印地址的代码：

```c
#include <stdio.h>
int main()
{
		int arr[10] = {0};
		printf("%p\n", arr);
		printf("%p\n", &arr);
return 0;
}
```

不同的点：

> 📣原因是： 
>
> - &arr和arr数值上相同，但是意义却不同 &arr表示的是数组的地址（对应之前的两种特殊情况）。
> -  arr表示的是数组首元素地址。 
> - 所以&arr + 1 跳过的是整个数组&arr + 1 和 &arr 的差值是40

```c
#include <stdio.h>
int main()
{
		int arr[10] = { 0 };
		printf("arr = %p\n", arr);
		printf("&arr= %p\n", &arr);
		printf("arr+1 = %p\n", arr+1);
		printf("&arr+1= %p\n", &arr+1);
	return 0;
}
```

## 5.数组指针的使用:

> 数组指针指向的是数组，那数组指针中存放的应该是数组的地址

```c
#include <stdio.h>
void print_arr1(int arr[3][5], int row, int col)
{
			int i = 0;
			for(i=0; i<row; i++)
			{
					for(j=0; j<col; j++)
					{
						printf("%d ", arr[i][j]);
					}
					printf("\n");
			}
}
void print_arr2(int (*arr)[5], int row, int col)
{
			int i = 0;
			for(i=0; i<row; i++)
			{
					for(j=0; j<col; j++)
					{
						printf("%d ", arr[i][j]);
					}
				printf("\n");
			}
}
int main()
{
			int arr[3][5] = {1,2,3,4,5,6,7,8,9,10};
			print_arr1(arr, 3, 5);
			//数组名arr，表示首元素的地址
			//但是二维数组的首元素是二维数组的第一行
			//所以这里传递的arr，其实相当于第一行的地址，是一维数组的地址
			//可以数组指针来接收
			print_arr2(arr, 3, 5);
		return 0;
}
```

几个例子：

```c
int arr[5];         //整型数组
int *parr1[10];     //指针数组
int (*parr2)[10];   //数组指针
int (*parr3[10])[5];//数组指针的数组
```

## 6.数组参数、指针参数:

### 6.1一维数组传参:

```c
#include <stdio.h>
void test(int arr[])//ok
{}
void test(int arr[10])//ok
{}
void test(int *arr)//ok
{}
void test2(int *arr[20])//ok
{}
void test2(int **arr)//ok
{}
int main()
{
		int arr[10] = {0};
		int *arr2[20] = {0};
		test(arr);
		test2(arr2);
}
```

<u>**数组传参：形参可以是数组，也可以是指针**</u>

### 6.2二维数组传参:

```c
void test(int arr[3][5])//ok
{}
void test(int arr[][])//no
{}
void test(int arr[][5])//ok
{}
//总结：二维数组传参，函数形参的设计只能省略第一个[]的数字。
//因为对一个二维数组，可以不知道有多少行，但是必须知道一行多少元素。
//这样才方便运算。
void test(int *arr)//no     是一级指针，无法接收数组的指针
{}
void test(int* arr[5])//no   是数组，无法接收指针
{}
void test(int (*arr)[5])//ok   数组指针可以接收
{}
void test(int **arr)//no   二级指针用于接收一级指针的地址与arr不匹配所以不行
{}
int main()
{
int arr[3][5] = {0};
test(arr);//因为arr是二维数组，所以arr表示的是第一行的地址
}
```

### 	6.3一级指针传参:

```c

#include <stdio.h>
void print(int *p, int sz)
{
		int i = 0;
		for(i=0; i<sz; i++)
		{
			printf("%d\n", *(p+i));
		}
}
int main()
{
		int arr[10] = {1,2,3,4,5,6,7,8,9};
		int *p = arr;
		int sz = sizeof(arr)/sizeof(arr[0]);
		//一级指针p，传给函数
		print(p, sz);
	return 0;
}
```

> 函数的形参是一级指针我们可以传什么参数：

```c
void test1(int *p)
{}
//test1函数能接收什么参数？

int a = 0;
test(&a);//ok

int * ptr = &a;
test(ptr);//ok

int arr[10];
test(arr);//ok

void test2(char* p)
{}
//test2函数能接收什么参数？
//类似上面
//char型变量的地址
//char型数组名或地址
```

### 6.4二级指针传参:

例如：

```c
#include <stdio.h>
void test(int** ptr)
{
		printf("num = %d\n", **ptr);
}
int main()
{
		int n = 10;
		int*p = &n;
		int **pp = &p;
		test(pp);
		test(&p);
	return 0;
}
```

> 当函数的参数为二级指针的时候，可以接收什么参数？

```c
void test(char **p)
{ }
int main()
{
		char c = 'b';
		char*pc = &c;
		char**ppc = &pc;
		char* arr[10];
		test(&pc);
		test(ppc);
		test(arr);//Ok  因为指针数组的数组名就是首元素地址也就是二级指针
	return 0;
}
```

## 7.函数指针:

首先看一段代码：

```c
#include <stdio.h>
void test()
{
printf("hehe\n");
}
int main()
{
printf("%p\n", test);
printf("%p\n", &test);
return 0;
}
```

![image-20221214125034356](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221214125034356.png)

### 7.1如何保存函数的地址：

> 创建一个函数指针类型：
>
> 形式  ：<u>**返回类型 （* 指针变量名）（函数参数）；**</u>

两段考基础的代码：

```c
//代码1
(*(void (*)())0)();
//代码2
void (*signal(int , void(*)(int)))(int);
```

解释：

```c
//代码1
0 - int
( void (*)() )0 - 把0当做一个函数的地址

( *( void (*)() )0 )();

//把0直接转换成一个void (*)()的函数指针，然后去调用0地址处的函数


//代码2
//上述代码是一次函数声明
//	//声明的函数叫：signal
//	//signal函数的第一个参数是int类型的
//	//signal函数的第二个参数是一个函数指针类型，该函数指针指向的函数参数是int，返回类型是void
//	//signal函数的返回类型也是一个函数指针类型，该函数指针指向的函数参数是int，返回类型是void
```

对代码2的化简：

```c
typedef void(*pfun_t)(int);
pfun_t signal(int, pfun_t);
```

## 8.函数指针数组:

> 即存放函数指针的数组
>
> **如何定义：**
>
> 返回类型 ( * 变量名[x] )（形参）；        // 其中x表示的是数组的大小
>
> 例如 返回值为int 无形参的函数的函数指针数组:
>
> ```c
>  int (*parr1[10])();
> ```

### 8.1应用：

用途是：转移表:

应用示例（计算机）：

```c
#include <stdio.h>
int add(int a, int b)
{
		return a + b;
}
int sub(int a, int b)
{
		return a - b;
}
int mul(int a, int b)
{
		return a*b;
}
int div(int a, int b)
{
		return a / b;
}
int main()
{
		int x, y;
		int input = 1;
		int ret = 0;
		int(*p[5])(int x, int y) = { 0, add, sub, mul, div }; //转移表
while (input)
{
		printf( "*************************\n" );
		printf( " 1:add 2:sub \n" );
		printf( " 3:mul 4:div \n" );
		printf( "*************************\n" );
		printf( "请选择：" );
		scanf( "%d", &input);
			if ((input <= 4 && input >= 1))
			{
					printf( "输入操作数：" );
					scanf( "%d %d", &x, &y);
					ret = (*p[input])(x, y);
			}
			else
					printf( "输入有误\n" );
					printf( "ret = %d\n", ret);
}
return 0;
}
```

## 9.指向函数指针数组的指针：

> 本质上还是一个指针：
>
> 如何定义：
>
> <u>**返回值类型 （\*（*变量名）[数组大小]）（形参）;**</u>

例如：

```c
void test(const char* str)
{
printf("%s\n", str);
}
int main()
{
		//函数指针pfun
		void (*pfun)(const char*) = test;
		//函数指针的数组pfunArr
		void (*pfunArr[5])(const char* str);
		pfunArr[0] = test;
		//指向函数指针数组pfunArr的指针ppfunArr
		void (*(*ppfunArr)[5])(const char*) = &pfunArr;
	return 0;
}
```

## 10.回调函数:

> 回调函数就是<u>**一个通过函数指针调用的函数**</u>。如果你把函数的指针（地址）作为参数传递给另一个
>
> 函数，当这个指针被用来调用其所指向的函数时，我们就说这是回调函数。回调函数不是由该函数
>
> 的实现方直接调用，而是在特定的事件或条件发生时由另外的一方调用的，用于对该事件或条件进
>
> 行响应

示例(快排)：

```c
int swap_int(const void* a, const void* b)
{
    return (*(int*)a) - (*(int*)b);
}
qsort(arr, sizeof(int), sizeof(arr) / sizeof(int), swap_int);
```

