# C++类的模拟实现

[toc]

----



## Ⅰ.默认成员函数部分

> ①构造函数
>
> 选择使用了带有缺省参数的构造函数写法：当没有传入字符串的时候，选择将string中的字符串初始化为空串。
>
> 初始化列表部分只对_size进行了初始化。
>
> ```Cpp
> string(const char* str = "")
> :_size(strlen(str))
> {
> 	_capacity = _size == 0 ? 3 : _size;
> 	_str = new char[_capacity + 1];//+1因为要多以\0
> 	strcpy(_str, str);
> }
> ```
>
> ②拷贝构造函数
>
> 拷贝构造函数是相当重要的，因为默认生成的拷贝构造函数只能实现浅拷贝，所以我们要自己来实现。
>
> 因为string类中涉及到了指针指向空间的问题所以在拷贝的时候也是需要新new一个空间出来的，这样就不会造成一份空间被析构两次的情况了
>
> ```c++
> string(const string& s)
> :_size(s._size)
> ,_capacity(s._capacity)
> {
> 	_str = new char[_capacity + 1];
> 	strcpy(_str, s._str);
> }
> ```
>
> ③析构函数
>
> 析构函数用于释放和清理string类实例化产生的空间。
>
> ```cpp
> ~string()
> {
> 	delete[] _str;
> 	_str = nullptr;
> 	_capacity = 0;
> 	_size = 0;
> }
> ```

## Ⅱ. 常用成员函数的实现

> - c_str()
>
>   用于返回string的字符串内容的函数。
>
>   ```cpp
>   char* c_str()
>   {
>   	return _str;
>   }
>   ```
>
> - swap()
>
>   用于交换两个string类的函数
>
>   ```c++
>   void swap(string& s)
>   {
>   	std::swap(_str, s._str);
>   	std::swap(_size, s._size);
>   	std::swap(_capacity, s._capacity);
>   }
>   ```
>
> - find()
>
>   ①从`pos`位置开始在字符串寻找和`ch`相同的字符，并返回它第一次出现的位置
>
>   ② 从`pos`位置开始在字符串寻找和`str`相同的字符串，并返回它第一次出现的位置
>
>   ```c++
>   size_t find(char ch, size_t pos = 0)
>   {
>   	assert(pos <= _size);
>   	for (int i = pos; i < _size; i++)
>   	{
>   		if (_str[i] == ch)
>   		{
>   			return i;
>   		}
>   	}
>   		return npos;
>   }
>   size_t find(const char* str, size_t pos = 0)
>   {
>   	assert(pos <= _size);
>   			
>   	char* p = strstr(_str + pos, str);
>   	if (p == nullptr)
>   	{
>   		return npos;
>   	}
>   	else
>   	{
>   		return p - _str;
>   	}
>   }
>   ```
>
> - clear()
>
>   用于将string中的字符串清空的函数
>
>   ```c++
>   void clear()
>   {
>   	_str[0] = '\0';
>   	_size = 0;
>   }
>   ```
>
> - reserve()
>
>   用于改变string容量的函数，一般用于扩容。
>
>   ```c++
>   void reserve(size_t n)
>   {
>   	char* temp = new char[n + 1];
>   	strcpy(temp, _str);
>   	delete[] _str;
>   	_str = temp; 
>   	_capacity = n;
>   }
>   ```
>
> - insert()
>
>   向字符串的`pos`位置插入字符或者字符串。
>
>   ```C++
>   string& insert(size_t pos, char ch)
>   {
>   	assert(pos <= _size);
>   	if (_size + 1 > _capacity)
>   	{
>   		reserve(2 * _capacity);
>   	}
>   	////错误问题会导致死循环
>   	//size_t end = _size;
>   	//while (end > pos)
>   	//{
>   	//	_str[end + 1] = _str[end];
>   	//	--end;
>   	//}
>   
>   	size_t end = _size + 1;
>   	while (end > pos)
>   	{
>   		_str[end] = _str[end - 1];
>   		--end;
>   	}
>   		_str[pos] = ch;
>   		++_size;
>   
>   		return *this;
>   	}
>   	string& insert(size_t pos, const char* str)
>   	{
>   		assert(pos <= _size);
>   		size_t len = strlen(str);
>   		//挪动数据
>   		size_t end = _size + len;
>   		if (_size + len > _capacity)
>   		{
>   			reserve(_size + len);
>   		}
>   		while (end - len  + 1 > pos)
>   		{
>   			_str[end] = _str[end - len];
>   			--end;
>   		}
>   		_size += len;
>   		//拷贝数据
>   		strncpy(_str + pos, str, len);
>   		return *this;
>   	}
>   ```
>
> - erase()
>
>   将字符串`pos`位置以及其之后的`len`长度的字符串都都删掉
>
>   ```c++
>   string& erase(size_t pos, size_t len = npos)
>   {
>   	if (len == npos || pos + len >= _size)
>   	{
>   		_str[pos] = '\0';
>   		_size = pos;
>   	}
>   	else
>   	{
>   		strcpy(_str + pos, _str + pos + len);
>   		_size -= len;
>   	}
>   	return *this;
>   }
>   ```
>
> - resize()
>
>   改变字符串的大小，这里给了一个缺省参数，也就是当我们是要增大字符串的大小的时候可以给这个缺省值也可以不给。
>
>   ```c++
>   void resize(size_t n, char ch = '\0')
>   {
>   	if (n < _size)
>   	{	
>   		_size = n;
>   		_str[n] = '\0';
>   	}
>   	else if(n > _size)
>   	{
>   		if (n > _capacity)
>   		{
>   			reserve(n);
>   		}
>   		int i = _size;
>   		while (i < n)
>   		{
>   			_str[i] = ch;
>   			++i;
>   		}
>   		_size = n;
>   		_str[n] = '\0';
>   		}
>   }
>   ```
>
> - pushback()
>
>   向字符出串尾部插入字符
>
>   ```c++
>   void push_back(char ch)
>   {
>   	//if (_size + 1 > _capacity)
>   	//{
>   	//	reserve(2 * _capacity);
>   	//}
>   	//_str[_size] = ch;
>   	//++_size;
>   	//_str[_size] = '\0';
>   	////apend(&ch);
>   	
>   	//insert函数的复用
>   	insert(_size, ch);
>   }
>   ```
>
> - apend()
>
>   向字符串尾部追加字符串
>
>   ```c++
>   void apend(const char* str)
>   {
>   	//int len = strlen(str);
>   	//if (_size + len > _capacity)
>   	//{
>   	//	reserve(_size + len);
>   	//}
>   	//strcpy(_str + _size, str);
>   	//_size += len;
>       //insert函数的复用
>   	insert(_size, str);
>   }
>   ```

## Ⅲ. 运算符以及输入输出流的重载

> ①运算符重载
>
> ```c++
> bool operator<(const string& s) const
> {
> 	return strcmp(_str, s._str) < 0;
> }
> bool operator==(const string& s) const
> {
> 	return strcmp(_str, s._str) == 0;
> }
> bool operator>(const string& s) const
> {
> 	return !(*this < s  && *this == s);
> }
> bool operator>=(const string& s) const
> {
> 	return *this > s || *this == s;
> }
> bool operator<=(const string& s) const
> {
> 	//这里给函数加上const才可以交换s和*this的值
> 	//因为不加const的话相当于是用s去调==的函数
> 	//但是因为s是一个const类型的，所以是权限的放大
> 	//所以结论就是对于不修改成员变量的函数最好加上const
> 	return *this < s || s == *this;
> }
> bool operator!=(const string& s) const
> {
> 	return !(*this == s);
> }
> string& operator=(const string& s)
> {
> 	if (this != &s)
> 	{
> 		//delete[] _str;
> 		//_str = new char[_capacity + 1];
> 		//strcpy(_str, s._str);
> 		//_capacity = s._capacity;
> 		//_size = s._size;
> 
> 		char* temp = new char[_capacity + 1];
> 		strcpy(temp, s._str);
> 		delete[] _str;
> 		_str = temp;
> 		_capacity = s._capacity;
> 		_size = s._size;
> 	 }
> 	return *this;
> }
> ```
>
> ②流插入和流输出重载
>
> ```c++
> 	ostream& operator << (ostream& out, const string& s)
> 	{
> 		for (auto ch : s)
> 		{
> 			out  << ch;
> 		}
> 		return out;
> 	}
> 
> 	istream& operator >> (istream& in,  string& s)
> 	{
> 		s.clear();
> 		char ch = in.get();
> 		char buff[128];
> 		size_t i = 0;
> 		while (ch != ' ' && ch != '\n')
> 		{
> 				buff[i++] = ch;
> 				if (i == 127)
> 				{
> 						buff[127] = '\0';
> 						s += buff;
> 						i = 0;
> 				}	
> 			
> 				ch = in.get();
> 		}
> 		if (i != 0)
> 		{
> 			buff[i] = '\0';
> 			s += buff;
> 		}
> 		return in;
> 	}
> ```

## Ⅳ. 迭代器实现

> 这里只是实现了正向迭代器的实现
>
> ```C++
> 		typedef char* iterator;
> 		typedef const char* const_iterator;
> 
> 		iterator begin()
> 		{
> 			return _str;
> 		}
> 		iterator end()
> 		{
> 			return _str + _size;
> 		}
> 
> 		const_iterator begin() const 
> 		{
> 			return _str;
> 		}
> 		const_iterator end() const
> 		{
> 			return _str + _size;
> 		}
> ```
>
> 