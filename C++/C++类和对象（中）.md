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

### Ⅱ.Ⅰ概念：

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
>     	_year = year;
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

### Ⅱ.Ⅱ特性：

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

### Ⅲ.Ⅰ概念：

> **<u>析构函数：</u>**与构造函数功能相反，析构函数不是完成对对象本身的销毁，局部对象销毁工作是由 编译器完成的。而对象在销毁时会自动调用析构函数，完成对象中资源的清理工作。

### Ⅲ.Ⅱ特性：

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

### Ⅳ.Ⅰ概念：

> 在现实生活中，可能存在一个与你一样的自己，我们称其为双胞胎。那么我们在C++中创建对象的时候是否可以创建一个和已存在对象相同的对象呢？
>
> 这时候就要用到拷贝构造函数了。
>
> <u>**拷贝构造函数：**</u>只有<u>**单个形参**</u>，该形参是对本类类型对象的<u>**引用(一般常用const修饰)**</u>，在用<u>**已存在的类类型对象创建新对象时由编译器自动调用**</u>。

### Ⅳ.Ⅱ特征：

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
>  	Date(const Date& d)   // 正确写法
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
> ![image-20230327135124078](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230327135124078.png)
>
> ③若未显式定义，编译器会生成默认的拷贝构造函数。 默认的拷贝构造函数对象按内存存储按 字节序完成拷贝，这种拷贝叫做浅拷贝，或者值拷贝。
>
> ```c++
> class Time
> {
> 	public:
>  	Time()
>  	{
>  		_hour = 1;
>  		_minute = 1;
>  		_second = 1;
> }
>  Time(const Time& t)
>  {
>  _hour = t._hour;
>  _minute = t._minute;
>  _second = t._second;
>  cout << "Time::Time(const Time&)" << endl;
>  }
> private:
>  int _hour;
>  int _minute;
>  int _second;
> };
> class Date
> {
> private:
>  // 基本类型(内置类型)
>  int _year = 1970;
>  int _month = 1;
>  int _day = 1;
>  // 自定义类型
>  Time _t;
> };
> int main()
> {
>  Date d1;
>     
>     // 用已经存在的d1拷贝构造d2，此处会调用Date类的拷贝构造函数
>     // 但Date类并没有显式定义拷贝构造函数，则编译器会给Date类生成一个默认的拷贝构
> 造函数
>  Date d2(d1);
>  return 0;
> }
> 
> ```
>
> 注意：在编译器生成的默认拷贝构造函数中，内置类型是按照字节方式直接拷贝的，而自定 义类型是调用其拷贝构造函数完成拷贝的.
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
> - 使用已存在的对象创建新的对象。
>
> - 函数的参数类型为类类型对象。
>
> - 函数的返回值为类类型对象。
>
>   <u>**为了提高程序效率，一般对象传参时，尽量使用引用类型，返回时根据实际场景，能用引用 尽量使用引用**</u>。