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

