# [C语言]指针初阶

## 1.指针def:

> 📣指针就是地址，口语中说的指针通常指的是指针变量

## 2.指针变量：

> 📣我们可以通过&（取地址操作符）取出变量的内存其实地址，把地址可以存放到一个变量中，这个变量就是指针变量

## 3.指针和内存的关系：

> 每一个字节（一个基本的内存单元）都有一个地址，在32位的机器上指针变量的大小就是4个字节，因为32位的二进制数字只能编址：4 GB原理如下：
>
> （2 ^ 32 Byte == 2^32/1024 KB ==2^32 / 1024 / 1024 MB ==2^32/1024/1024/1024 GB == 4 GB） 4 G的空间进行编址。
>
> 如果是在64位机器上，指针变量的大小就是8个字节，八个字节的二进制数字可以编址的内存空间大小是32位机器的2的32次方倍；

## 4.指针和指针类型:

> 📣指针变量的大小都是相同的，为什么还要分很多种类： 决定了指针类型加减整数，指针类型解引用可以操作的内存空间大小

```c
char  *pc = NULL;
int   *pi = NULL;
short *ps = NULL;
long  *pl = NULL;
float *pf = NULL;
double *pd = NULL;
```

### 4.1指针类型：

> 指针类决定了指针进行解引用操作的时候，访问几个字节(权限)
>
> 例如:
>
> ```c
> char*   //char类型指针每次解引用访问只访问一个字节
> int*    //int类型指针每次解引用访问只访问四个字节
> double* //double类型指针每次解引用访问只访问八个字节
> ```

## 5.指针+-整数:

> 📣指针的类型决定了指针向前或者向后走一步有多大（距离）

```c
#include <stdio.h>
//演示实例
int main()
{
 int n = 10;
 char *pc = (char*)&n;
 int *pi = &n;
 
 printf("%p\n", &n);
 printf("%p\n", pc);
 printf("%p\n", pc+1);
 printf("%p\n", pi);
 printf("%p\n", pi+1);
 return  0; }
```

## 6.指针的解引用:

> 📣指针的类型决定了，对指针解引用的时候有多大的权限（能操作几个字节）。 比如： char* 的指针解引用就只能访问一个字节，而 int* 的指针的解引用就能访问四个字节。

```c
//演示实例
#include <stdio.h>
int main()
{
 int n = 0x11223344;
 char *pc = (char *)&n;
 int *pi = &n;
 *pc = 0;   //重点在调试的过程中观察内存的变化。
 *pi = 0;   //重点在调试的过程中观察内存的变化。
 return 0; }
```

## 7.野指针：

> 📣野指针就是指针指向的位置是不可知的（随机的、不正确的、没有明确限制的）

### 7.1成因：

(1)指针未初始化：

```c
#include <stdio.h>
int main()
{ 
 int *p;//局部变量指针未初始化，默认为随机值
    *p = 20;
 return 0; }
```

(2)指针越界访问：

```c
#include <stdio.h>
int main()
{
    int arr[10] = {0};
    int *p = arr;
    int i = 0;
    for(i=0; i<=11; i++)
   {
        //当指针指向的范围超出数组arr的范围时，p就是野指针
        *(p++) = i;
   }
    return 0; }
```

(3)指针指向的空间释放;

### 7.2如何规避野指针:

> 1. 指针初始化
> 2. 小心指针越界
> 3. 指针指向空间释放，及时置NULL
> 4. 避免返回局部变量的地址
> 5. 指针使用之前检查有效性

例如：

```c
#include <stdio.h>
int main()
{
    int *p = NULL;
    //....
    int a = 10;
    p = &a;
    if(p != NULL)
   {
        *p = 20;
   }
    return 0; }
```

## 8.指针运算:

> 1. 指针+- 整数 
> 2. 指针-指针     //指针+指针不可以（进阶会解释）
> 3.  指针的关系运算

### 8.1指针+-整数：

```cc
#define N_VALUES 5
float values[N_VALUES];
float *vp;
//指针+-整数；指针的关系运算
for (vp = &values[0]; vp < &values[N_VALUES];)
{
     *vp++ = 0; 
}
```

### 8.2指针-指针:

```c
int my_strlen(char *s) {
       char *p = s;
       while(*p != '\0' )
              p++;
       return p-s; }
```

### 8.3指针的关系运算：

> 📣指针可以比较大小

```
for(vp = &values[N_VALUES]; vp > &values[0];)
{
    *--vp = 0; 
}

for(vp = &values[N_VALUES-1]; vp >= &values[0];vp--) 
{
    *vp = 0; 
}
```

> 📣标准规定：
>
> 允许<u>**指向数组元素的指针**</u>与<u>**指向数组最后一个元素后面的那个内存位置的指针**</u>比较，但是不允许与 **<u>指向第一个元素之前的那个内存位置的指针</u>**进行比较。

## 9.指针和数组：

### 9.1数组和指针的关系：

> 📣不同：
>
> - 数组和指针不是一个东西
> - 数组能够存放一组数，连续的空间，数组的大小取决于元素个数
> - 指针是一个变量，是存放地址的，4/8个字节
>
> 📣联系：
>
> - 数组名是地址(指针)
> - 数组把首元素的地址，交给一个指针变量后可以通过指针变量来访问数组

指针和数组关系应用示例：

```c
#include <stdio.h>
int main()
{
    int arr[] = {1,2,3,4,5,6,7,8,9,0};
    int *p = arr; //指针存放数组首元素的地址
    int sz = sizeof(arr)/sizeof(arr[0]);
    for(i=0; i<sz; i++)
   {
        printf("&arr[%d] = %p   <====> p+%d = %p\n", i, &arr[i], i, p+i);
   }
    return 0; }
```

![image-20221214122058161](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221214122058161.png)

## 10.指针数组：

> 📣def：存放指针的数组；

两种示例：

1.实现多个变量的指针的存储

```c
int main()
{
	int a = 10;
	int b = 20;
	int c = 30;
	int d = 40;
	int e = 50;

	int* arr3[5] = {&a, &b, &c, &d, &e};//存放整型指针的数组
	int i = 0;
	for (i = 0; i < 5; i++)
	{
		printf("%d ", *(arr3[i]));
	}

	return 0;
}
```

2.模拟实现二维数组

```c
int main()
{
	//用一维数组模拟一个二维数组
	int arr1[] = { 1,2,3,4,5 };
	int arr2[] = { 2,3,4,5,6 };
	int arr3[] = { 3,4,5,6,7 };
	int arr4[] = { 4,5,6,7,8 };

	int* arr[4] = {arr1, arr2, arr3, arr4};
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		int j = 0;
		for (j = 0; j < 5; j++)
		{
			printf("%d ", *(*(arr+i)+j));
		}
		printf("\n");
	}
```

​	[C语言](https://blog.csdn.net/weixin_61766635/category_11931072.html?spm=1001.2014.3001.5482)