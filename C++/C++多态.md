# C++多态

## Ⅰ. 多态的概念

> 多态的概念：通俗来说，就是多种形态，具体点就是去完成某个行为，当不同的对象去完成时会 产生出不同的状态。
>
> 举个例子：当我们买票的时候，学生去买票就是就是学生票，成人去买票就是成人票，儿童去买票就是儿童票。

## Ⅱ.多态的定义及实现

### Ⅱ. Ⅰ多态的构成条件

> 多态是在不同继承关系的类对象，去调用同一函数，产生了不同的行为。比如Student继承了 Person。Person对象买票全价，Student对象买票半价。
>
> 那么在继承中要构成多态还有两个条件：
>
> ①虚函数的重写 ---- **三同(返回值，函数名，参数)**。
>
> ②必须是**父类**的**指针或者引用**去调。
>
> ![image-20230730221817563](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230730221817563.png)

### Ⅱ. Ⅱ 虚函数：

> 虚函数：即被virtual修饰的类成员函数称为虚函数。
>
> ```cpp
> class Person {
> public:
>  virtual void BuyTicket() { cout << "买票-全价" << endl;}
> };
> ```

### Ⅱ. Ⅲ虚函数的重写：

> 虚函数的重写(覆盖)：派生类中有一个跟基类完全相同的虚函数(即派生类虚函数与基类虚函数的 返回值类型、函数名字、参数列表完全相同)，称子类的虚函数重写了基类的虚函数。
>
> ```cpp
> class Person 
> {
> 	public:
>  		virtual void BuyTicket() { cout << "买票-全价" << endl; }
> };
> class Student : public Person {
> 	public:
>  		virtual void BuyTicket() { cout << "买票-半价" << endl; }
> /*注意：在重写基类虚函数时，派生类的虚函数在不加virtual关键字时，虽然也可以构成重写(因
> 为继承后基类的虚函数被继承下来了在派生类依旧保持虚函数属性),但是该种写法不是很规范，不建议
> 这样使用*/
> /*void BuyTicket() { cout << "买票-半价" << endl; }*/
> };
> void Func(Person& p)
>     { p.BuyTicket(); }
> int main()
> {
> 	Person ps;
> 	Student st;
> 	Func(ps);
> 	Func(st);
>  	return 0;
> }
> ```
>
> **虚函数重写的两个例外：**
>
> ①**协变(基类与派生类虚函数返回值类型不同)**
>
> 派生类重写基类虚函数时，与基类虚函数返回值类型不同。即**基类虚函数返回基类对象的指 针或者引用**，**派生类虚函数返回派生类对象的指针或者引用时**，称为协变。（了解）
>
> ```cpp
> class A{};
> class B : public A {};
> class Person {
> public:
>  virtual A* f() {return new A;}
> };
> class Student : public Person {
> public:
>  virtual B* f() {return new B;}
> };
> 
> ```
>
> ②**析构函数的重写(基类与派生类析构函数的名字不同)**
>
> 如果基类的析构函数为虚函数，此时派生类析构函数只要定义，无论是否加virtual关键字， 都与基类的析构函数构成重写，虽然基类与派生类析构函数名字不同。虽然函数名不相同， 看起来违背了重写的规则，其实不然，这里可以理解为编译器对析构函数的名称做了特殊处 理，编译后析构函数的名称统一处理成destructor。
>
> ```cpp
> class Person 
> {
> 	public:
>  		virtual ~Person() {cout << "~Person()" << endl;}
> };
> class Student : public Person {
> 	public:
>  		virtual ~Student() { cout << "~Student()" << endl; }
> };
> // 只有派生类Student的析构函数重写了Person的析构函数，下面的delete对象调用析构函
> //数，才能构成多态，才能保证p1和p2指向的对象正确的调用析构函数。
> int main()
> {
>  	Person* p1 = new Person;
>  	Person* p2 = new Student;
>  	delete p1;
>  	delete p2;
>  	return 0;
> }
> ```

### Ⅱ. Ⅳ C++11 override 和 final：

> 从上面可以看出，C++对函数重写的要求比较严格，但是有些情况下由于疏忽，可能会导致函数 名字母次序写反而无法构成重载，而这种错误在编译期间是不会报出的，只有在程序运行时没有 得到预期结果才来debug会得不偿失，因此：C++11提供了override和final两个关键字，可以帮 助用户检测是否重写。
>
> **①final：修饰虚函数，表示该虚函数不能再被重写**
>
> ```cpp
> class Car
> {
> 	public:
>  		virtual void Drive() final {}
> };
> class Benz :public Car
> {
> 	public:
>  		virtual void Drive() {cout << "Benz-舒适" << endl;}
> };
> 
> ```
>
> **②override: 检查派生类虚函数是否重写了基类某个虚函数，如果没有重写编译报错。**
>
> ```cpp
> class Car{
> public:
>  	virtual void Drive(){}
> };
> class Benz :public Car {
> public:
>  	virtual void Drive() override {cout << "Benz-舒适" << endl;}
> };
> ```

### Ⅱ. Ⅴ 重载、覆盖(重写)、隐藏(重定义)的对比

> ![image-20230730223713615](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230730223713615.png)

## Ⅲ. 抽象类

### Ⅲ. Ⅰ 概念

> 在虚函数的后面写上 =0 ，则这个函数为纯虚函数。**包含纯虚函数的类叫做抽象类（也叫接口 类），抽象类不能实例化出对象**。**派生类继承后也不能实例化出对象，只有重写纯虚函数，派生类才能实例化出对象**(所以是一种强制检测派生类是否重写纯虚函数的方法)。纯虚函数规范了派生类必须重写，另外纯虚函数更体现出了接口继承。
>
> ```cpp
> class Car
> {
> 	public:
> 		virtual void Drive() = 0;
> };
> class Benz :public Car
> {
> 	public:
>  		virtual void Drive()
>  {
>  	cout << "Benz-舒适" << endl;
>  }
> };
> class BMW :public Car
> {
> 	public:
>  		virtual void Drive()
>  {
>  	cout << "BMW-操控" << endl;
>  }
> };
> void Test()
> {
> 	Car* pBenz = new Benz;
>  	pBenz->Drive();
>  	Car* pBMW = new BMW;
>  	pBMW->Drive();
> }
> 
> ```

### Ⅲ. Ⅱ 接口继承和实现继承

> **普通函数的继承是一种实现继承**，派生类继承了基类函数，可以使用函数，继承的是函数的实现。**虚函数的继承是一种接口继承**，派生类继承的是基类虚函数的接口，目的是为了重写，达成 多态，继承的是接口。所以如果不实现多态，不要把函数定义成虚函数。

## Ⅳ. 多态的原理

### Ⅳ. Ⅰ 虚函数表：

> ```cpp
> // 这里常考一道笔试题：sizeof(Base)是多少？
> class Base
> {
> 	public:
>  		virtual void Func1()
>  {
>  	cout << "Func1()" << endl;
>  }
> 	private:
>  		int _b = 1;
> };
> ```
>
> 通过观察测试我们发现b对象是8bytes，除了_b成员，还多一个**__vfptr**放在对象的前面(注意有些 平台可能会放到对象的最后面，这个跟平台有关)，对象中的这个指针我们叫做虚函数表指针(v代 表virtual，f代表function)。一个含有虚函数的类中都至少都有一个虚函数表指针，因为虚函数 的地址要被放到虚函数表中，虚函数表也简称虚表，。那么派生类中这个表放了些什么呢？我们 接着往下分析:
>
> ![image-20230731080317735](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230731080317735.png)
>
> ```cpp
> // 针对上面的代码我们做出以下改造
> // 1.我们增加一个派生类Derive去继承Base
> // 2.Derive中重写Func1
> // 3.Base再增加一个虚函数Func2和一个普通函数Func3
> class Base
> {
> 	public:
>  		virtual void Func1()
>  {
>  	cout << "Base::Func1()" << endl;
>  }
>  virtual void Func2()
>  {
>  	cout << "Base::Func2()" << endl;
>  }
>  void Func3()
>  {
>  	cout << "Base::Func3()" << endl;
>  }
> 	private:
>  		int _b = 1;
> };
> class Derive : public Base
> {
> 	public:
>  		virtual void Func1()
>  		{
>       		cout << "Derive::Func1()" << endl;
>  		}
> 	private:
>  		int _d = 2;
> };
> int main()
> {
>  	Base b;
>  	Derive d;
>  	return 0;
> }
> ```
>
> 通过观察和测试，我们发现了以下几点问题：
>
> ①派生类对象d中也有一个虚表指针，d对象由两部分构成，一部分是父类继承下来的成员，虚 表指针也就是存在部分的另一部分是自己的成员。
>
> ②基类b对象和派生类d对象虚表是不一样的，**这里我们发现`Func1`完成了重写，所以d的虚表 中存的是重写的`Derive::Func1`，所以虚函数的重写也叫作覆盖，覆盖就是指虚表中虚函数 的覆盖**。重写是语法的叫法，覆盖是原理层的叫法。
>
> ③另外`Func2`继承下来后是虚函数，所以放进了虚表，`Func3`也继承下来了，但是不是虚函 数，所以不会放进虚表。
>
> ④ **虚函数表本质是一个存虚函数指针的指针数组，一般情况这个数组最后面放了一个`nullptr`**。
>
> ⑤总结一下派生类的虚表生成：a.先将基类中的虚表内容拷贝一份到派生类虚表中 b.如果派生 类重写了基类中某个虚函数，用派生类自己的虚函数覆盖虚表中基类的虚函数 c.派生类自己 新增加的虚函数按其在派生类中的声明次序增加到派生类虚表的最后。
>
> ⑥这里还有一个童鞋们很容易混淆的问题：虚函数存在哪的？虚表存在哪的？ 答：虚函数存在虚表，虚表存在对象中。注意上面的回答的错的。但是很多童鞋都是这样深以为然的。注意 虚表存的是虚函数指针，不是虚函数，虚函数和普通函数一样的，都是存在代码段的，只是 他的指针又存到了虚表中。另外对象中存的不是虚表，存的是虚表指针。那么虚表存在哪的 呢？实际我们去验证一下会发现vs下是存在代码段的，Linux g++下大家自己去验证？

### Ⅳ. Ⅱ 多态的原理

> 上面分析了这个半天了那么多态的原理到底是什么？还记得这里`Func`函数传`Person`调用的 `Person::BuyTicket`，传Student调用的是`Student::BuyTicket`.![image-20230731090808387](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230731090808387.png)
> ```cpp
> class Person 
> {
> 	public:
>  		virtual void BuyTicket() { cout << "买票-全价" << endl; }
> };
> class Student : public Person 
> {
> 	public:
>  		virtual void BuyTicket() { cout << "买票-半价" << endl; }
> };
> void Func(Person& p)
> {
> 	p.BuyTicket();
> }
> int main()
> {
> 	Person Mike;
>  	Func(Mike);
> 	Student Johnson;
> 	Func(Johnson);
>  	return 0;
> }
> ```
>
> ①观察下图的红色箭头我们看到，p是指向mike对象时，p->BuyTicket在mike的虚表中找到虚 函数是Person::BuyTicket。
>
> ②观察下图的蓝色箭头我们看到，p是指向johnson对象时，p->BuyTicket在johson的虚表中 找到虚函数是Student::BuyTicket。
>
> ③这样就实现出了不同对象去完成同一行为时，展现出不同的形态。
>
> ④反过来思考我们要达到多态，有两个条件，一个是虚函数覆盖，一个是对象的指针或引用调 用虚函数。反思一下为什么？
>
> ⑤再通过下面的汇编代码分析，看出满足多态以后的函数调用，不是在编译时确定的，是运行 起来以后到对象的中取找的。不满足多态的函数调用时编译时确认好的。
>
> ![image-20230731091039323](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230731091039323.png)

### Ⅳ. Ⅲ 动态绑定与静态绑定

> ①静态绑定又称为前期绑定(早绑定)，在程序编译期间确定了程序的行为，也称为静态多态， 比如：函数重载
>
> ②动态绑定又称后期绑定(晚绑定)，是在程序运行期间，根据具体拿到的类型确定程序的具体 行为，调用具体的函数，也称为动态多态。
>
> ③本小节之前(5.2小节)买票的汇编代码很好的解释了什么是静态(编译器)绑定和动态(运行时)绑定。

## Ⅴ. 单继承和多继承虚函数表的关系

> 需要注意的是在单继承和多继承关系中，下面我们去关注的是派生类对象的虚表模型，因为基类 的虚表模型前面我们已经看过了，没什么需要特别研究的

### Ⅴ. Ⅰ 单继承中的虚函数表

> ```cpp
> class Base { 
> public :
>  virtual void func1() { cout<<"Base::func1" <<endl;}
>  virtual void func2() {cout<<"Base::func2" <<endl;}
> private :
>  int a;
> };
> class Derive :public Base { 
> public :
>  virtual void func1() {cout<<"Derive::func1" <<endl;}
>  virtual void func3() {cout<<"Derive::func3" <<endl;}
>  virtual void func4() {cout<<"Derive::func4" <<endl;}
> private :
>  int b;
> };
> ```
>
> 观察下图中的监视窗口中我们发现看不见func3和func4。这里是编译器的监视窗口故意隐藏了这 两个函数，也可以认为是他的一个小bug。那么我们如何查看d的虚表呢？下面我们使用代码打印 出虚表中的函数。![image-20230731093846804](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230731093846804.png)	
> ```cpp
> typedef void(*VFPTR) ();
> void PrintVTable(VFPTR vTable[])
> {
> // 依次取虚表中的虚函数指针打印并调用。调用就可以看出存的是哪个函数
>  cout << " 虚表地址>" << vTable << endl;
>  for (int i = 0; vTable[i] != nullptr; ++i)
>  {
>  printf(" 第%d个虚函数地址 :0X%x,->", i, vTable[i]);
>  VFPTR f = vTable[i];
>  f();
>       }
>  cout << endl;
> }
> int main()
> {
>  Base b;
>  Derive d;
> // 思路：取出b、d对象的头4bytes，就是虚表的指针，前面我们说了虚函数表本质是一个存虚函数
> //指针的指针数组，这个数组最后面放了一个nullptr
> // 1.先取b的地址，强转成一个int*的指针
> // 2.再解引用取值，就取到了b对象头4bytes的值，这个值就是指向虚表的指针
> // 3.再强转成VFPTR*，因为虚表就是一个存VFPTR类型(虚函数指针类型)的数组。
> // 4.虚表指针传递给PrintVTable进行打印虚表
> // 5.需要说明的是这个打印虚表的代码经常会崩溃，因为编译器有时对虚表的处理不干净，虚表最
> //后面没有放nullptr，导致越界，这是编译器的问题。我们只需要点目录栏的-生成-清理解决方案，再
> //编译就好了。
>  VFPTR* vTableb = (VFPTR*)(*(int*)&b);
>  PrintVTable(vTableb);
>  VFPTR* vTabled = (VFPTR*)(*(int*)&d);
>  PrintVTable(vTabled);
>  return 0;
> }
> ```
>
> ![](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230731101946344.png)

### Ⅴ. Ⅱ 多继承中的虚函数表

> ```cpp
> class Base1 {
> public:
>  	virtual void func1() {cout << "Base1::func1" << endl;}
>  	virtual void func2() {cout << "Base1::func2" << endl;}
> private:
>  	int b1;
> };
> class Base2 {
> public:
>  	virtual void func1() {cout << "Base2::func1" << endl;}
>  	virtual void func2() {cout << "Base2::func2" << endl;}
> private:
>  	int b2;
> };
> class Derive : public Base1, public Base2 {
> public:
>  	virtual void func1() {cout << "Derive::func1" << endl;}
>  	virtual void func3() {cout << "Derive::func3" << endl;}
> private:
>  	int d1;
> };
> typedef void(*VFPTR) ();
> void PrintVTable(VFPTR vTable[])
> {
>  	cout << " 虚表地址>" << vTable << endl;
>  	for (int i = 0; vTable[i] != nullptr; ++i)
>  	{
>  		printf(" 第%d个虚函数地址 :0X%x,->", i, vTable[i]);
>  		VFPTR f = vTable[i];
>  		f();
>  	}
>  	cout << endl;
> }
> int main()
> {
>  	Derive d;
>  	VFPTR* vTableb1 = (VFPTR*)(*(int*)&d);
>  	PrintVTable(vTableb1);
>  	VFPTR* vTableb2 = (VFPTR*)(*(int*)((char*)&d+sizeof(Base1)));
>  	PrintVTable(vTableb2);
>  	return 0;
> }
> 
> ```
>
> ![image-20230731124321778](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230731124321778.png)

