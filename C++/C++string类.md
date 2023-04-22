# C++string类

[toc]

------

## Ⅰ. 为什么要学习string类

> 首先我们在C语言中有字符串的概念，是一些以`\0`结尾的字符的集合，而且C语言中也有很多和字符串相关的函数例如：`strstr`等，但是C语言中是将这些函数和字符串是分离开来的，这不符面向对象变成的思想，而且空间管理是交给用户自己来处理，非常容易出现越界问题。所以在C++中将字符串和字符串相关的函数集成在一个类里面，提高编程效率。

## Ⅱ. 标准库中的string类

### Ⅱ. Ⅰ. string类的了解

> 这里首先附上string类的文档[string - C++ Reference (nonplussing)](https://legacy.cplusplus.com/reference/string/string/?kw=string)
>
> - 字符串是表示字符序列的类
>
> - 标准的字符串类提供了对此类对象的支持，其接口类似于[标准字容器](https://legacy.cplusplus.com/reference/stl/)的接口，但添加了专门用于操作 单字节字符字符串的设计特性
>
> - string类是使用char(即作为它的字符类型，使用它的默认[char_traits](https://legacy.cplusplus.com/reference/string/char_traits/)和[allocator(分配器)](https://legacy.cplusplus.com/reference/memory/allocator/)类型(关于模板的更多信 息，请参阅[basic_string](https://legacy.cplusplus.com/reference/string/basic_string/))
>
> - string类是[basic_string](https://legacy.cplusplus.com/reference/string/basic_string/)模板类的一个实例，它使用char来实例化[basic_string](https://legacy.cplusplus.com/reference/string/basic_string/)模板类，并用[char_traits](https://legacy.cplusplus.com/reference/string/char_traits/) 和[allocator(分配器)](https://legacy.cplusplus.com/reference/memory/allocator/)作为[basic_string](https://legacy.cplusplus.com/reference/string/basic_string/)的默认参数(根于更多的模板信息请参考[basic_string](https://legacy.cplusplus.com/reference/string/basic_string/))。
>
> - 注意，这个类独立于所使用的编码来处理字节:如果用来处理多字节或变长字符(如UTF-8[^1])的序列，这个类的所有成员(如长度或大小)以及它的迭代器，将仍然按照字节(而不是实际编码的字符)来操作
>
>   总结：
>
>   - string是表示字符串的字符串类
>   - 该类的接口与常规容器的接口基本相同，再添加了一些专门用来操作string的常规操作
>   - string在底层实际是：basic_string模板类的别名，typedef basic_string  string
>   - 不能操作多字节或者变长字符的序列
>
>   -----
>
>   [^1]:是一种字符编码方式详细了解可查看这个文章[Unicode、UTF-8、UTF-16 终于懂了 - 知乎 ](https://zhuanlan.zhihu.com/p/427488961)

### Ⅱ. Ⅱ. string类中常用接口说明

> ①string类对象的常见构造：
>
> 当然库中不止以下这些可以点击下面的`constuct`链接详细阅读。
>
> | ([construct](https://legacy.cplusplus.com/reference/string/string/string/))函数名称 | 功能说明                                                     |
> | :----------------------------------------------------------- | :----------------------------------------------------------- |
> | **string()**                                                 | Constructs an [empty](https://legacy.cplusplus.com/string::empty) string, with a [length](https://legacy.cplusplus.com/string::length) of zero characters. |
> | string (const string& str);                                  | Constructs a copy of *str*.(拷贝构造)                        |
> | string (size_t n, char c);                                   | Fills the string with *n* consecutive copies of character *c*. |
> | string (const char* s);                                      | Copies the null-terminated character sequence (C-string) pointed by *s* |
>
> 例如：
>
> ```cpp
> void Teststring()
> {
> 	string s1; // 构造空的string类对象s1
> 	string s2("hello bit"); // 用C格式字符串构造string类对象s2
> 	string s3(s2); // 拷贝构造s3
> }
> ```
>
> ②string类对象的容量操作：
>
> | 函数名称                                                     | 功能说明                                                     |
> | :----------------------------------------------------------- | :----------------------------------------------------------- |
> | **[size](https://legacy.cplusplus.com/reference/string/string/size/)** | Returns the length of the string, in terms of bytes（我们更常用这个相比于length[^2]） |
> | [length](https://legacy.cplusplus.com/reference/string/string/length/) | Returns the length of the string, in terms of bytes          |
> | [capacity](https://legacy.cplusplus.com/reference/string/string/capacity/) | Returns the size of the storage space currently allocated for the [string](https://legacy.cplusplus.com/string), expressed in terms of bytes |
> | [reserve](https://legacy.cplusplus.com/reference/string/string/reserve/) | Requests that the [string capacity](https://legacy.cplusplus.com/string::capacity) be adapted to a planned change in [size](https://legacy.cplusplus.com/string::size) to a [length](https://legacy.cplusplus.com/string::length) of **<u>up to</u>** *n* characters |
> | [clear](https://legacy.cplusplus.com/reference/string/string/clear/) | Erases the contents of the [string](https://legacy.cplusplus.com/string), which becomes an [empty string](https://legacy.cplusplus.com/string::empty) (with a [length](https://legacy.cplusplus.com/string::length) of `0` characters). |
> | [resize](https://legacy.cplusplus.com/reference/string/string/resize/) | Resizes the string to a [length](https://legacy.cplusplus.com/string::length) of *n* characters. |
> | [empty](https://legacy.cplusplus.com/reference/string/string/empty/) | Returns whether the [string](https://legacy.cplusplus.com/string) is empty (i.e. whether its [length](https://legacy.cplusplus.com/string::length) is `0`). |
>
> 注意：
>
> - [^2]:size()与length()方法底层实现原理完全相同，引入size()的原因是为了与其他容器的接口保持一 致，一般情况下基本都是用size()。
>
> - clear()只是将string中有效字符清空，不改变底层空间大小。
>
> - resize(size_t n) 与 resize(size_t n, char c)都是将字符串中有效字符个数改变到n个，不同的是当字 符个数增多时：resize(n)用0来填充多出的元素空间，resize(size_t n, char c)用字符c来填充多出的 元素空间。注意：resize在改变元素个数时，如果是将元素个数增多，可能会改变底层容量的大 小，如果是将元素个数减少，底层空间总大小不变。
>
> - reserve(size_t res_arg = 0)：为string预留空间，不改变有效元素个数，当reserve的参数小于 string的底层空间总大小时，reserver不会改变容量大小
>
>
> ```cpp
> void Teststring1()
> {
> 	// 注意：string类对象支持直接用cin和cout进行输入和输出
> 	string s("hello, bit!!!");
> 	cout << s.size() << endl;
> 	cout << s.length() << endl;
> 	cout << s.capacity() << endl;
> 	cout << s << endl;
> 
> 	// 将s中的字符串清空，注意清空时只是将size清0，不改变底层空间的大小
> 	s.clear();
> 	cout << s.size() << endl;
> 	cout << s.capacity() << endl;
> 
> 	// 将s中有效字符个数增加到10个，多出位置用'a'进行填充
> 	// “aaaaaaaaaa”
> 	s.resize(10, 'a');
> 	cout << s.size() << endl;
> 	cout << s.capacity() << endl;
> 
> 	// 将s中有效字符个数增加到15个，多出位置用缺省值'\0'进行填充
> 	// "aaaaaaaaaa\0\0\0\0\0"
> 	// 注意此时s中有效字符个数已经增加到15个
> 	s.resize(15);
> 	cout << s.size() << endl;
> 	cout << s.capacity() << endl;
> 	cout << s << endl;
> 
> 	// 将s中有效字符个数缩小到5个
> 	s.resize(5);
> 	cout << s.size() << endl;
> 	cout << s.capacity() << endl;
> 	cout << s << endl;
> }
> 
> ```
>
> ③string类对象的访问及遍历操作
>
> | 函数名称                                                     | 功能说明                                                     |
> | :----------------------------------------------------------- | :----------------------------------------------------------- |
> | [operator[ ]](https://legacy.cplusplus.com/reference/string/string/operator[]/) | Returns a reference to the character at position *pos* in the [string](https://legacy.cplusplus.com/string). |
> | [begin](https://legacy.cplusplus.com/reference/string/string/begin/) + [end](https://legacy.cplusplus.com/reference/string/string/end/) | begin: Returns an iterator pointing to the **<u>first</u>** character of the string. end: Returns an iterator pointing to the ***<u>past-the-end</u>*** character of the string. |
> | [rbegin](https://legacy.cplusplus.com/reference/string/string/rbegin/) + [rend](https://legacy.cplusplus.com/reference/string/string/rend/) | rbegin: Returns a ***reverse iterator*** pointing to the **last character** of the string. rend: Returns a ***reverse iterator*** pointing to the theoretical element **preceding the first character** of the string |
> | 范围for                                                      | C++11支持更简洁的范围for的新遍历方式(底层还是迭代器(iterator)) |
>
> 范围for例子：
>
> ```c++
> int arr[5] = {10, 20, 30, 40, 50};
> for (int x : arr) {
>     // 打印每一个元素
>     std::cout << x << " ";
> }
> ```
>
> 访问元素及遍历例子：
>
> ```cpp
> 	string s("hello xupt");
> 	// 3种遍历方式：
> 	// 需要注意的以下三种方式除了遍历string对象，还可以遍历是修改string中的字符，
> 	// 另外以下三种方式对于string而言，第一种使用最多
> 	// 1. for+operator[]
> 	for (size_t i = 0; i < s.size(); ++i)
> 		cout << s[i] << endl;
> 
> 	// 2.迭代器
> 	string::iterator it = s.begin();
> 	while (it != s.end())
> 	{
> 		cout << *it << endl;
> 		++it;
> 	}
> 
> 	// string::reverse_iterator rit = s.rbegin();
> 	// C++11之后，直接使用auto定义迭代器，让编译器推到迭代器的类型
> 	auto rit = s.rbegin();
> 	while (rit != s.rend())
> 		cout << *rit << endl;
> 
> 	// 3.范围for
> 	for (auto ch : s)
> 		cout << ch << endl;
> ```
>
> ④string类对象的修改操作：
>
> | 函数名称                                                     | 功能说明                                                     |
> | ------------------------------------------------------------ | ------------------------------------------------------------ |
> | [push_back](https://legacy.cplusplus.com/reference/string/string/push_back/) | Appends character *c* to the end of the [string](https://legacy.cplusplus.com/string), increasing its [length](https://legacy.cplusplus.com/string::length) by one. |
> | [append](https://legacy.cplusplus.com/reference/string/string/append/) | Extends the [string](https://legacy.cplusplus.com/string) by appending additional characters at the end of its current value： |
> | **[operator+=](https://legacy.cplusplus.com/reference/string/string/operator+=/)** | Extends the [string](https://legacy.cplusplus.com/string) by appending additional characters at the end of its current value: |
> | **[c_str](https://legacy.cplusplus.com/reference/string/string/c_str/)** | Returns a pointer to an array that contains a null-terminated sequence of characters (i.e., a C-string) representing the current value of the [string](https://legacy.cplusplus.com/string) object. |
> | **[find ](https://legacy.cplusplus.com/reference/string/string/find/)+ [npos](https://legacy.cplusplus.com/reference/string/string/npos/)** | find: Searches the [string](https://legacy.cplusplus.com/string) for the first occurrence of the sequence specified by its arguments. npos :**Maximum value for size_t**  `static const size_t npos = -1;` |
> | [rfind](https://legacy.cplusplus.com/reference/string/string/rfind/) | Searches the [string](https://legacy.cplusplus.com/string) for the **last occurrence** of the sequence specified by its arguments. |
> | [substr](https://legacy.cplusplus.com/reference/string/string/substr/) | Returns a newly constructed [string](https://legacy.cplusplus.com/string) object with its value initialized to a copy of a substring of this object. |
>
> 注意：
>
> - 在string尾部追加字符时，s.push_back(c) / s.append(1, c) / s += 'c'三种的实现方式差不多，一般 情况下string类的+=操作用的比较多，+=操作不仅可以连接单个字符，还可以连接字符串。
> - 对string操作时，如果能够大概预估到放多少字符，**可以先通过reserve把空间预留好**(因为扩容是需要一定的时间代价的，提前扩容的话，就可以避免反复扩容的情况，从而达到节省时间的目的)。
>
> ```c++
> 	string str;
> 	str.push_back(' ');   // 在str后插入空格
> 	str.append("hello");  // 在str后追加一个字符"hello"
> 	str += 'b';           // 在str后追加一个字符'b'   
> 	str += "it";          // 在str后追加一个字符串"it"
> 	cout << str << endl;
> 	cout << str.c_str() << endl;   // 以C语言的方式打印字符串
> 
> 	// 获取file的后缀
> 	string file("string.cpp");
> 	size_t pos = file.rfind('.');
> 	string suffix(file.substr(pos, file.size() - pos));
> 	cout << suffix << endl;
> 
> 	// npos是string里面的一个静态成员变量
> 	// static const size_t npos = -1;
> 
> 	// 取出url中的域名
> 	string url("http://www.cplusplus.com/reference/string/string/find/");
> 	cout << url << endl;
> 	size_t start = url.find("://");
> 	if (start == string::npos)
> 	{
> 		cout << "invalid url" << endl;
> 		return;
>     }
> ```
>
> ⑤string类非成员函数
>
> | 函数                                                         | 功能说明                                                     |
> | ------------------------------------------------------------ | ------------------------------------------------------------ |
> | [operator+](https://legacy.cplusplus.com/reference/string/string/operator+/)[^4] | Returns a newly constructed [string](https://legacy.cplusplus.com/string) object with its value being the concatenation of the characters in *lhs*[^3] followed by those of *rhs*.[^3] |
> | **[operator>>](https://legacy.cplusplus.com/reference/string/string/operator>>/)** | Extracts a string from the input stream *is*, storing the sequence in *str*, which is overwritten (**the previous value of *str* is replaced**). |
> | **[operator<<](https://legacy.cplusplus.com/reference/string/string/operator<</)** | Inserts the sequence of characters that conforms value of *str* into *os*. |
> | [getline](https://legacy.cplusplus.com/reference/string/string/getline/) | Extracts characters from *is* and stores them into *str* until the delimitation character *delim* is found (or the newline character, `'\n'`, for *(2)*). |
> | **[relational operators ](https://legacy.cplusplus.com/reference/string/string/operators/)** | Performs the appropriate comparison operation between the [string](https://legacy.cplusplus.com/string) objects *lhs* and *rhs*. |
>
> ------
>
> [^4]:尽量少用，因为传值返回，会导致拷贝效率低
> [^3]:分别是运算符左侧和右侧的值
>
> ⑥vs和g++下string结构的说明
>
> 注意：下述结构都是在32位平台下验证的。
>
> - vs下的`string`结构:
>
>   string总共占28个字节，内部结构稍微复杂一点，先是有一个联合体，联合体用来定义string中字符串的存储空间：
>
>   - **当字符串长度小于16时，使用内部固定的字符数组来存放**
>   - **当字符串长度大于等于16时，从堆上开辟空间**
>
>   ```cPP
>   union _Bxty
>   { 	// storage for small buffer or pointer to larger one
>    	value_type _Buf[_BUF_SIZE];
>    	pointer _Ptr;
>    	char _Alias[_BUF_SIZE]; // to permit aliasing
>   } _Bx;
>   
>   ```
>
>   这种设计也是有一定道理的，大多数情况下字符串的长度都小于16，那string对象创建好之后，内 部已经有了16个字符数组的固定空间，不需要通过堆创建，效率高。 
>
>   其次：还有**一个size_t字段保存字符串长度，一个size_t字段保存从堆上开辟空间总的容量** 
>
>   最后：还**有一个指针**做一些其他事情。
>
>   故总共占16+4+4+4=28个字节。
>
> - g++下string的结构
>
>   G++下，string是通过写时拷贝实现的，string对象总共占4个字节，内部只包含了一个指针，该指 针将来指向一块堆空间，内部包含了如下字段：
>
>   - 空间总大小
>   - 字符串有效长度
>   - 引用计数
>
>   ```cpp
>   struct _Rep_base
>   {
>    size_type _M_length;
>    size_type _M_capacity;
>    _Atomic_word _M_refcount;
>   };
>   ```
>
>   - 指向堆空间的指针，用来存储字符串.
