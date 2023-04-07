# C++类和对象（下）

## Ⅰ. 再谈构造函数

### Ⅰ. Ⅰ构造函数体赋值：

> 在创建对象时，编译器通过调用构造函数，给对象中各个成员变量一个合适的初始值。
>
> ```C++
> class Date
> {
> public:
> Date(int year, int month, int day)
>  {
>      _year = year;
>      _month = month;
>      _day = day;
>  }
> private:
> int _year;
> int _month;
> int _day;
> };
> 
> ```
>
> 虽然上述构造函数调用之后，对象中已经有了一个初始值，<u>**但是不能将其称为对对象中成员变量的初始化**</u>，构造函数体中的语句<u>**只能将其称为赋初值**</u>，而不能称作初始化。<u>**因为初始化只能初始化一次，而构造函数体内可以多次赋值。**</u>

### Ⅰ. Ⅱ初始化列表：

> 初始化列表：以一个**冒号开始**，接着是一个**以逗号分隔的数据成员列表**，每个"成员变量"后面跟 一个放在括号中的初始值或表达式。
>
> ```c++
> class Date
> {
> public:
> Date(int year, int month, int day)
>      : _year(year)
>      , _month(month)
>      , _day(day)
>  {}
> private:
> int _year;
> int _month;
> int _day;
> };
> ```
>
> **注意：**
>
> ​	①每个成员变量在初始化列表中**<u>只能出现一次</u>**（初始化只能进行一次）
>
> ​	②类中包含以下类型的成员，必须放在初始化列表位置进行初始化：
>
> - 引用成员变量
>
> - const 成员变量
>
> - 自定义类型成员（且该类的没有默认的构造函数）。
>
>   ```c++
>   class A
>   {
>   public:
>    	A(int a)
>    	:_a(a)
>    	{}
>   private:
>    	int _a;
>   };
>   class B
>   {
>   	public:
>    	B(int a, int ref)
>    	:_aobj(a)
>    	,_ref(ref)
>    	,_n(10)
>    	{}
>   private:
>    	A _aobj;  // 没有默认构造函数
>    	int& _ref;  // 引用
>    	const int _n; // const 
>   };
>   ```
>
>   ③尽量使用初始化列表初始化，因为不管你是否使用初始化列表，对于自定义类型成员变量， 一定会先使用初始化列表初始化。
>
>   ```C++
>   class Time
>   {
>   public:
>    	Time(int hour = 0)
>    	:_hour(hour)
>    	{
>    		cout << "Time()" << endl;
>    	}
>   private:
>    	int _hour;
>   	};
>   class Date
>   {
>   	public:
>    	Date(int day)
>    	{}
>   	private:
>    		int _day;
>    		Time _t;
>   };
>   int main()
>   {
>       Date d(1);
>   }
>   ```
>
>   ④成员变量在类中声明次序就是其在初始化列表中的初始化顺序，与其在初始化列表中的先后 次序无关
>
>   ```c++
>   class A
>   {
>   public:
>       A(int a)
>          :_a1(a)
>          ,_a2(_a1)
>      {}
>             
>       void Print() {
>           cout<<_a1<<" "<<_a2<<endl;
>      }
>   private:
>       int _a2;
>       int _a1;
>   };
>   int main() {
>       A aa(1);
>       aa.Print();
>   }
>   A. 输出1  1
>   B.程序崩溃
>   C.编译不通过
>   D.输出1  随机值
>   //综上选D
>   ```
>

### Ⅰ. Ⅲ explicit关键字：

> 构造函数不仅可以构造与初始化对象，**<u>对于单个参数或者除第一个参数无默认值其余均有默认值的构造参数，还具有类型转换的作用。</u>**
>
> ```c++
> class Date
> {
> public:
>     // 1. 单参构造函数，没有使用explicit修饰，具有类型转换作用
>  // explicit修饰构造函数，禁止类型转换---explicit去掉之后，代码可以通过编译
>  explicit Date(int year)
>  :_year(year)
>  {}
>  /*
>  // 2. 虽然有多个参数，但是创建对象时后两个参数可以不传递，没有使用explicit修饰，具
> 有类型转换作用
>  // explicit修饰构造函数，禁止类型转换
>  explicit Date(int year, int month = 1, int day = 1)
>  : _year(year)
>  , _month(month)
>  , _day(day)
>  {}
>  */
>  Date& operator=(const Date& d)
>  {
>  if (this != &d)
>  {
>  _year = d._year;
>  _month = d._month;
>  _day = d._day;
>  }
>  return *this;
>  }
> private:
>  int _year;
>  int _month;
>  int _day;
> };
> void Test()
> {
>  Date d1(2022);
>  // 用一个整形变量给日期类型对象赋值
>  // 实际编译器背后会用2023构造一个无名对象，最后用无名对象给d1对象进行赋值
>  d1 = 2023;
>  // 将1屏蔽掉，2放开时则编译失败，因为explicit修饰构造函数，禁止了单参构造函数类型转
> 换的作用
> }
> ```
>
> 上述代码可读性不是很好，**用explicit修饰构造函数，将会禁止构造函数的隐式转换**。

## Ⅱ. static 成员

### Ⅱ .Ⅰ概念：

> 声明为static的类的成员**称为类的静态成员**，用static修饰的成员变量，称**之为静态成员变量**；用**static修饰的函数，称之为静态成员函数**。<u>**静态成员变量一定要在类外进行初始化！**</u>（在类外初始化的原因，因为它属于所有对象，属于整个类，所以它首先不是在构造函数的初始化列表进行初始化，那么就不能给缺省值，只能将其定义在类外语法是： **类型`类名` 静态变量名 = 值；**）
>
> Ⅱ . Ⅱ特性：
>
> ①**静态成员**为**所有成员所共享**，不属于某个具体的对象，**存放在静态区**。
>
> ②**静态成员变量必须在类外定义**，定义是**不添加static关键字，类中只是声明**。
>
> ③类静态成员可以用 **类名`::`静态成员**或者**a`.`静态成员**来访问。
>
> ④**静态成员函数**没有隐藏的this指针，不能访问任何非静态成员。
>
> ⑤**非静态成员函数**是可以调用**类的静态成员函数**。
>
> **面试题：**实现一个类，计算程序中创建除了多少个类。
>
> ```c++
> class A
> {
> 	public:
> 		A() { ++_scount; }//构造函数
> 		A(const A& t) { ++_scount; } //拷贝构造函数
> 		~A() { --_scount; }	//析构函数
> 		static int GetACount() { return _scount; } //静态成员函数
> 	private:
> 		static int _scount; //静态类成员变量
> };
> 	int A::_scount = 0;
> void TestA()
> {
> 	cout << A::GetACount() << endl;
> 	A a1, a2;
> 	A a3(a1);
> 	cout << A::GetACount() << endl;
> }
> ```

## Ⅲ	匿名对象：

> 概念：是一个没有名字的类的对象，但是它的声明周期只在它定义的那一行有效。
>
> 语法：类名`()`;
>
> 例子：
>
> ```c++
> class A
> {
> public:
>  	A(int a = 0)
>  	:_a(a)
>  	{
>  		cout << "A(int a)" << endl;
>  	}
>  	~A()
>  	{
> 		cout << "~A()" << endl;
>  	}
> private:
>  	int _a;
> };
> class Solution {
> public:
>  	int Sum_Solution(int n) 
>     {
>  		//...
>  		return n;
>  	}
> };
> int main()
> {
>  	A aa1;
>  	// 不能这么定义对象，因为编译器无法识别下面是一个函数声明，还是对象定义
>  	//A aa1();
>  	// 但是我们可以这么定义匿名对象，匿名对象的特点不用取名字，
>  	// 但是他的生命周期只有这一行，我们可以看到下一行他就会自动调用析构函数
>  	A();
>  	A aa2(2);
>  	// 匿名对象在这样场景下就很好用，当然还有一些其他使用场景，这个我们以后遇到了再说
>  	Solution().Sum_Solution(10);
>  	return 0;
> }
> 
> ```

## Ⅳ 友元

> 友元提供了一种突破封装的方式，有时提供了便利。但是有缘会破坏耦合度，破坏了封装，所以友元不宜多用。
>
> 友元分为：**友元函数**和**友元类**

### Ⅵ .Ⅰ友元函数

> 问题：现在尝试去重载operator<<，然后发现没办法将operator<<重载成成员函数。**因为`cout`的 输出流对象和隐含的this指针在抢占第一个参数的位置**。this指针默认是第一个参数也就是左操作 数了。**但是实际使用中`cout`需要是第一个形参对象，才能正常使用**。所以要将operator<<重载成 全局函数。**但又会导致类外没办法访问成员**，此时就需要友元来解决。operator>>同理。
>
> ```c++
> class Date
> {
> public:
> Date(int year, int month, int day)
>      : _year(year)
>      , _month(month)
>      , _day(day)
>  {}
> // d1 << cout; -> d1.operator<<(&d1, cout); 不符合常规调用
> // 因为成员函数第一个参数一定是隐藏的this，所以d1必须放在<<的左侧
> ostream& operator<<(ostream& _cout)
>  {
>      _cout << _year << "-" << _month << "-" << _day << endl;
>      return _cout;
>  }
> private:
> int _year;
> int _month;
> int _day;
> };
> ```
>
> **友元函数**可以**直接访问类的私有成员**，它是**定义在类外部**的普通函数，**不属于任何类**，但需**要在类的内部声明，声明时需要加friend关键字.**
>
> ```c++
> class Date
> {
>  	friend ostream& operator<<(ostream& _cout, const Date& d);
>  	friend istream& operator>>(istream& _cin, Date& d);
> public:
>  	Date(int year = 1900, int month = 1, int day = 1)
>  	: _year(year)
>  	, _month(month)
>  	, _day(day)
>  	{}
> private:
>  	int _year;
>  	int _month;
>  	int _day;
> };
> ostream& operator<<(ostream& _cout, const Date& d)
> {
>  	_cout << d._year << "-" << d._month << "-" << d._day;
>  	return _cout; 
> }
> istream& operator>>(istream& _cin, Date& d)
> {
>  	_cin >> d._year;
>  	_cin >> d._month;
>  	_cin >> d._day;
>  	return _cin;
> }
> int main()
> {
>  	Date d;
>  	cin >> d;
>  	cout << d << endl;
>  	return 0;
> }
> 
> ```
>
> 说明：
>
> - **友元函数**可以访问类的私有和保护乘员，但**不是类的成员函数**。
> - 友元函数**不能用`const`修饰**。
> - 友元函数可以声明在类定义的**任意位置**，**不受类的访问限定符的限制**。
> - 一个函数可以是多个类的友元函数。
> - 友元函数的调用和普通函数的调用原理相同。

### Ⅳ. Ⅱ友元类：

> 友元类的所有成员函数都可以是另一个类的友元函数，都可以访问另一个类中的非公有成员。
>
> - 友元关系是单向的，不具有交换性。
>
>   比如上述Time类和Date类，**在Time类中声明Date类为其友元类，那么可以在Date类中直接 访问Time类的私有成员变量**，但想在Time类中访问Date类中私有的成员变量则不行。
>
> - 友元关系不能传递。
>
>   如果C是B的友元， B是A的友元，则不能说明C时A的友元。
>
> - 友元关系不能继承。
>
> ```c++
> class Time
> {
>    friend class Date;   // 声明日期类为时间类的友元类，则在日期类中就直接访问Time类中的私有成员变量
> public:
>  Time(int hour = 0, int minute = 0, int second = 0)
>  : _hour(hour)
>  , _minute(minute)
>  , _second(second)
>  {}
>    
> private:
>    int _hour;
>    int _minute;
>    int _second;
> };
> class Date
> {
> public:
>    Date(int year = 1900, int month = 1, int day = 1)
>        : _year(year)
>        , _month(month)
>        , _day(day)
>    {}
>    
>    void SetTimeOfDate(int hour, int minute, int second)
>    {
>        // 直接访问时间类私有的成员变量
>        _t._hour = hour;
>        _t._minute = minute;
>        _t._second = second;
>    }
>    
> private:
>    int _year;
>    int _month;
>    int _day;
>    Time _t;
> };
> ```

## Ⅴ内部类

> 概念：**如果一个类定义在另一个类的内部，这个内部类就叫做内部类**。内部类是一个独立的类， 它不属于外部类，更不能通过外部类的对象去访问内部类的成员。外部类对内部类没有任何优越 的访问权限。
>
> 注意：**内部类就是外部类的友元类**，参见友元类的定义，内部类可以通过外部类的对象参数来访 问外部类中的所有成员。但是外部类不是内部类的友元。
>
> 特性：
>
> - 内部类可以定义在外部类的public、protected、private都是可以的。
> -  注意**内部类可以直接访问外部类中的static成员**，不需要外部类的对象/类名。
> - **`sizeof`(外部类)=外部类，和内部类没有任何关系**。
>
> ```C++
> class A
> {
> private:
>  	static int k;
>  	int h;
> public:
>  	class B // B天生就是A的友元
>  	{
> 	public:
>  		void foo(const A& a)
>  		{
>  			cout << k << endl;//OK
>  			cout << a.h << endl;//OK
>  		}
>  	};
> };
> int A::k = 1;
> int main()
> {
>     A::B b;
>     b.foo(A());
>     
>     return 0;
> }
> 
> ```
>

## Ⅵ 拷贝对象时的一些编译器的优化

> 在传参和传返回值的过程中，一般编译器会做一些优化，减少对象的拷贝，这个在一些场景下还是非常有用的。
>
> ```c++
> class A
> {
> public:
>  	A(int a = 0)
>  	:_a(a)
>  	{
>  		cout << "A(int a)" << endl;
>  	}
>  	A(const A& aa)
>  	:_a(aa._a)
>  	{
>  		cout << "A(const A& aa)" << endl;
>  	}
>   	A& operator=(const A& aa)
>  	{
>  		cout << "A& operator=(const A& aa)" << endl;
>  		if (this != &aa)
>  		{
>  			_a = aa._a;
>  		}
>  		return *this;
>  	}
>  	~A()
>  	{
>  		cout << "~A()" << endl;
>  	}
> private:
>  	int _a;
> };
> void f1(A aa)
> {}
> A f2()
> {
>  	A aa;
>  	return aa;
> }
> int main()
> {
>  	// 传值传参
>  	A aa1;
>  	f1(aa1);
>  	cout << endl;
>  	// 传值返回
>  	f2();
>  	cout << endl;
>  	// 隐式类型，连续构造+拷贝构造->优化为直接构造
>  	f1(1);
>  	// 一个表达式中，连续构造+拷贝构造->优化为一个构造
>  	f1(A(2));
>  	cout << endl;
>  	// 一个表达式中，连续拷贝构造+拷贝构造->优化一个拷贝构造
>  	A aa2 = f2();
>  	cout << endl;
>  	// 一个表达式中，连续拷贝构造+赋值重载->无法优化
>  	aa1 = f2();
>  	cout << endl;
>  return 0;
> }
> 
> ```
>
> 综上：
>
> - 隐式类型，连续构造+拷贝构造->优化为直接构造
> - 一个表达式中，连续构造+拷贝构造->优化为一个构造
> - 一个表达式中，连续拷贝构造+拷贝构造->优化一个拷贝构造
> - 接受返回值对象尽量拷贝构造的方式接收不要赋值接收
> - 函数中返回对象时，尽量返回匿名对象
> - 函数传参尽量使用const & 传参

