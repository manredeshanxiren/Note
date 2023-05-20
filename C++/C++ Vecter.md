# C++ Vecter

## Ⅰ. vector 的介绍和使用

> ### Ⅰ. Ⅰvector的介绍
>
> [vector文档](https://cplusplus.com/reference/vector/vector/)
>
> ①vector是表示可变大小数组的序列容器。
>
> ②就像数组一样，vector也采用的连续存储空间来存储元素。也就是意味着可以采用下标对vector的元素 进行访问，和数组一样高效。但是又不像数组，它的大小是可以动态改变的，而且它的大小会被容器自 动处理。
>
> ③ 本质讲，vector使用动态分配数组来存储它的元素。当新元素插入时候，这个数组需要被重新分配大小 为了增加存储空间。其做法是，分配一个新的数组，然后将全部元素移到这个数组。就时间而言，这是 一个相对代价高的任务，因为每当一个新的元素加入到容器的时候，vector并不会每次都重新分配大小。
>
> ④vector分配空间策略：vector会分配一些额外的空间以适应可能的增长，因为存储空间比实际需要的存 储空间更大。不同的库采用不同的策略权衡空间的使用和重新分配。但是无论如何，重新分配都应该是 对数增长的间隔大小，以至于在末尾插入一个元素的时候是在常数时间的复杂度完成的。
>
> ⑤ 因此，vector占用了更多的存储空间，为了获得管理存储空间的能力，并且以一种有效的方式动态增长。
>
> ⑥与其它动态序列容器相比（deque, list and forward_list），vector在访问元素的时候更加高效，在末 尾添加和删除元素相对高效。对于其它不在末尾的删除和插入操作，效率更低。比起list和forward_list 统一的迭代器和引用更好。
>
> ### Ⅰ. Ⅱvector的使用
>
> 在`vector`的学习中我们一定要学会查看文档[vector文档](https://cplusplus.com/reference/vector/vector/)，对于vector的使用我们一般只需要熟悉以下常用的接口就可以了！
>
> #### Ⅰ.Ⅱ .Ⅰvector的定义
>
> | ([constructor](https://cplusplus.com/reference/vector/vector/vector/))构造函数声明 | 接口说明                 |
> | ------------------------------------------------------------ | ------------------------ |
> | **<u>vector()</u>**                                          | 无参构造                 |
> | vector（size_type n, const value_type& val = value_type()）  | 构造并初始化n个val       |
> | vector (const vector& x); （重点）                           | 拷贝构造                 |
> | vector (InputIterator first, InputIterator last);            | 使用迭代器进行初始化构造 |
>
> ```cpp
> int TestVector1()
> {
>     // constructors used in the same order as described above:
>     vector<int> first;                                // empty vector of ints
>     vector<int> second(4, 100);                       // four ints with value 100
>     vector<int> third(second.begin(), second.end());  // iterating through second
>     vector<int> fourth(third);                       // a copy of third
> 
>     // 下面涉及迭代器初始化的部分，我们学习完迭代器再来看这部分
>     // the iterator constructor can also be used to construct from arrays:
>     int myints[] = { 16,2,77,29 };
>     vector<int> fifth(myints, myints + sizeof(myints) / sizeof(int));
> 
>     cout << "The contents of fifth are:";
>     for (vector<int>::iterator it = fifth.begin(); it != fifth.end(); ++it)
>         cout << ' ' << *it;
>     cout << '\n';
> 
>     return 0;
> }
> ```
>
> ###  
>
> ### Ⅰ.Ⅱ .Ⅱvector iterator 的使用
>
> vector的迭代器使用和string的迭代器使用是非常类似的。
>
> | iterator的使用                                               | 接口说明                                                     |
> | ------------------------------------------------------------ | ------------------------------------------------------------ |
> | **<u>([begin](https://cplusplus.com/reference/vector/vector/begin/))+([end](https://cplusplus.com/reference/vector/vector/end/))</u>** | 获取第一个数据位置的iterator/const_iterator， 获取最后一个数据的下一个位置的iterator/const_iterator |
> | ([rbegin](https://cplusplus.com/reference/vector/vector/rbegin/)) + ([rend](https://cplusplus.com/reference/vector/vector/rend/)) | 获取最后一个数据位置的reverse_iterator，获取第一个数据前一个位置的 reverse_iterator |
>
> ```cpp
> void PrintVector(const vector<int>& v)
> {
> 	// const对象使用const迭代器进行遍历打印
> 	vector<int>::const_iterator it = v.begin();
> 	while (it != v.end())
> 	{
> 		cout << *it << " ";
> 		++it;
> 	}
> 	cout << endl;
> }
> ```
>
> ### Ⅰ.Ⅱ .Ⅲ空间增长问题
>
> | 容量空间                                                     | 接口说明             |
> | ------------------------------------------------------------ | -------------------- |
> | ([size](https://cplusplus.com/reference/vector/vector/size/)) | 获取数据个数         |
> | ([capacity](https://cplusplus.com/reference/vector/vector/capacity/)) | 获取容量大小         |
> | ([empty](https://cplusplus.com/reference/vector/vector/empty/)) | 判断是否为空         |
> | **([resize](https://cplusplus.com/reference/vector/vector/resize/))** | 改变vector的size     |
> | ([reserve](https://cplusplus.com/reference/vector/vector/reserve/)) | 改变vector的capacity |
>
> - capacity的代码在vs和g++下分别运行会发现，**vs下capacity是按1.5倍增长的，g++是按2倍增长的**。 这个问题经常会考察，不要固化的认为，vector增容都是2倍，具体增长多少是根据具体的需求定义 的。vs是PJ版本STL，g++是SGI版本STL。
>
> - reserve只负责开辟空间，如果确定知道需要用多少空间，reserve可以缓解vector增容的代价缺陷问题。
>
> - resize在开空间的同时还会进行初始化,影响size。
>
>   ```cpp
>   // 如果已经确定vector中要存储元素大概个数，可以提前将空间设置足够
>   // 就可以避免边插入边扩容导致效率低下的问题了
>   void TestVectorExpandOP()
>   {
>    	vector<int> v;
>    	size_t sz = v.capacity();
>    	v.reserve(100); // 提前将容量设置好，可以避免一遍插入一遍扩容
>    	cout << "making bar grow:\n";
>    	for (int i = 0; i < 100; ++i) 
>    	{
>    		v.push_back(i);
>    		if (sz != v.capacity())
>    		{
>    			sz = v.capacity();
>    			cout << "capacity changed: " << sz << '\n';
>    		}
>    	}
>   }
>   ```
>
>   ### Ⅰ.Ⅱ .Ⅳ vector的增删查改
>
>   | vector增删查改                                               | 接口说明                                               |
>   | ------------------------------------------------------------ | ------------------------------------------------------ |
>   | ([push_back](http://www.cplusplus.com/reference/vector/vector/push_back/)) | 尾插                                                   |
>   | ([pop_back](http://www.cplusplus.com/reference/vector/vector/pop_back/)) | 尾删                                                   |
>   | ([find](http://www.cplusplus.com/reference/algorithm/find/?kw=find)) | 查找。（注意这个是算法模块实现，不是vector的成员接口） |
>   | ([insert](https://cplusplus.com/reference/vector/vector/insert/)) | 在position之前插入val                                  |
>   | ([erase](https://cplusplus.com/reference/vector/vector/erase/)) | 删除position位置的数据                                 |
>   | ([swap](https://cplusplus.com/reference/vector/vector/swap/)) | 交换两个vector的数据空间                               |
>   | ([opeerator[]](https://cplusplus.com/reference/vector/vector/operator%5B%5D/)) | 像数组一样访问`[]`的重载                               |
>
>   ```cpp
>   // 尾插和尾删：push_back/pop_back
>   void TestVector4()
>   {
>   	vector<int> v;
>   	v.push_back(1);
>   	v.push_back(2);
>   	v.push_back(3);
>   	v.push_back(4);
>   
>   	auto it = v.begin();
>   	while (it != v.end()) 
>   	{
>   		cout << *it << " ";
>   		++it;
>   	}
>   	cout << endl;
>   
>   	v.pop_back();
>   	v.pop_back();
>   
>   	it = v.begin();
>   	while (it != v.end()) 
>   	{
>   		cout << *it << " ";
>   		++it;
>   	}
>   	cout << endl;
>   }
>   
>   // 任意位置插入：insert和erase，以及查找find
>   // 注意find不是vector自身提供的方法，是STL提供的算法
>   ```
>
>   ### Ⅰ.Ⅱ .Ⅴvector 迭代器失效问题
>
>   **迭代器的主要作用就是让算法能够不用关心底层数据结构，其底层实际就是一个指针，或者是对指针进行了 封装，比如：vector的迭代器就是原生态指针T* 。因此迭代器失效，实际就是迭代器底层对应指针所指向的 空间被销毁了，而使用一块已经被释放的空间，造成的后果是程序崩溃(即如果继续使用已经失效的迭代器， 程序可能会崩溃)。**
>
>   对于vector可能会导致其迭代器失效的操作有：
>
>   ①**会引起其底层空间改变的操作，都有可能是迭代器失效**，比如：resize、reserve、insert、assign、 push_back等。
>
>   ```cpp
>   #include <iostream>
>   using namespace std;
>   #include <vector>
>   int main()
>   {
>    	vector<int> v{1,2,3,4,5,6};
>    
>    	auto it = v.begin();
>    
>    	// 将有效元素个数增加到100个，多出的位置使用8填充，操作期间底层会扩容
>    	// v.resize(100, 8);
>    
>    	// reserve的作用就是改变扩容大小但不改变有效元素个数，操作期间可能会引起底层容量改变
>    	// v.reserve(100);
>    
>    	// 插入元素期间，可能会引起扩容，而导致原空间被释放
>    	// v.insert(v.begin(), 0);
>    	// v.push_back(8);
>    
>    	// 给vector重新赋值，可能会引起底层容量改变
>    	v.assign(100, 8);
>    
>    	/*
>    	出错原因：以上操作，都有可能会导致vector扩容，也就是说vector底层原理旧空间被释放掉，
>   	而在打印时，it还使用的是释放之间的旧空间，在对it迭代器操作时，实际操作的是一块已经被释放的
>   	空间，而引起代码运行时崩溃。
>   	 解决方式：在以上操作完成之后，如果想要继续通过迭代器操作vector中的元素，只需给it重新
>   	赋值即可。
>   	 */
>    while(it != v.end())
>    {
>    	cout<< *it << " " ;
>   	 ++it;
>    }
>    	cout<<endl;
>    	return 0;
>   }
>   ```
>
>   ② 指定位置元素的删除操作--**erase**
>
>   ```cpp
>   #include <iostream>
>   using namespace std;
>   #include <vector>
>   int main()
>   {
>    	int a[] = { 1, 2, 3, 4 };
>    	vector<int> v(a, a + sizeof(a) / sizeof(int));
>    	// 使用find查找3所在位置的iterator
>    	vector<int>::iterator pos = find(v.begin(), v.end(), 3);
>    	// 删除pos位置的数据，导致pos迭代器失效。
>    	v.erase(pos);
>    	cout << *pos << endl; // 此处会导致非法访问
>    	return 0;
>   }
>   ```
>
>   **erase删除pos位置元素后，pos位置之后的元素会往前搬移，没有导致底层空间的改变，理论上讲迭代 器不应该会失效，但是：如果pos刚好是最后一个元素，删完之后pos刚好是end的位置，而end位置是 没有元素的，那么pos就失效了。因此删除vector中任意位置上元素时，vs就认为该位置迭代器失效了。**
>
>   以下代码的功能是删除vector中所有的偶数，请问那个代码是正确的，为什么？
>
>   ```cpp
>   #include <iostream>
>   using namespace std;
>   #include <vector>
>   int main()
>   {
>    	vector<int> v{ 1, 2, 3, 4 };
>    	auto it = v.begin();
>    	while (it != v.end())
>    	{
>    		if (*it % 2 == 0)
>    		v.erase(it);
>    		++it;
>    	}
>    
>    	return 0;
>   }
>   int main()
>   {
>    	vector<int> v{ 1, 2, 3, 4 };
>    	auto it = v.begin();
>    	while (it != v.end())
>    	{
>     		if (*it % 2 == 0)
>    		it = v.erase(it);
>    		else
>    		++it;
>    	}
>    	return 0;
>   }
>   ```
>
>   当然是第二个正确，因为我们在erase的时候有可能会造成迭代器失效，所以我们让it不断的去接收erase返回的迭代器，去更新迭代器，防止迭代器失效问题。
>
>   ③注意：Linux下，g++编译器对迭代器失效的检测并不是非常严格，处理也没有vs下极端。
>
>   ```cpp
>   // 1. 扩容之后，迭代器已经失效了，程序虽然可以运行，但是运行结果已经不对了
>   int main()
>   {
>    	vector<int> v{1,2,3,4,5};
>    	for(size_t i = 0; i < v.size(); ++i)
>    	cout << v[i] << " ";
>    	cout << endl;
>    	auto it = v.begin();
>    	cout << "扩容之前，vector的容量为: " << v.capacity() << endl;
>    	// 通过reserve将底层空间设置为100，目的是为了让vector的迭代器失效 
>    	v.reserve(100);
>    	cout << "扩容之后，vector的容量为: " << v.capacity() << endl;
>    
>    	// 经过上述reserve之后，it迭代器肯定会失效，在vs下程序就直接崩溃了，但是linux下不会
>    	// 虽然可能运行，但是输出的结果是不对的
>    	while(it != v.end())
>    	{
>    		cout << *it << " ";
>    		++it;
>    	}
>    	cout << endl;
>    	return 0;
>   }
>   程序输出：
>   1 2 3 4 5
>   扩容之前，vector的容量为: 5
>   扩容之后，vector的容量为: 100
>   0 2 3 4 5 409 1 2 3 4 5
>   // 2. erase删除任意位置代码后，linux下迭代器并没有失效
>   // 因为空间还是原来的空间，后序元素往前搬移了，it的位置还是有效的
>   #include <vector>
>   #include <algorithm>
>   int main()
>   {
>    	vector<int> v{1,2,3,4,5};
>    	vector<int>::iterator it = find(v.begin(), v.end(), 3);
>    	v.erase(it);
>       cout << *it << endl;
>    	while(it != v.end())
>    	{
>    		cout << *it << " ";
>    		++it;
>    	}
>    	cout << endl;
>    	return 0;
>   }
>   程序可以正常运行，并打印：
>   4
>   4 5
>    
>   // 3: erase删除的迭代器如果是最后一个元素，删除之后it已经超过end
>   // 此时迭代器是无效的，++it导致程序崩溃
>   int main()
>   {
>    	vector<int> v{1,2,3,4,5};
>    	// vector<int> v{1,2,3,4,5,6};
>    	auto it = v.begin();
>    	while(it != v.end())
>    	{
>    		if(*it % 2 == 0)
>    		v.erase(it);
>    		++it;
>    	}
>    	for(auto e : v)
>    	cout << e << " ";
>    	cout << endl;
>    	return 0;
>   }
>   ========================================================
>   // 使用第一组数据时，程序可以运行
>   [sly@VM-0-3-centos 20220114]$ g++ testVector.cpp -std=c++11
>   [sly@VM-0-3-centos 20220114]$ ./a.out
>   1 3 5
>   =========================================================
>   // 使用第二组数据时，程序最终会崩溃
>   [sly@VM-0-3-centos 20220114]$ vim testVector.cpp
>   [sly@VM-0-3-centos 20220114]$ g++ testVector.cpp -std=c++11
>   [sly@VM-0-3-centos 20220114]$ ./a.out
>   Segmentation fault
>   ```
>
>   从上述三个例子中可以看到：SGI STL中，迭代器失效后，代码并不一定会崩溃，但是运行结果肯定不 对，如果it不在begin和end范围内，肯定会崩溃的。
>
>   ④与vector类似，string在插入+扩容操作+erase之后，迭代器也会失效
>
>   ```cpp
>   #include <string>
>   void TestString()
>   {
>    	string s("hello");
>    	auto it = s.begin();
>    	// 放开之后代码会崩溃，因为resize到20会string会进行扩容
>    	// 扩容之后，it指向之前旧空间已经被释放了，该迭代器就失效了
>    	// 后序打印时，再访问it指向的空间程序就会崩溃
>    	//s.resize(20, '!');
>    	while (it != s.end())
>    	{
>    		cout << *it;
>    		++it;
>    	}
>    	cout << endl;
>    	it = s.begin();
>    	while (it != s.end())
>    	{
>    		it = s.erase(it);
>    		// 按照下面方式写，运行时程序会崩溃，因为erase(it)之后
>    		// it位置的迭代器就失效了
>    		// s.erase(it); 
>    		++it;
>    	}
>   }
>   ```

## Ⅱ. vector深度剖析及模拟实现

> ### Ⅱ . Ⅰ std::vector的核心框架接口的模拟实现xupt::vector
>
> [vector的模拟实现](https://github.com/manredeshanxiren/learn_c/blob/main/Vecter/vector.h)
>
> ### Ⅱ. Ⅱ使用memcpy拷贝问题
>
> 假设模拟实现的vector中的reserve接口中，使用memcpy进行的拷贝，以下代码会发生什么问题？
>
> ```cpp
> int main()
> {
>  	bite::vector<bite::string> v;
>  	v.push_back("1111");
>  	v.push_back("2222");
>  	v.push_back("3333");
>  	return 0;
> }
> 
> ```
>
> 问题分析：
>
> ​	①memcpy是内存的二进制格式拷贝，将一段内存空间中内容原封不动的拷贝到另外一段内存空间中
>
> ​	②如果拷贝的是非自定义类型的元素，memcpy既高效又不会出错，但如果拷贝的是自定义类型元素，并且自**定义类型元素中涉及到资源管理时，就会出错**，因为memcpy的拷贝实际是浅拷贝。
>
> ### Ⅱ. Ⅲ 动态二维数组理解
>
> ```cpp
> // 以杨辉三角的前n行为例：假设n为5
> void test2vector(size_t n)
> {
>  	// 使用vector定义二维数组vv，vv中的每个元素都是vector<int>
>  	xupt::vector<xupt::vector<int>> vv(n);
>  
>  	// 将二维数组每一行中的vecotr<int>中的元素全部设置为1
>  	for (size_t i = 0; i < n; ++i)
>  	vv[i].resize(i + 1, 1);
>  	// 给杨辉三角出第一列和对角线的所有元素赋值
>  	for (int i = 2; i < n; ++i)
>  	{
>  		for (int j = 1; j < i; ++j)
>  		{
>  			vv[i][j] = vv[i - 1][j] + vv[i - 1][j - 1];
>  		}
>  	}
> }
> ```
>
> `xupt::vector<xupt vector<int>> vv(n);`构造一个vv动态二维数组，vv中总共有n个元素，每个元素都是vector类 型的，每行没有包含任何元素，如果n为5时如下所示：
>
> ![image-20230430231055536](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230430231055536.png)
>
> vv中元素填充完成之后，如下图所示：
>
> ![image-20230430232236054](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230430232236054.png)
>
> 使用标准库中vector构建动态二维数组时与上图实际是一致的。