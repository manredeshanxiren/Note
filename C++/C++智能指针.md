# C++智能指针

## 1.为什么要存在只能指针

> 因为一系列的原因我们在C++中引入了异常的概念，那么在异常的处理过程中，程序的执行会不断的乱跳，那么有时候就会造成已经申请的内存而没有进行释放的情况。所以我们需要适配这样的使用场景当我们的程序跳出当前的代码块时，对应内部的申请的内存空间可以自行释放。这就是智能指针所要实现的需求。

## 2.智能指针要解决的问题

### 2.1RAII

> RAII（**R**esource **A**cquisition **I**s **I**nitialization）是由c++之父Bjarne Stroustrup提出的，中文翻译为资源获取即初始化，他说：使用局部对象来管理资源的技术称为资源获取即初始化；这里的资源主要是指操作系统中有限的东西如内存、网络套接字等等，局部对象是指存储在栈的对象，它的生命周期是由操作系统来管理的，无需人工介入；
>
> ```c++
> template<class T>
> class SmartPtr
> {
> public:
> 	SmartPtr(T* ptr)
> 		:_ptr(ptr)
> 	{}
> 
> 	SmartPtr(SmartPtr<T>& ptr)
> 	{
> 		_ptr = new T(*ptr);
> 	}
> 
> 	~SmartPtr()
> 	{
> 		cout << "delete " << _ptr << endl;
> 		delete _ptr;
> 	}
> 
> private:
> 	T* _ptr;
> };
> 
> int main()
> {
> 	SmartPtr<int> p1(new int(10));
> 	SmartPtr<int> p2(new int(20));
> 	return 0;
> }
> ```
>
> 这样指针就可以随着运行申请和释放

### 2.2像指针一样使用

> 那么我们需要实现两个功能：①重载*；②重载->
>
> ```c++
> template<class T>
> class SmartPtr
> {
> public:
> 	SmartPtr(T* ptr)
> 		:_ptr(ptr)
> 	{}
> 
> 	SmartPtr(SmartPtr<T>& ptr)
> 	{
> 		_ptr = new T(*ptr);
> 	}
> 
> 	T& operator*()
> 	{
> 		return *_ptr;
> 	}
> 
> 	T* operator->()
> 	{
> 		return _ptr;
> 	}
> 
> 	~SmartPtr()
> 	{
> 		cout << "delete " << _ptr << endl;
> 		delete _ptr;
> 	}
> 
> private:
> 	T* _ptr;
> };
> 
> int main()
> {
> 	SmartPtr<int> p1(new int(10));
> 	SmartPtr<int> p2(new int(20));
> 	SmartPtr<int> p3(p1);
> 
> 	*p1 = 1;
> 	*p2 = 2;
> 
> 	cout << *p1 << " " << *p2 << endl;
> 
> 	return 0;
> }
> ```
>
> 运行结果：
>
> ![image-20240203010144149](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240203010144149.png)

### 2.3拷贝问题

#### 2.3.1智能指针的发展史

①指针管理权的转移--auto_ptr

> template<class T>
> 	class auto_ptr
> 	{
> 	public:
> 		auto_ptr(T* ptr)
> 			:_ptr(ptr)
> 		{}
>
> 		~auto_ptr()
> 		{
> 			if (_ptr)
> 			{
> 				cout << "delete:" << _ptr << endl;
> 				delete _ptr;
> 			}
> 		}
> 	
> 		// 管理权转移
> 		auto_ptr(auto_ptr<T>& ap)
> 			:_ptr(ap._ptr)
> 		{
> 			ap._ptr = nullptr;
> 		}
> 	
> 		T& operator*()
> 		{
> 			return *_ptr;
> 		}
> 	
> 		T* operator->()
> 		{
> 			return _ptr;
> 		}
> 	private:
> 		T* _ptr;
> 	};

②防拷贝--unique_ptr

> 拷贝构造和赋值是默认成员函数，我们不写会自动生成，所以我们不需要写
>
> C++98的思路：只声明不实现，但是使用者可能会在外部强制定义，所以再加一条声明为私有：
>
> ```C++
> private:
> 	unique_ptr(const unique_ptr<T>& up);
> ```
>
> C++11的思路：语法支持将这个函数删除
>
> ```c++
> unique_ptr(const unique_ptr<T>& up) = delete;
> unique_ptr<T>& operator=(const unique_ptr<T>& up) = delete;
> ```
>

③引用计数--shared_ptr

> 采用引用计数的方式来实现多个智能针织指向同一个对象的情况(多线程)，另外利用了加锁的方式来实现多线程安全的问题。
>
> ```c++
> template<class T>
> 	class shared_ptr
> 	{
> 	public:
> 		shared_ptr(T* ptr)
> 			:_ptr(ptr)
> 			, _pcount(new int(1))
> 			,_pmtx(new mutex)
> 		{}
> 
> 		shared_ptr(const shared_ptr<T>& sp)
> 			:_ptr(sp._ptr)
> 			,_pcount(sp._pcount)
> 			,_pmtx(sp._pmtx)
> 		{
> 			AddCount();
> 		}
> 
> 		T& operator*()
> 		{
> 			return *_ptr;
> 		}
> 
> 		T* operator->()
> 		{
> 			return _ptr;
> 		}
> 
> 		shared_ptr<T>& operator=(const shared_ptr<T>& sp)
> 		{
> 			if (_ptr != sp._ptr)
> 			{
> 				//被赋值指针引用计数为1的情况
> 				if (--(*_pcount) == 0)
> 				{
> 					cout << "delete:" << _ptr << endl;
> 					delete _ptr;
> 					delete _pcount;
> 				}
> 
> 				_ptr = sp._ptr;
> 				_pcount = sp._pcount;
> 				AddCount();
> 			}
> 
> 			return *this;
> 		}
> 
> 		T* get()
> 		{
> 			return _ptr;
> 		}
> 
> 		int use_count()
> 		{
> 			return* _pcount;
> 		}
> 
> 		void AddCount()
> 		{
> 			_pmtx->lock();
> 
> 			++(*_pcount);	
> 
> 			_pmtx->unlock();
> 		}
> 
> 
> 
> 		~shared_ptr()
> 		{
> 			if (--(*_pcount) == 0)
> 			{
> 				delete _ptr;
> 				delete _pcount;
> 			}
> 		}
> 
> 	private:
> 		T* _ptr;
> 		int* _pcount;
> 		mutex* _pmtx;
> 	};
> 
> 	struct Date
> 	{
> 		int _year = 0;
> 		int _month = 0;
> 		int _day = 0;
> 
> 		~Date()
> 		{}
> 	};
> 
> 
> 
> 	void SharePtrFunc(xupt::shared_ptr<Date>& sp, size_t n, mutex& mtx)
> 	{
> 		
> 		xupt::shared_ptr<Date> copy(sp);
> 
> 		
> 		for (size_t i = 0; i < n; ++i)
> 		{
> 
> 			mtx.lock();
> 
> 			sp->_year++;
> 			sp->_month++;
> 			sp->_day++;
> 
> 			mtx.unlock();
> 		}
> 	}
> 
> ```

**shared_ptr应用的局限--循环引用**

> 当我们建立一个双向链表的时，那么这时候shared_ptr就会导致资源无法完成释放，从而造成内存泄漏。
>
> 我们先写出如下的代码：
>
> ```c++
> struct ListNode
> 	{
> 		xupt::shared_ptr<ListNode> _prev;
> 		xupt::shared_ptr<ListNode> _next;
> 
> 
> 		~ListNode()
> 		{
> 			cout << "~ListNode()" << endl;
> 		}
> 	};
> 
> 	void test_shared_cycle()
> 	{
> 		xupt::shared_ptr<ListNode> n1(new ListNode);
> 		xupt::shared_ptr<ListNode> n2(new ListNode);
> 
> 		n1->_next = n2;
>         
> 		cout << n1.use_count() << endl;
> 		cout << n2.use_count() << endl;
> 	}
> 	
> 
> ```
>
> 运行一下：
>
> 我们发现资源被正常释放，为什么会正常释放？首先我们只是让n1的next指向n2所对应的指针，那么此时n1对应指针的引用计数是1，n2对应指针的引用计数是2。这样一来当程序运行出作用域后，n2和n1释放，那么最终产生的结果就是n1的应用计数为0，资源释放并且对应的内部成员也跟着释放，n2引用计数为1，但是由于n1的next紧跟着释放，那么对应的应用计数也变成了0；所以两个资源都正常释放了！
>
> ![image-20240208114336039](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240208114336039.png)
>
> 接下来我们尝试一下另外一种：两个指针都赋值
>
> 我们发现内存没有被释放，这是为什么，原因是当我们赋值结束的时候，n1和n2所指向的资源引用计数都是2，那么当程序运行走出作用域，首先n2被析构，n2对应的指针引用计数减一，同样的n1也是如此。但是由于他们的引用计数都不是0，所以都没有被释放，并且由于_next 和 _prev 都指向了n2和n1，他们两个互相牵制都释放不了对应的资源。
>
> ```c++
> 		xupt::shared_ptr<ListNode> n1(new ListNode);
> 		xupt::shared_ptr<ListNode> n2(new ListNode);
> 
> 		n1->_prev = n2;
> 		n2->_prev = n1;
> ```
>
> ![image-20240208114741981](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240208114741981.png)

④weak_ptr--解决shared_ptr的循环引用问题

1、他不是常规的智能指针，不支持RAII
2、支持像指针一样
3、专门设计出来，辅助解决shared_ptr的循环引用问题，weak_ptr可以指向资源，但是他不参与管理，不增加引用计数

> 我们发现上面为什么会引起循环引用的问题，原因就是引用计数后面产生的矛盾，所以我们用weak_ptr解决引用计数问题的核心方法就是不让引用计数加加：
>
> ```c++
> template<class T>
> 	class weak_ptr
> 	{
> 	public:
> 		weak_ptr()
> 			:_ptr(nullptr)
> 		{}
> 
> 		weak_ptr(shared_ptr<T>& sp)
> 			:_ptr(sp.get())
> 		{}
> 
> 		T& operator*()
> 		{
> 			return *_ptr;
> 		}
> 
> 		T* operator->()
> 		{
> 			return _ptr;
> 		}
> 
> 		T* get()
> 		{
> 			return _ptr;
> 		}
> 
> 	private:
> 		T* _ptr;
> 	};
> 
> struct ListNode
> 	{
> 		//xupt::shared_ptr<ListNode> _prev;
> 		//xupt::shared_ptr<ListNode> _next;
> 
> 		xupt::weak_ptr<ListNode> _prev;
> 		xupt::weak_ptr<ListNode> _next;
> 
> 		~ListNode()
> 		{
> 			cout << "~ListNode()" << endl;
> 		}
> 	};
> ```
>
> 运行一下：
>
> 资源被正常释放了！
>
> ![image-20240208121650768](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240208121650768.png)

### 2.4定制删除器---可调用对象

> 我们在shared_ptr的构造函数中多引入了一个参数del，这就是我们的定制删除器，本质也就是一个可调用对象。
>
> ```c++
> 		template<class D>
> 		shared_ptr(T* ptr, D del)
> 			:_ptr(ptr)
> 			, _pcount(new int(1))
> 			, _pmtx(new mutex)
> 			, _del(del)
> 		{}
> ```
>
> 那么成员函数怎么定义：
>
> 我们为什么这么写？因为模板参数写在了构造函数我们不能直接去写D类型的成员，我们可以通过包装器的方式来实现，并且给一个默认的lambda表达式。
>
> ```c++
> // 包装器
> 		function<void(T*)> _del = [](T* ptr){
> 			cout << "lambda delete:" << ptr << endl;
> 			delete ptr; 
>         };
> ```
>
> 我们测试一下：
>
> ```c++
> void test_shared_deletor()
> 	{
> 		xupt::shared_ptr<Date> sp0(new Date);
> 
> 		xupt::shared_ptr<Date> spa2(new Date[10], [](Date* ptr) {
> 			cout << "lambda delete[]:" << ptr << endl;
> 			delete[] ptr;
> 			});
> 
> 		xupt::shared_ptr<FILE> spF3(fopen("test.cpp", "r"), [](FILE* ptr) {
> 			cout << "lambda fclose:" << ptr << endl;
> 			fclose(ptr);
> 			});
> 	}
> ```
>
> 运行结果：
>
> ![image-20240208131625311](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240208131625311.png)

