# C++内存管理



## Ⅰ. C/C++内存分布

> ![image-20230414190031724](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230414190031724.png)
>
> 
>
> 
>
> ![image-20230414185957610](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230414185957610.png)

面试题：

①malloc/calloc/realloc的区别？

- **函数malloc不能初始化所分配的内存空间,而函数calloc能**.
- **函数calloc() 会将所分配的内存空间中的每一位都初始化为零**
- 函数malloc向系统申请分配指定size个字节的内存空间.**返回类型是 void\*类型.void\*表示未确定类型的指针**.C,C++规定，void* 类型可以强制转换为任何其它类型的指针.
- **realloc可以对给定的指针所指的空间进行扩大或者缩小，无论是扩张或是缩小，原有内存的中内容将保持不变**

② malloc的实现原理？

[(5条消息) malloc函数实现原理！_malloc是怎么实现的_YEDITABA的博客-CSDN博客](https://blog.csdn.net/yeditaba/article/details/53443792)

### Ⅰ. Ⅰnew/delete操作内置类型

> ```c++
> void Test()
> {
>   // 动态申请一个int类型的空间
>   int* ptr4 = new int;
>   
>   // 动态申请一个int类型的空间并初始化为10
>   int* ptr5 = new int(10);
>   
>   // 动态申请10个int类型的空间
>   int* ptr6 = new int[3];
>   delete ptr4;
>   delete ptr5;
>   delete[] ptr6;
> }
> 
> ```
>
> 注意：申请和释放单个元素的空间，**使用new和delete操作符**，申请和释放连续的空间，使用 new[]和delete[]，注意：<font color = red>**匹配起来使用**。</font>
>
> 对于必须要匹配的情况是因为对于自定义类型来说需要调用析构函数那么`new`在申请空间的时候就会在这空间的头部之前多申请一个4字节的空间用于存储需要调用析构函数的次数例如：
>
> ![image-20230414235724746](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230414235724746.png)





### Ⅰ. Ⅱ new和delete操作自定义类型

> ```C++
> class A
> {
> 	public:
>  		A(int a = 0)
>  		: _a(a)
>  		{
>  			cout << "A():" << this << endl;
> 		}
>  		~A()
>  		{
>  			cout << "~A():" << this << endl;
>  		}
> 	private:
>  		int _a;
> };
> int main()
> {
>  	// new/delete 和 malloc/free最大区别是 new/delete对于【自定义类型】除了开空间
> 	//还会调用构造函数和析构函数
>  	A* p1 = (A*)malloc(sizeof(A));
>  	A* p2 = new A(1);
>  	free(p1);
>  	delete p2;
>  	// 内置类型是几乎是一样的
>  	int* p3 = (int*)malloc(sizeof(int)); // C
>  	int* p4 = new int;
> 	free(p3);
> 	delete p4;
>  	A* p5 = (A*)malloc(sizeof(A)*10);
>  	A* p6 = new A[10];
>  	free(p5);
>  	delete[] p6;
>  return 0;
> }
> 
> ```
>
> 注意：在申请自定义类型的空间时，**new会调用构造函数，delete会调用析构函数**，而malloc与 free不会。



### Ⅰ . Ⅲ <u>operator new与operator delete函数</u>

> new和delete是用户进行**动态内存申请和释放的操作符**，operator new 和operator delete**是系统提供的全局函数**，new在底层调用operator new全局函数来申请空间，delete在底层通过 operator delete全局函数来释放空间
>
> ```cpp
> /*
> operator new：该函数实际通过malloc来申请空间，当malloc申请空间成功时直接返回；申请空间
> 失败，尝试执行空间不足应对措施，如果改应对措施用户设置了，则继续申请，否则抛异常。
> */
> void *__CRTDECL operator new(size_t size) _THROW1(_STD bad_alloc)
> {
> // try to allocate size bytes
> void *p;
> while ((p = malloc(size)) == 0)
>      if (_callnewh(size) == 0)
>      {
>          // report no memory
>          // 如果申请内存失败了，这里会抛出bad_alloc 类型异常
>          static const std::bad_alloc nomem;
>          _RAISE(nomem);
>      }
> return (p);
> }
> /*
> operator delete: 该函数最终是通过free来释放空间的
> */
> void operator delete(void *pUserData)
> {
>      _CrtMemBlockHeader * pHead;
>      RTCCALLBACK(_RTC_Free_hook, (pUserData, 0));
>      if (pUserData == NULL)
>          return;
>      _mlock(_HEAP_LOCK);  /* block other threads */
>      __TRY
>          /* get a pointer to memory block header */
>          pHead = pHdr(pUserData);
>           /* verify block type */
>          _ASSERTE(_BLOCK_TYPE_IS_VALID(pHead->nBlockUse));
>          _free_dbg( pUserData, pHead->nBlockUse );
>      __FINALLY
>          _munlock(_HEAP_LOCK);  /* release other threads */
>      __END_TRY_FINALLY
>      return;
> }
> /*
> free的实现
> */
> #define   free(p)               _free_dbg(p, _NORMAL_BLOCK)
> 
> ```
>
> 通过上述两个全局函数的实现知道，**operator new 实际也是通过malloc来申请空间，**如果 malloc申请空间成功就直接返回，否则执行用户提供的空间不足应对措施，如果用户提供该措施 就继续申请，否则就抛异常。**operator delete 最终是通过free来释放空间的。**

### Ⅰ . Ⅳ new和delete的实现原理

> #### Ⅰ . Ⅳ .Ⅰ**内置类型：**
>
> 如果申请的是内置类型的空间，new和malloc，delete和free基本类似，不同的地方是： 
>
> new/delete申请和释放的是单个元素的空间，new[]和delete[]申请的是连续空间，而且new在申 请空间失败时会抛异常，malloc会返回NULL。
>
> #### Ⅰ . Ⅳ .Ⅱ**自定义类型：**
>
> - **new的原理**
>
>   ①调用operator new函数申请空间’
>
>   ②在申请的空间上执行构造函数，完成对象的构造
>
> - **delete的原理**
>
>   ①在空间上执行析构函数，完成对象中资源的清理工作
>
>   ②调用operator delete函数释放对象的空间
>
> - **new T[N]的原理**
>
>   ①调用operator new[]函数，在operator new[]中实际调用operator new函数完成N个对 象空间的申请
>
>   ②在申请的空间上执行N次构造函数
>
> - **delete[]的原理**
>
>   ①在释放的对象空间上执行N次析构函数，完成N个对象中资源的清理
>
>   ②调用operator delete[]释放空间，实际在operator delete[]中调用operator delete来释 放空间

### Ⅰ. Ⅴ 定位new表达式(placement-new)

> 定位new表达式**是在已分配的原始内存空间中调用构造函数初始化一个对象**。
>
> 使用格式： 
>
> **new (place_address) type**或者**new (place_address) type(initializer-list)**，其中 **place_address必须是一个指针**，**initializer-list是类型的初始化列表** 
>
> 使用场景： 
>
> 定位new表达式在实际中一般是配合内存池使用。因为内存池分配出的内存没有初始化，所以如 果是自定义类型的对象，需要使用new的定义表达式进行显示调构造函数进行初始化。
>
> ![image-20230415080851511](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230415080851511.png)
>
> ```C++
> class A
> {
> 	public:
>  		A(int a = 0)
>  		: _a(a)
>  		{
>  			cout << "A():" << this << endl;
>  		}
>  		~A()
>  		{
>  			cout << "~A():" << this << endl;
>  		}
> 	private:
>  		int _a;
> };
> // 定位new/replacement new
> int main()
> {
>  	// p1现在指向的只不过是与A对象相同大小的一段空间，还不能算是一个对象，因为构造函数没有执行
>  	A* p1 = (A*)malloc(sizeof(A));
>  	new(p1)A;  // 注意：如果A类的构造函数有参数时，此处需要传参
>  	p1->~A();
>  	free(p1);
>  	A* p2 = (A*)operator new(sizeof(A));
>  	new(p2)A(10);
>  	p2->~A();
>  	operator delete(p2);
>   return 0;
> }
> 
> ```