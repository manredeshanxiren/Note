# C++单例模式

**什么是单例模式：**一个类只能创建一个对象，即单例模式，该模式可以保证系统中该类只有一个实例，并提供一个
访问它的全局访问点，该实例被所有程序模块共享  

**单例模式的本质**，保证一些数据(一个进程)全局只有一份，并且方便访问。

## 1.单例模式实现的方式

### 1.1饿汉模式

> 不管后续是否要使用，在程序启动的时候都会创建一个唯一的实例对象。
>
> ```c++
> class Singleton
> {
> public:
> 	static Singleton* GetInstance()
> 	{
> 		return &m_instance;
> 	}
> 
> private:
> 	// 构造函数私有
> 	Singleton(){};
> 	// C++98 防拷贝
> 	Singleton(Singleton const&);
> 	Singleton& operator=(Singleton const&);
> 	// or
> 	// C++11
> 	Singleton(Singleton const&) = delete;
> 	Singleton& operator=(Singleton const&) = delete;
> 	static Singleton m_instance;
> };
> 	Singleton Singleton::m_instance; // 在程序入口之前就完成单例对象的初始化
> ```
>

### 1.2懒汉模式

> 懒汉也就是延迟加载，在程序使用的时候才创建唯一的实例。
>
> ```c++
> #define _CRT_SECURE_NO_WARNINGS 1
> // 懒汉
> // 优点：第一次使用实例对象时，创建对象。进程启动无负载。多个单例实例启动顺序自由控制。
> // 缺点：复杂
> #include <iostream>
> #include <mutex>
> #include <thread>
> using namespace std;
> class Singleton
> {
> public:
>     static Singleton* GetInstance() {
>         // 注意这里一定要使用Double-Check的方式加锁，才能保证效率和线程安全
>         if (nullptr == m_pInstance) {
>             m_mtx.lock();
>             if (nullptr == m_pInstance) {
>                 m_pInstance = new Singleton();
>             }
>             m_mtx.unlock();
>         }
>         return m_pInstance;
>     }
>     // 实现一个内嵌垃圾回收类
>     class CGarbo {
>     public:
>         ~CGarbo() {
>             if (Singleton::m_pInstance)
>                 delete Singleton::m_pInstance;
>         }
>     };
>     // 定义一个静态成员变量，程序结束时，系统会自动调用它的析构函数从而释放单例对象
>     static CGarbo Garbo;
> private:
>     // 构造函数私有
>     Singleton() {};
>     // 防拷贝
>     Singleton(Singleton const&);
>     Singleton& operator=(Singleton const&);
>     static Singleton* m_pInstance; // 单例对象指针
>     static mutex m_mtx; //互斥锁
> };
> Singleton* Singleton::m_pInstance = nullptr;
> Singleton::CGarbo Garbo;
> mutex Singleton::m_mtx;
> int main()
> {
>     thread t1([] {cout << Singleton::GetInstance() << endl; });
>     thread t2([] {cout << Singleton::GetInstance() << endl; });
>     t1.join();
>     t2.join();
>     cout << Singleton::GetInstance() << endl;
>     cout << Singleton::GetInstance() << endl;
>     return 0;
> }
> ```

### 1.3懒汉和恶汉的优缺点

|      |             优点             |                             缺点                             |
| :--: | :--------------------------: | :----------------------------------------------------------: |
| 懒汉 | 懒汉完美解决了饿汉缺点的问题 |                           实现复杂                           |
| 饿汉 |    相对于懒汉而言简单一些    | 1.如果单例对象初始化很慢(如初始化动作多，还会伴随一些IO行为，如读取配置文件等等)，main函数之前就要申请，第一，暂时不需要使用却占用资源，第二程序启动会受影响。2.如果两个单例都是饿汉并且有依赖关系，要求创建单例1，再创建单例2，饿汉无法控制顺序，懒汉才可以。 |