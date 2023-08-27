# C++模板

## Ⅰ.非类型模板参数

> 模板的参数分为**类型形参**和**非类型形参**
>
> - 类型形参：出现模板参数列表中，跟在`class`和`typename`之后的参数类型名称
> - 非类型形参：用一个常量来作为类(函数)模板参数，在类(函数)模板内部可以被当作常量来使用。
>
> 例如：
>
> ```cpp
> namespace xupt
> {
>  	//定义一个模板类型的静态数组
>  	template<class T, size_t N = 10>
>  	class array
>  	{
>  		public:
>  			T& operator[](size_t index){return _array[index];}
>  			const T& operator[](size_t index)const{return _array[index];}
>  
>  			size_t size()const{return _size;}
>  			bool empty()const{return 0 == _size;}
>  
>  		private:
>  			T _array[N];
>  			size_t _size;
>  	};
> }
> 
> ```
>
> **注意：**
>
> - 非类型模板参数只能是常量整型（包括枚举）；
> - 非类型模板参数必须在编译前就为一个确定的值；

## Ⅱ.  模板的特化：

> ①什么是模板的特化：
>
> 再类模板或者函数模板的基础上，针对特殊类型所进行的特殊化的实现方式。
>
> ②模板特化分为：
>
> - 类模板特化
> - 函数模板特化
>
> ③那么我们为什么要进行模板的特化
>
> 如下的案例，当我们实现一个小于比较的函数模板。如下：
>
> ```cpp
> template<class T>
> bool Less(const T &A, const T &B)
> {
> 	return A < B;
> }
> ```
>
> 我们发现这个函数大多数情况下都可以实现我们所预期的函数功能，但是当我们向这个函数的两个形参传递的是两个**对象的指针**这时候就会出现错误的结果，因为这时候比较的函数就会产生错误的结果。因为函数模板不会知道你向他传输的是一个指针类型，那么他就会用指针进行比较，从而产生错误的结果。所以我们要实现一个模板的特化：

### Ⅱ. Ⅰ函数模板的特化

> ④函数模板特化的步骤：
>
> - 得先有一个基础的函数模板。
> - `template`只跟一对`<>`。
> - 函数名后需要跟一对`<>`，`<>`内部需要放入需要特化的类型
> - 函数形参表：必须要和模板函数的基础参数类型**完全相同**。
>
> 例如：
>
> ```cpp
> // 函数模板 -- 参数匹配
> template<class T>
> bool Less(T left, T right)
> {
>  	return left < right;
> }
> // 对Less函数模板进行特化
> template<>
> bool Less<Date*>(Date* left, Date* right)
> { 
>     return *left < *right;
> }
> int main()
> {
>  	cout << Less(1, 2) << endl;
>  	Date d1(2022, 7, 7);
>  	Date d2(2022, 7, 8);
>  	cout << Less(d1, d2) << endl;
>  	Date* p1 = &d1;
>  	Date* p2 = &d2;
>  	cout << Less(p1, p2) << endl; // 调用特化之后的版本，而不走模板生成了
>  return 0;
> }
> ```
>
> **注意：**
>
> ​	我们一般直接给出函数，而不选择进行函数模板的特化，因为对于一些参数类型复杂的函数模板，我们选择直接给出函数更加的方便快捷，代码可读性也可以保证。

### Ⅱ .Ⅱ . 类模板的特化

> 类模板的特化：
>
> - 类模板的全特化
> - 类模板的偏特化

#### Ⅱ. Ⅱ . Ⅰ 全特化 

> **全**特化就是将类模板参数中的所有参数都确定：
>
> 例如：
>
> ```cpp
> template<class T1, class T2>
> class Data
> {
> 	public:
>  		Data() {cout<<"Data<T1, T2>" <<endl;}
> 	private:
>  		T1 _d1;
>  		T2 _d2;
> };
> template<>
> class Data<int, char>
> {
> 	public:
>  		Data() {cout<<"Data<int, char>" <<endl;}
> 	private:
>  		int _d1;
>  		char _d2;
> };
> void TestVector()
> {
>  	Data<int, int> d1;
>  	Data<int, char> d2;
> } 
> ```

#### Ⅱ . Ⅱ . Ⅱ偏特化

> **偏**特化：任何针对模版参数进一步进行条件限制设计的特化版本
>
> 例如：
>
> ```cpp
> //模板类例子
> template<class T1, class T2>
> class Data
> {
> public:
>  Data() {cout<<"Data<T1, T2>" <<endl;}
> private:
>  T1 _d1;
>  T2 _d2;
> };
> 
> ```
>
> 偏特化分为两种：
>
> - 部分特化
>
>   将类模板参数表中的的部分进行参数特化
>
>   例如：
>
>   ```cpp
>   // 将第二个参数特化为int
>   template <class T1>
>   class Data<T1, int>
>   {
>   	public:
>    		Data() {cout<<"Data<T1, int>" <<endl;}
>   	private:
>    		T1 _d1;
>    		int _d2;
>   }; 
>   ```
>
> - 参数的进一步限制：
>
>   偏特化并不仅仅是指特化部分参数，而是针对模板参数更进一步的条件限制所设计出来的一个特化版本。
>
>   例如指针，引用等等；
>
>   ```cpp
>   //两个参数偏特化为指针类型
>   template <typename T1, typename T2>
>   class Data <T1*, T2*>
>   { 
>   public:
>    Data() {cout<<"Data<T1*, T2*>" <<endl;}
>      
>   private:
>    T1 _d1;
>    T2 _d2;
>   };
>   //两个参数偏特化为引用类型
>   template <typename T1, typename T2>
>   class Data <T1&, T2&>
>   {
>   public:
>    Data(const T1& d1, const T2& d2)
>    : _d1(d1)
>    , _d2(d2)
>    {
>    cout<<"Data<T1&, T2&>" <<endl;
>    }
>      
>   private:
>    const T1 & _d1;
>    const T2 & _d2; 
>    };
>   void test2 () 
>   {
>    Data<double , int> d1; // 调用特化的int版本
>    Data<int , double> d2; // 调用基础的模板 
>    Data<int *, int*> d3; // 调用特化的指针版本
>    Data<int&, int&> d4(1, 2); // 调用特化的指针版本
>   }
>   ```
>
>   

## Ⅲ . 模板的分离编译：

> 将声明和定义放到一个文件 `"xxx.cpp"` 里面或者`"xxx.h"`.



