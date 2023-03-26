# C++类和对象（上）

## Ⅰ.类的引入

> C语言结构体中只能定义变量，在C++中，结构体内不仅可以定义变量，也可以定义函数。比如：
> 之前在数据结构初阶中，用C语言方式实现的栈，结构体中只能定义变量；现在以C++方式实现，
> 会发现struct中也可以定义函数 ,如下：
>
> ```c++
> typedef int DataType;
> struct Stack
> {
> 	void Init(size_t capacity)
> 	{
> 		_array = (DataType*)malloc(sizeof(DataType) * capacity);
> 		if (nullptr == _array)
> 		{
> 			perror("malloc申请空间失败");
> 			return;
> 		}
> 	_capacity = capacity;
> 	_size = 0;
> 	}
> 	void Push(const DataType& data)
> 	{
> 		// 扩容
> 		_array[_size] = data;
> 		++_size;
> 	}
> 	DataType Top()
> 	{
>     	return _array[_size - 1];
> 	}
> 	void Destroy()
> 	{
> 		if (_array)
> 		{
> 			free(_array);
> 			_array = nullptr;
> 			_capacity = 0;
> 			_size = 0;
> 		}
> 	}
> 	DataType* _array;
> 	size_t _capacity;
> 	size_t _size;
> };
> int main()
> {
> 	Stack s;
> 	s.Init(10);
> 	s.Push(1);
> 	s.Push(2);
> 	s.Push(3);
> 	cout << s.Top() << endl;
> 	s.Destroy();
> 	return 0;
> }
> ```
>
> 

## Ⅱ.类的定义  

> 上面结构体的定义，在C++中更喜欢用class来代替 。
>
> ```c++
> class className
> {
> // 类体：由成员函数和成员变量组成
> }; // 一定要注意后面的分号
> ```
>
> class为定义类的关键字，ClassName为类的名字，{}中为类的主体，**<u>注意类定义结束时后面分号不能省略</u>** 。
>
> 类体中内容称为类的成员：类中的变量称为<u>**类的属性或成员变量**</u>; 类中的函数称为**<u>类的方法或者成员函数</u>**。   

### Ⅱ.Ⅰ 类的两种定义方式：

> ①声明和定义全部放在类体中，需注意：成员函数如果在类中定义，编译器可能会将其当成内
> 联函数处理。
>
> ```c++
> class Person
> {
> public :
> 	void showInfo()
> 	{
> 		cout << _name << _sex << _age << endl;
> 	}
> public:
> 	char* _name;
> 	char* _age;
> 	char* _sex;
> };
> ```
>
> ②类声明放在.h文件中，成员函数定义放在.cpp文件中，注意：成员函数名前需要加类名::  
>
> ![image-20230322204757427](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230322204757427.png)

**<u>注意：</u>**一般会采用第二种方法来定义类;

成员命名规则的建议:

```c++
// 我们看看这个函数，是不是很僵硬？
class Date
{
	public:
	void Init(int year)
	{
		// 这里的year到底是成员变量，还是函数形参？
		year = year;
	}
	private:
	int year;
};
// 所以一般都建议这样
class Date
{
	public:
	void Init(int year)
	{
		_year = year;
	}
	private:
	int _year;
};
// 或者这样
class Date
{
	public:
	void Init(int year)
	{
		mYear = year;
	}
	private:
	int mYear;
};
// 其他方式也可以的，主要看公司要求。一般都是加个前缀或者后缀标识区分就行
```



## Ⅲ.类的访问限定符及封装

### Ⅲ.Ⅰ访问限定符 :

> C++实现封装的方式：用类将对象的属性与方法结合在一块，让对象更加完善，通过访问权限选
> 择性的将其接口提供给外部的用户使用 .
>
> ![image-20230322205634202](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230322205634202.png)

> **<u>【访问限定符说明】</u>**  
>
> ①public修饰的成员在类外可以直接被访问 
>
> ②protected和private修饰的成员在类外<u>**不能直接被访问**</u>(此处protected和private是类似的)  
>
> ③访问权限<u>**作用域从该访问限定符出现的位置开始直到下一个访问限定符出现时为止**</u>  
>
> ④如果后面没有访问限定符，作用域就到 } 即类结束 
>
> ⑤class的默认访问权限为private，struct为public(因为struct要兼容C) 
>
> <u>**注意：**</u>访问限定符只在编译时有用，当数据映射到内存后，没有任何访问限定符上的区别  
>
> 那么C++中struct和class的区别是什么？  
>
> C++需要兼容C语言，所以C++中struct可以当成结构体使用。另外C++中struct还可以用来
> 定义类。和class定义类是一样的，区别是struct定义的类默认访问权限是public，class定义的类
> 默认访问权限是private。注意：在继承和模板参数列表位置，struct和class也有区别。

###  Ⅲ.Ⅱ封装 ：

> <u>**封装：**</u>将数据和操作数据的方法进行有机结合，隐藏对象的属性和实现细节，仅对外公开接口来
> 和对象进行交互。
>
> 封装本质上是一种管理，让用户更方便使用类。比如：对于电脑这样一个复杂的设备，提供给用
> 户的就只有开关机键、通过键盘输入，显示器，USB插孔等，让用户和计算机进行交互，完成日
> 常事务。但实际上电脑真正工作的却是CPU、显卡、内存等一些硬件元件。
>
> 对于计算机使用者而言，不用关心内部核心部件，比如主板上线路是如何布局的，CPU内部是如
> 何设计的等，用户只需要知道，怎么开机、怎么通过键盘和鼠标与计算机进行交互即可。因此**<u>计</u>**
> **<u>算机厂商在出厂时，在外部套上壳子，将内部实现细节隐藏起来，仅仅对外提供开关机、鼠标以</u>**
> **<u>及键盘插孔等，让用户可以与计算机进行交互即可</u>**    
>
> 在C++语言中实现封装，可以通过类将数据以及操作数据的方法进行有机结合，通过访问权限来
> 隐藏对象内部实现细节，控制哪些方法可以在类外部直接被使用 。

## Ⅳ.类的作用域  

> <u>**类定义了一个新的作用域**</u>，类的所有成员都在类的作用域中。在类体外定义成员时，需要使用 ::
> 作用域操作符指明成员属于哪个类域  
>
> ```c++
> class Person
> {
> 	public:
> 	void PrintPersonInfo();
> 	private:
> 		char _name[20];
> 		char _gender[3];
> 		int _age;
> };
> // 这里需要指定PrintPersonInfo是属于Person这个类域
> void Person::PrintPersonInfo()
> {
> 	cout << _name << " "<< _gender << " " << _age << endl;
> }
> ```

## Ⅶ.类的实例化  

> 用类类型创建对象的过程，称为类的实例化  
>
> ①类是对对象进行描述的，是一个模型一样的东西，限定了类有哪些成员，**<u>定义出一个类并没</u>**
> **<u>有分配实际的内存空间来存储它</u>**；比如：入学时填写的学生信息表，表格就可以看成是一个
> 类，来描述具体学生信息  
>
> ②类可以实例化出多个对象 **实例化出的对象 占用实际的物理空间，存储类成员变量**
>
> ③做个比方。<u>**类实例化出对象就像现实中使用建筑设计图建造出房子，类就像是设计图**</u>，只设
> 计出需要什么东西，但是并没有实体的建筑存在，同样类也只是一个设计，实例化出的对象
> 才能实际存储数据，占用物理空间。
>
> ![image-20230322220318413](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230322220318413.png)

## Ⅴ.类对象模型

### Ⅴ.Ⅰ如何计算类对象的大小：

> ```c++
> class A
> {
> 	public:
> 	void PrintA()
> 	{
> 		cout<<_a<<endl;
> 	}
> 	private:
> 		char _a;
> };	
> ```
>
> 为什么成员变量在对象中，成员函数不在对象中呢？
>
> 每个对象成员变量是不一样的，需要独立存储。
>
> 而每个对象调用的成员函数是一样的，放到共享公共区域（<u>**代码段**</u>）
>
> **<u>所以一个类的大小就是该类中“成员变量”之和，当然要注意内存对齐。（跟结构体内存对齐一样）</u>**
>
> **<u>注意空类的大小，空类比较特殊，编译器给了空类一个字节来唯一标识这个类的对象。</u>**

### Ⅴ.Ⅱ结构体内存对齐

> ①第一个成员在与结构体偏移量为0的地址处  
>
> ②其他成员变量要对齐到某个数字（对齐数）的整数倍的地址处。
> 注意：对齐数 = 编译器默认的一个对齐数 与 该成员大小的较小值。VS中默认的对齐数为8
>
> ③结构体总大小为：最大对齐数（所有变量类型最大者与默认对齐参数取最小）的整数倍  
>
> ④如果嵌套了结构体的情况，嵌套的结构体对齐到自己的最大对齐数的整数倍处，结构体的整
> 体大小就是所有最大对齐数（含嵌套结构体的对齐数）的整数倍 

## Ⅵ.this指针

### Ⅵ.Ⅰthis指针的引出：

> 我们先来定义一个日期类`Date`
>
> ```c++
> class Date
> {
> 	public:
> 	void Init(int year, int month, int day)
> 	{
> 		_year = year;
> 		_month = month;
> 		_day = day;
> 	}
> 	void Print()
> 	{
> 		cout <<_year<< "-" <<_month << "-"<< _day <<endl;
> 	}
> 	private:
> 	int _year; // 年
> 	int _month; // 月
> 	int _day; // 日
> };
> int main()
> {
> 	Date d1, d2;
> 	d1.Init(2022,1,11);
> 	d2.Init(2022, 1, 12);
> 	d1.Print();
> 	d2.Print();
> 	return 0;
> }
> ```
>
> 对于上述类，有这样的一个问题：
> Date类中有 Init 与 Print 两个成员函数，函数体中没有关于不同对象的区分，那当d1调用 Init 函
> 数时，该函数是如何知道应该设置d1对象，而不是设置d2对象呢？
> C++中通过引入this指针解决该问题，即：<u>**C++编译器给每个“非静态的成员函数“增加了一个隐藏**</u>
> **<u>的指针参数，让该指针指向当前对象(函数运行时调用该函数的对象)，在函数体中所有“成员变量”</u>**
> <u>**的操作，都是通过该指针去访问。只不过所有的操作对用户是透明的，即用户不需要来传递，编**</u>
> <u>**译器自动完成。**</u>  

### Ⅵ.Ⅱthis指针的特性：

> ①**this指针的类型：类类型* const**，即成员函数中，不能给this指针赋值  
>
> ②只能在“成员函数”的内部使用
>
> ③**this指针本质上是“成员函数”的形参**，当对象调用成员函数时，将对象地址作为实参传递给
> this形参。**所以对象中不存储this指针** 
>
> ④**this指针是“成员函数”第一个隐含的指针形参，一般情况由编译器通过<u>ecx寄存器</u>自动传**
> **递，不需要用户传递**  
>
> ```c++
> 	void showInfo(Person* this)
> 	{
> 		cout << this->_name << this->_sex << this->_age << endl;
> 	}
> ```
>
> ①this指针存在哪里？
>
> 因为this指针是一个隐含形参，所以存储在栈中。vs下通过ecx寄存器存储。
>
> ②this指针可以为空吗？  
>
>  this指针是可以为空的！
>
> ```c++
> // 1.下面程序编译运行结果是？ A、编译报错 B、运行崩溃 C、正常运行
> class A
> {
> 	public:
> 	void Print()
> 	{
> 		cout << "Print()" << endl;
> 	}
> 	private:
> 	int _a;
> };
> int main()
> {
> 	A* p = nullptr;
> 	p->Print();
> 	return 0;
> }
> // 1.下面程序编译运行结果是？ A、编译报错 B、运行崩溃 C、正常运行
> class A
> {
> 	public:
> 	void PrintA()
> 	{
> 		cout<<_a<<endl;
> 	}
> 	private:
> 	int _a;
> };
> int main()
> {
> 	A* p = nullptr;
> 	p->PrintA();
> 	return 0;
> }
> ```
>
> 那么结果是1正常运行，而2运行崩溃，程序是否崩溃的根源是this指针是否会被解引用，当this指针被解引用的时候就会造成运行崩溃。
>
> 所以对于1这种情况来说是没有对this指针进行解引用的，因为Print函数内部是没有涉及到成员对象的。所以编译器运行该程序的时候只会在公共区域即：代码段去寻找这个函数，而不会去解引用this指针。而2情况来说PrintA函数内部涉及到了成员变量a，那么这时候就需要执行this->a也就是对this指针进行解引用，那么这时候就会造成程序运行崩溃。

