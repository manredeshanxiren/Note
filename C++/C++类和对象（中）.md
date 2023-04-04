# C++类和对象（中）

## Ⅰ.类的6个默认成员函数：



> 如果一个类中什么成员都没有，简称为空类。
>
> 空类中编译器会自动生成以下6个默认成员函数。
>
> **<u>默认成员函数</u>**：用户没有显式实现，编译器会生成的成员函数称为默认成员函数。
>
> ![image-20230326225033559](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230326225033559.png)

## Ⅱ.构造函数：

### Ⅱ. Ⅰ概念：

> 对于以下的一个Data类，我们可以通过Init函数来给对象设置日期，但是如果每次创建对象的时候都去调用该方法去实现日期的初始化，那么是有点麻烦的，因此C++设计了**<u>构造函数</u>**。
>
> <u>**构造函数**</u>是一个特殊的成员函数，它的<u>**名字和类的名字相同**</u>，创建类类型对象时<u>**由编译器自动调用**</u>，以保证每个数据成员都有一个合适的初始值，并且**<u>在对象的声明周期内</u>**。
>
> ```c++
> class Date
> {
> public:
>  	void Init(int year, int month, int day)
>  	{
>     		_year = year;
>  		_month = month;
>  		_day = day;
>  	}
>  	void Print()
>  	{
>  		cout << _year << "-" << _month << "-" << _day << endl;
>  	}
> 	private:
>  		int _year;
>  		int _month;
>  		int _day;
> };
> int main()
> {
>  	Date d1;
>  	d1.Init(2022, 7, 5);
>  	d1.Print();
>  	Date d2;
>  	d2.Init(2022, 7, 6);
>  	d2.Print();
>  	return 0;
> }
> ```

### Ⅱ. Ⅱ特性：

> **<u>构造函数</u>**并不是开空间创建对象，而是初始化对象。
>
> 其特征如下：
>
> ①函数名与类名相同。
>
> ②无返回值。
>
> ③对象实例化的时候，编译器<u>**自动调用**</u>对应的构造函数。
>
> ④构造函数**<u>可以重载</u>**。
>
> ```c++
>  class Date
>  {
>   public:
>       // 1.无参构造函数
>       Date()
>      {}
>   
>       // 2.带参构造函数
>       Date(int year, int month, int day)
>      {
>           _year = year;
>             _month = month;
>           _day = day;
>      }
>   private:
>       int _year;
>       int _month;
>       int _day;
>  };
>   
>   void TestDate()
>  {
>       Date d1; // 调用无参构造函数
>       Date d2(2015, 1, 1); // 调用带参的构造函数
>   
>       // 注意：如果通过无参构造函数创建对象时，对象后面不用跟括号，否则就成了函数声明
>       // 以下代码的函数：声明了d3函数，该函数无参，返回一个日期类型的对象
>       // warning C4930: “Date d3(void)”: 未调用原型函数(是否是有意用变量定义的?)
>       Date d3();
>  }        
> ```
>
> ⑤如果类中没有显式定义构造函数，则C++编译器会自动生成一个无参的默认构造函数，一旦 用户显式定义编译器将不再生成。
>
> ```c++
>  class Date
>  {
>   	public:
>  	/*
>  	// 如果用户显式定义了构造函数，编译器将不再生成
> 	 Date(int year, int month, int day)
>  	{
>  		_year = year;
>  		_month = month;
>  		_day = day;
>  	}
>  	*/
>  
>  void Print()
>  {
>  	cout << _year << "-" << _month << "-" << _day << endl;
>  }
>   
>   private:
>  	int _year;
>  	int _month;
>  	int _day;
>  };
>   
>  int main()
>  {
>  	// 将Date类中构造函数屏蔽后，代码可以通过编译，因为编译器生成了一个无参的默认构造函数
>  	// 将Date类中构造函数放开，代码编译失败，因为一旦显式定义任何构造函数，编译器将不再生成
>     // 无参构造函数，放开后报错：error C2512: “Date”: 没有合适的默认构造函数可用
>  	Date d1;
>  	return 0;
>  }
> 
> ```
>
> ⑥关于编译器生成的默认成员函数：不实现构造函数的情况下，编译器会 生成默认的构造函数。但是看起来默认构造函数又没什么用？d对象调用了编译器生成的默 认构造函数，但是d对象_year/_month/_day，依旧是随机值。也就说在这里编译器生成的 默认构造函数并没有什么用？？ C++把类型分成内置类型(基本类型)和自定义类型。内置类型就是语言提供的数据类 型，如：int/char...，自定义类型就是我们使用class/struct/union等自己定义的类型，看看 下面的程序，就会发现编译器生成默认的构造函数会对自定类型成员_t调用的它的默认成员 函数
>
> ```C++
> class Time
> {
> public:
>  	Time()
> 	{
>  		cout << "Time()" << endl;
>  		_hour = 0;
>  		_minute = 0;
>  		_second = 0;
>  	}
> private:
>  	int _hour;
>  	int _minute;
>  	int _second;
> };
> class Date
> {
> 	private:
>  		// 基本类型(内置类型)
>  		int _year;
>  		int _month;
>  		int _day;
>  		// 自定义类型
>  		Time _t;
> };
> int main()
> {
>  	Date d;
>  	return 0;
> }
> ```
>
> 注意：C++11 中针对内置类型成员不初始化的缺陷，又打了补丁，即：<u>**内置类型成员变量在 类中声明时可以给默认值**</u>。
>
> ```C++
> class Time
> {
> 	public:
>  		Time()
>  		{
>  			cout << "Time()" << endl;
>  			_hour = 0;
>  			_minute = 0;
>  			_second = 0;
>  		}
> 	private:
>  		int _hour;
>  		int _minute;
>  		int _second;
> };
> class Date
> {
> private:
>  	// 基本类型(内置类型)
> 	int _year = 1970;
>  	int _month = 1;
>  	int _day = 1;
>  	// 自定义类型
> 	 Time _t;
> };
> int main()
> {
>  	Date d;
>  	return 0;
> }
> 
> ```
>
> ⑦无参的构造函数和全缺省的构造函数都称为默认构造函数，并且默认构造函数只能有一个。 注意：无参构造函数、全缺省构造函数、我们没写编译器默认生成的构造函数，都可以认为 是默认构造函数
>
> ```C++
> class Date
> {
> 	public:
>  	Date()
>  	{
>  		_year = 1900;
>  		_month = 1;
>  		_day = 1;
>  	}
>  	Date(int year = 1900, int month = 1, int day = 1)
>  	{
>  		_year = year;
>  		_month = month;
>  		_day = day;
>  	}
> 	private:
>  		int _year;
>  		int _month;
>  		int _day;
> };
> // 以下测试函数能通过编译吗？
> //不能通过编译，因为一个类中不能包含多个默认构造函数。
> void Test()
> {
>  	Date d1;
> }
> ```

## Ⅲ.析构函数：

### Ⅲ. Ⅰ概念：

> **<u>析构函数：</u>**与构造函数功能相反，析构函数不是完成对对象本身的销毁，局部对象销毁工作是由 编译器完成的。而对象在销毁时会自动调用析构函数，完成对象中资源的清理工作。

### Ⅲ. Ⅱ特性：

> ①析构函数的函数名是在函数名前加上`~`。
>
> ②<u>**无参数无返回值类型**</u>。
>
> ③一个类<u>**只能有一个**</u>析构函数。若未显式定义，系统会<u>**自动生成默认的析构函数**</u>。注意：析构函数不能重载。
>
> ④对象的声明周期结束时，C++编译器会自动调用析构函数。
>
> ```C++
> typedef int DataType;
> class Stack
> {
> 	public:
>  	Stack(size_t capacity = 3)
>  	{
>  		_array = (DataType*)malloc(sizeof(DataType) * capacity);
>  		if (NULL == _array)
>  		{
>  			perror("malloc申请空间失败!!!");
>  			return;
>  		}
>   		_capacity = capacity;
>  		_size = 0;
>  	}
>  	void Push(DataType data)
>  	{
>  		// CheckCapacity();
>  		_array[_size] = data;
>  		_size++;
>  	}
>  	// 其他方法...
>  	~Stack()
>  	{
>  		if (_array)
>  		{
>  			free(_array);
>  			_array = NULL;
>  			_capacity = 0;
>  			_size = 0;
>  		}
>  	}
> 	private:
>  		DataType* _array;
>  		int _capacity;
>  		int _size;
> };
> void TestStack()
> {
>  	Stack s;
>  	s.Push(1);
>  	s.Push(2);
> }
> ```
>
> ⑤关于编译器自动生成的析构函数，是否会完成一些事情呢？下面的程序我们会看到，编译器 生成的默认析构函数，对自定类型成员调用它的析构函数。
>
> ```c++
> class Time
> {
> 	public:
>  	~Time()
>  	{
>  		cout << "~Time()" << endl;
>  	}
> 	private:
>  		int _hour;
>  		int _minute;
>  		int _second;
> };
> class Date
> {
> 	private:
>  	// 基本类型(内置类型)
>  	int _year = 1970;
>  	int _month = 1;
>  	int _day = 1;
>  	// 自定义类型
>  	Time _t;
> };
> int main()
> {
>  	Date d;
>  	return 0;
> }
> // 程序运行结束后输出：~Time()
> // 在main方法中根本没有直接创建Time类的对象，为什么最后会调用Time类的析构函数？
> // 因为：main方法中创建了Date对象d，而d中包含4个成员变量，其中_year, _month, _day三个是
> // 内置类型成员，销毁时不需要资源清理，最后系统直接将其内存回收即可；而_t是Time类对象，所以在
> // d销毁时，要将其内部包含的Time类的_t对象销毁，所以要调用Time类的析构函数。但是：main函数
> // 中不能直接调用Time类的析构函数，实际要释放的是Date类对象，所以编译器会调用Date类的析构函
> // 数，而Date没有显式提供，则编译器会给Date类生成一个默认的析构函数，目的是在其内部调用Time
> // 类的析构函数，即当Date对象销毁时，要保证其内部每个自定义对象都可以正确销毁
> // main函数中并没有直接调用Time类析构函数，而是显式调用编译器为Date类生成的默认析构函数
> // 注意：创建哪个类的对象则调用该类的析构函数，销毁那个类的对象则调用该类的析构函数
> 
> ```
>
> ⑥如果类中没有申请资源时，析构函数可以不写，直接使用编译器生成的默认析构函数，比如 Date类；有资源申请时，一定要写，否则会造成资源泄漏，比如Stack类。

### Ⅳ.拷贝构造函数：

### Ⅳ. Ⅰ概念：

> 在现实生活中，可能存在一个与你一样的自己，我们称其为双胞胎。那么我们在C++中创建对象的时候是否可以创建一个和已存在对象相同的对象呢？
>
> 这时候就要用到拷贝构造函数了。
>
> <u>**拷贝构造函数：**</u>只有<u>**单个形参**</u>，该形参是对本类类型对象的<u>**引用(一般常用const修饰)**</u>，在用<u>**已存在的类类型对象创建新对象时由编译器自动调用**</u>。

### Ⅳ. Ⅱ特征：

> ①拷贝构造函数是<u>**构造函数的一个重载形式**</u>。
>
> ②拷贝构造函数的参数<u>**只有一个且必须是类类型对象的引用**</u>，使用传值方式编译器直接报错， 因为会引发无穷递归调用。
>
> ```C++
> class Date
> {
> 	public:
> 		Date(int year = 1900, int month = 1, int day = 1)
> 		{
> 			_year = year;
> 			_month = month;
> 			_day = day;
> 		}
> 		//Date(Date d)   // 错误写法：编译报错，会引发无穷递归
> 	Date(const Date& d)   // 正确写法
> 		{
> 			_year = d._year;
> 			_month = d._month;
> 			_day = d._day;
> 		}
> 	private:
> 		int _year;
> 		int _month;
> 		int _day;
> };
> int main()
> {
> 	Date d1;
> 	Date d2(d1);
> return 0;
> }
> ```
>
> ![	](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230327135124078.png)
>
> ③若未显式定义，编译器会生成默认的拷贝构造函数。 默认的拷贝构造函数对象按内存存储按 字节序完成拷贝，这种拷贝叫做浅拷贝，或者值拷贝。
>
> ```c++
> class Time
> {
> 	public:
> 	Time()
> 	{
> 		_hour = 1;
> 		_minute = 1;
> 		_second = 1;
> }
> Time(const Time& t)
> {
> _hour = t._hour;
> _minute = t._minute;
> _second = t._second;
> cout << "Time::Time(const Time&)" << endl;
> }
> private:
> int _hour;
> int _minute;
> int _second;
> };
> class Date
> {
> private:
> // 基本类型(内置类型)
> int _year = 1970;
> int _month = 1;
> int _day = 1;
> // 自定义类型
> Time _t;
> };
> int main()
> {
> Date d1;
> 
>  // 用已经存在的d1拷贝构造d2，此处会调用Date类的拷贝构造函数
>  // 但Date类并没有显式定义拷贝构造函数，则编译器会给Date类生成一个默认的拷贝构
> 造函数
> Date d2(d1);
> return 0;
> }
> 
> ```
>
> 注意：<u>**在编译器生成的默认拷贝构造函数中，内置类型是按照字节方式直接拷贝的，而自定 义类型是调用其拷贝构造函数完成拷贝的.**</u>
>
> ④编译器生成的默认拷贝构造函数已经可以完成字节序的值拷贝了，还需要自己显式实现吗？ 当然像日期类这样的类是没必要的。
>
> 注意：类中如果没有涉及资源申请时，拷贝构造函数是否写都可以；一旦涉及到资源申请 时，则拷贝构造函数是一定要写的，否则就是浅拷贝。
>
> 如下：
>
> ![image-20230327214701482](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230327214701482.png)
>
> ⑤拷贝构造函数典型调用场景：
>
> - 实现了析构函数的情况。
>
> - 使用已存在的对象创建新的对象。
>
> - 函数的参数类型为类类型对象。
>
> - 函数的返回值为类类型对象。
>
>   <u>**为了提高程序效率，一般对象传参时，尽量使用引用类型，返回时根据实际场景，能用引用 尽量使用引用**</u>。

## Ⅴ.赋值运算符重载：

> ### Ⅴ. Ⅰ运算符重载：
>
> C++为了增强代码的可读性引入了运算符重载，运算符重载是<u>**具有特殊函数名的函数**</u>，也具有其 返回值类型，函数名字以及参数列表，其返回值类型与参数列表与普通的函数类似。
>
> 函数的名称：<u>**关键字`operator`后面接需要重载的运算符符号**</u>
>
> 函数原型：<u>**返回值类型 `operator`操作符(参数列表)**</u>
>
> <u>**注意：**</u>
>
> - 不能通过连接其他符号来创建新的操作符：比如operator@。
>
> - 重载操作符必须有一个类类型参数。
>
> - 用于内置类型的运算符，其含义不能改变，例如：内置的整型+，不能改变其含义。
>
> - 作为类成员函数重载时，其形参看起来比操作数数目少1，因为成员函数的第一个参数为隐 藏的this。
>
> - `.*   ::   sizeof    ?:   .` 注意以上5个运算符不能重载。这个经常在笔试选择题中出现。
>
>   
>
>   ```c++
>   // 全局的operator==
>   class Date
>   { 
>   public:
>    Date(int year = 1900, int month = 1, int day = 1)
>      {
>           _year = year;
>           _month = month;
>           _day = day;
>      }    
>   //private:
>    int _year;
>    int _month;
>    int _day;
>   };
>   // 这里会发现运算符重载成全局的就需要成员变量是公有的，那么问题来了，封装性如何保证？
>   // 这里其实可以用我们后面学习的友元解决，或者干脆重载成成员函数。
>   bool operator==(const Date& d1, const Date& d2)
>   {
>       return d1._year == d2._year
>      && d1._month == d2._month
>           && d1._day == d2._day;
>   }
>   void Test ()
>   {
>       Date d1(2018, 9, 26);
>       Date d2(2018, 9, 27);
>       cout<<(d1 == d2)<<endl;
>   }
>   
>   ```
>
>   ```C++
>   class Date
>   { 
>   public:
>    Date(int year = 1900, int month = 1, int day = 1)
>   {
>           _year = year;
>           _month = month;
>           _day = day;
>    }
>       
>       // bool operator==(Date* this, const Date& d2)
>       // 这里需要注意的是，左操作数是this，指向调用函数的对象
>       bool operator==(const Date& d2)
>    {
>           return _year == d2._year;
>               && _month == d2._month
>               && _day == d2._day;
>    }
>   private:
>    int _year;
>    int _month;
>    int _day;
>   };
>   ```

### Ⅴ. Ⅱ赋值运算符重载：

> ①赋值运算符重载格式：
>
> - <u>**参数类型**</u>：const T&，传递引用可以提高传参效率。
>
> - <u>**返回值类型**</u>：T&，返回引用可以提高返回的效率，有返回值目的是为了支持连续赋值。
>
> - <u>**检测是否自己给自己赋值。**</u>
>
> - <u>**返回*this**</u> ：要复合连续赋值的含义。
>
>   ```C++
>   class Date
>   { 
>   public :
>    Date(int year = 1900, int month = 1, int day = 1)
>      {
>           _year = year;
>           _month = month;
>           _day = day;
>      }
>        
>    Date (const Date& d)
>      {
>           _year = d._year;
>           _month = d._month;
>           _day = d._day;
>      }
>        
>    Date& operator=(const Date& d)
>    {
>    if(this != &d)
>          {
>               _year = d._year;
>               _month = d._month;
>               _day = d._day;
>          }
>               
>           return *this;
>    }
>   private:
>       int _year ;
>    	int _month ;
>    	int _day ;
>   };
>       
>   ```
>
>   ②<u>**赋值运算符只能重载成类的成员函数不能重载成全局函数。**</u>
>
>   ```C++
>   class Date
>   {
>   public:
>    Date(int year = 1900, int month = 1, int day = 1)
>    {
>    _year = year;
>    _month = month;
>    _day = day;
>    }
>    int _year;
>    int _month;
>    int _day;
>   };
>   // 赋值运算符重载成全局函数，注意重载成全局函数时没有this指针了，需要给两个参数
>   Date& operator=(Date& left, const Date& right)
>   {
>    if (&left != &right)
>    {
>    left._year = right._year;
>    left._month = right._month;
>    left._day = right._day;
>    }
>    return left;
>   }
>   // 编译失败：
>   // error C2801: “operator =”必须是非静态成员
>   ```
>
>   <u>**原因：**</u>赋值运算符如果不显式实现，编译器会生成一个默认的。此时用户再在类外自己实现 一个全局的赋值运算符重载，就和编译器在类中生成的默认赋值运算符重载冲突了，故赋值 运算符重载只能是类的成员函数。
>
>   ③<u>**用户没有显式实现时，编译器会生成一个默认赋值运算符重载，以值的方式逐字节拷贝**</u>。注 意：内置类型成员变量是直接赋值的，而自定义类型成员变量需要调用对应类的赋值运算符 重载完成赋值。
>
>   ```c++
>   class Time
>   {
>   public:
>    Time()
>    {
>    	_hour = 1;
>   	_minute = 1;
>    	_second = 1;
>    }
>    Time& operator=(const Time& t)
>    {
>    if (this != &t)
>    {
>    	_hour = t._hour;
>    	_minute = t._minute;
>    	_second = t._second;
>    }
>    return *this;
>    }
>   private:
>    	int _hour;
>    	int _minute;
>    	int _second;
>   };
>   class Date
>   {
>   private:
>    	// 基本类型(内置类型)
>    	int _year = 1970;
>    	int _month = 1;
>    	int _day = 1;
>    // 自定义类型
>    	Time _t;
>   };
>   int main()
>   {
>    	Date d1;
>    	Date d2;
>    	d1 = d2;
>    	return 0;
>   }
>       
>   ```
>
>   既然编译器生成的默认赋值运算符重载函数已经可以完成字节序的值拷贝了，我们就不需要再去生成了。
>
>   ```C++
>   // 这里会发现下面的程序会崩溃掉？这里就需要我们以后讲的深拷贝去解决。
>   typedef int DataType;
>   class Stack
>   {
>   public:
>    Stack(size_t capacity = 10)
>    {
>    	_array = (DataType*)malloc(capacity * sizeof(DataType));
>    	if (nullptr == _array)
>    	{
>    		perror("malloc申请空间失败");
>    		return;
>    	}
>        	_size = 0;
>    		_capacity = capacity;
>    }
>    void Push(const DataType& data)
>    {
>    	// CheckCapacity();
>    	_array[_size] = data;
>    	_size++;
>    }
>    ~Stack()
>    {
>    	if (_array)
>    	{
>    		free(_array);
>    		_array = nullptr;
>    		_capacity = 0;
>    		_size = 0;
>    	}
>    }
>   	private:
>    		DataType *_array;
>    		size_t _size;
>    		size_t _capacity;
>   };
>   int main()
>   {
>    	Stack s1;
>    	s1.Push(1);
>    	s1.Push(2);
>    	s1.Push(3);
>    	s1.Push(4);
>    	Stack s2;
>    	s2 = s1;
>    	return 0;
>   }
>       
>   ```
>
>   **<u>注意：</u>**如果类中未涉及到资源管理，赋值运算符是否实现都可以；一旦涉及到资源管理则必须要实现。

### Ⅴ. Ⅲ 前置++和后置++重载：

> ```c++
> class Date
> {
> 	public:
> 		Date(int year = 1900, int month = 1, int day = 1)
> 		{
>  			_year = year;
>  			_month = month;
>  			_day = day;
>  		}
>  		// 前置++：返回+1之后的结果
>  		// 注意：this指向的对象函数结束后不会销毁，故以引用方式返回提高效率
>  		Date& operator++()
>  		{
>  			_day += 1;
>  			return *this;
>  		}
>  		// 后置++：
>  		// 前置++和后置++都是一元运算符，为了让前置++与后置++形成能正确重载
>  		// C++规定：后置++重载时多增加一个int类型的参数，但调用函数时该参数不用传递，编译器自动传递
>  		// 注意：后置++是先使用后+1，因此需要返回+1之前的旧值，故需在实现时需要先将this保存一份，然后给this+1
>  		// 而temp是临时对象，因此只能以值的方式返回，不能返回引用
>  		Date operator++(int)
>  		{
>  			Date temp(*this);
>  			_day += 1;
> 			return temp;
>  		}
> 	private:
>  		int _year;
>  		int _month;
>  		int _day;
> };
> int main()
> {
>  	Date d;
>  	Date d1(2022, 1, 13);
>  	d = d1++;    // d: 2022,1,13   d1:2022,1,14
>  	d = ++d1;    // d: 2022,1,15   d1:2022,1,15
>  	return 0;
> }
> ```
>

### Ⅴ. Ⅳ 自定义类型的流插入和流输出：

> 在C++中是默认支持内部类型的流插入和流输出的，那么我们如何实现自定义类型的流插入和流输出呢？
>
> 首先我们观察`<<`和`>>`它和C语言中的左移和右移操作符相同，所以C++中也是将这两个运算符重载，得到流插入和流输出；
>
> ①按照这个思路我们先写出定义；
>
> ```c++
> //这里的返回值类型和形参的类型是因为 cin 和 cout 分别包含在istream和ostream 中，具体的细节可以查看c++官网手册
> ostream& operator << (ostream& out, const Date& d)
> {
> 	 out << d._year << "年" << d._month << "月" << d._day << "日" << endl;
> 	 return out;
> }
> 
> istream& operator >> (istream& in, Date& d)
> {
> 	 in >> d._year >> d._month >> d._day;
> 	 return in;
> }
> ```
>
> ②将函数声明在类的外部：
>
> 这时候因为`Date`类内部的成员是`private:`修饰的,所以我们在类的外部是无法访问的，所以这时候在类的内部声明一个友元函数。
>
> ```c++
> ostream& operator << (ostream& out, const Date& d); 
> istream& operator >> (istream& out,Date& d);
> ```
>
> ③声明友元函数：
>
> ```c++
> friend ostream& operator << (ostream& out, const Date& d);
> friend istream& operator >> (istream& out, Date& d);
> ```
>
> 这时候我们就可以像输入和输出内部类型那样，输出自定义类型`Date`类了：
>
> ```c++
> 	Date d1(2023, 3, 28);
> 	cin >> d1;
> 	cout << d1 << endl;
> ```
>
> 但是我们一般会直接将函数的声明和定义放在一起，声明成一个内联函数，这样可以提高程序运行时候的效率。
>
> ```c++
> inline ostream& operator << (ostream& out, const Date& d)
> {
> 	out << d._year << "年" << d._month << "月" << d._day << "日" << endl;
> 	return out;
> }
> 
> inline istream& operator >> (istream& in, Date& d)
> {
> 	in >> d._year >> d._month >> d._day;
> 	return in;
> }
> ```

## Ⅵ. const 成员：

> <u>将const修饰的“成员函数”</u>称之为const成员函数，const修饰类成员函数，<u>实际修饰该成员函数 隐含的this指针，表明在该成员函数中不能对类的任何成员进行修改。</u>![image-20230401161100626](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230401161100626.png)

## Ⅶ.取地址及const取地址操作符重载

> 这两个默认成员函数一般不用重新定义 ，编译器默认会生成。
>
> ```C++
> class Date
> { 
> public :
>  Date* operator&()
>  {
>  return this ;
>  }
>  const Date* operator&()const
>  {
>  return this ;
>  }
> private :
>  int _year ; // 年
>  int _month ; // 月
>  int _day ; // 日
> }
> ```
>
> 这两个运算符一般不需要重载，使用编译器生成的默认取地址的重载即可，只有特殊情况，才需 要重载，比如想让别人获取到指定的内容！
