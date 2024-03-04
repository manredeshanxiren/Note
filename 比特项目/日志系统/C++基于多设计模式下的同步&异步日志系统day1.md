# C++基于多设计模式下的同步&异步日志系统day1

## 1.项目介绍

### 1.1项目功能

本项⽬主要实现⼀个⽇志系统，其主要⽀持以下功能:  

- ⽀持多级别⽇志消息  
- ⽀持同步⽇志和异步⽇志  
- ⽀持可靠写⼊⽇志到控制台、⽂件以及滚动⽂件中
- ⽀持多线程程序并发写⽇志    
- ⽀持扩展不同的⽇志落地⽬标地

### 1.2开发环境

- CentOS7  
- vscode/vim  
- g++/gdb  
- Makefile

### 1.3核心技术

- 类层次设计(继承和多态的应⽤)
- C++11(多线程、auto、智能指针、右值引⽤等) 
- 双缓冲区  
- ⽣产消费模型  
- 多线程  
- 设计模式(单例、⼯⼚、代理、模板等)  

### 1.4环境搭建

本项目不依赖于其他任何第三方库，只需要安装好CentOS/Ubuntu+vscode/vim环境即可开发。

## 2.日志系统的介绍

### 2.1什么是日志？

> **日志：**程序运行过程中所记录的程序运行状态信息。
>
> - 作用：记录了程序的运行状态信息，以便于程序员能够随时根据状态信息，对系统的运行状态，进行分析

### 2.2为什么需要日志系统

- ⽣产环境的产品为了保证其稳定性及安全性是不允许开发⼈员附加调试器去排查问题，可以借助⽇
  志系统来打印⼀些⽇志帮助开发⼈员解决问题  
- 上线客⼾端的产品出现bug⽆法复现并解决，可以借助⽇志系统打印⽇志并上传到服务端帮助开发
  ⼈员进⾏分析 
- 对于⼀些⾼频操作（如定时器、⼼跳包）在少量调试次数下可能⽆法触发我们想要的⾏为，通过断
  点的暂停⽅式，我们不得不重复操作⼏⼗次、上百次甚⾄更多，导致排查问题效率是⾮常低下，可
  以借助打印⽇志的⽅式查问题
-  在分布式、多线程/多进程代码中，出现bug⽐较难以定位，可以借助⽇志系统打印log帮助定位
  bug  
-  帮助⾸次接触项⽬代码的新开发⼈员理解代码的运⾏流程 

### 2.3日志系统的技术实现

⽇志系统的技术实现主要包括三种类型 :

- 利⽤`printf、std::cout`等输出函数将⽇志信息打印到控制台  

- 对于⼤型商业化项⽬，为了⽅便排查问题，我们⼀般会将⽇志输出到⽂件或者是数据库系统⽅便查
  询和分析⽇志，主要分为同步⽇志和异步⽇志⽅式  

  a.同步写⽇志  

  b.异步写⽇志  

#### 2.3.1同步写日志

同步⽇志是指当输出⽇志时，必须等待⽇志输出语句执⾏完毕后，才能执⾏后⾯的业务逻辑语句，⽇
志输出语句与程序的业务逻辑语句将在同⼀个线程运⾏。每次调⽤⼀次打印⽇志API就对应⼀次系统调
⽤write写⽇志⽂件。

![image-20240304093855106](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240304093855106.png)

在⾼并发场景下，随着⽇志数量不断增加，同步⽇志系统容易产⽣系统瓶颈：

- ⼀⽅⾯，⼤量的⽇志打印陷⼊等量的write系统调⽤，有⼀定系统开销.  
- 另⼀⽅⾯，使得打印⽇志的进程附带了⼤量同步的磁盘IO，影响程序性能.

#### 2.3.2异步写日志

异步⽇志是指在进⾏⽇志输出时，⽇志输出语句与业务逻辑语句并不是在同⼀个线程中运⾏，⽽是**有专⻔的线程⽤于进⾏⽇志输出操作**。业务线程只需要将⽇志放到⼀个内存缓冲区中不⽤等待即可继续执⾏后续业务逻辑（作为⽇志的⽣产者），⽽⽇志的落地操作交给单独的⽇志线程去完成（作为⽇志的消费者）,这是⼀个**典型的⽣产-消费模型**。

![image-20240304093326776](C:/Users/jason/AppData/Roaming/Typora/typora-user-images/image-20240304093326776.png)

这样做的好处是即使⽇志没有真的地完成输出也不会影响程序的主业务，可以提⾼程序的性能：

- 主线程调⽤⽇志打印接⼝成为⾮阻塞操作  
- 同步的磁盘IO从主线程中剥离出来交给单独的线程完成  

## 3.相关技术知识补充

### 3.1不定参函数

在初学C语⾔的时候，我们都⽤过`printf`函数进⾏打印。其中`printf`函数就是⼀个不定参函数，在函数内
部可以根据格式化字符串中格式化字符分别获取不同的参数进⾏数据的格式化。  

**不定参宏函数**

```cpp
#include<iostream>
#include<cstdarg>

#define LOG(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
//其中最后一个参数__VA_ARGS__,前的##作用是当我们只有一个参数的时候,编译器自动忽略从最后一个","开始的内容;
int main()
{
    LOG("%s-%s", "hello", "西安邮电大学");
    return 0;
}
```

**C风格不定参函数**

①一个打印数字的例子：

```cpp
#include<iostream>
#include<cstdarg>

void printNum(int n, ...)
{
    va_list ap;
    va_start(ap, n); //使得ap指向参数n后面的第一个可变参数
    for(int i = 0; i < n; ++i)
    {
        int num = va_arg(ap, int); //从可变参数中获取每一个整形参数
        std::cout << num << " ";
    }
    std::cout << std::endl;
    va_end(ap); //清空可变参数列表--->将ap置空,防止内存泄漏
}


int main()
{
    printNum(2, 1, 2);
    printNum(5, 78, 25, 37, 43, 55);
    return 0;
}

//运行效果：
//[mi@lavm-5wklnbmaja lesson1]$ ./args 
//1 2 
//78 25 37 43 55 
```

②实现一个`printf`

```c
#include<iostream>
#include<cstdarg>

void myprintf(const char* fmt, ...)
{
    char *res;
    va_list ap;
    va_start(ap, fmt); 
    int ret = vasprintf(&res, fmt, ap); //调用vasprintf函数将不定参列表中的参数按照fmt提供的格式，整合成一个字符串;
    if(ret != -1)  //vasprintf函数失败的时候返回-1
    std::cout << res << std::endl;
    va_end(ap);
    free(res);
}

int main()
{
    myprintf("西安邮电大学");
    myprintf("%s-%d", "西安邮电大学",666);
    return 0;
}

//运行效果：
//[mi@lavm-5wklnbmaja lesson1]$ ./args 
//西安邮电大学
//西安邮电大学-666
```

**C++风格不定参函数**

```cpp
#include<iostream>
#include<cstdarg>

void xprintf()
{
    std::cout << std::endl;
}
template<typename T, typename ...Args>
void xprintf(const T & val, Args &&...args) //这里使用右值引用目的是为了使用完美转发
{
    std::cout << val << " ";
    if((sizeof ...(args)) > 0)
    {
        xprintf(std::forward<Args>(args)...); //完美转发，传参的过程中保留对象原生类型属性
    }
    else 
    {
        xprintf();
    }
}

int main()
{
    xprintf("西安邮电大学");
    xprintf("西安邮电大学", "666");
    xprintf("西安邮电大学", "666", "旭日苑");
    return 0;
}

//运行效果
//[root@lavm-5wklnbmaja lesson1]# ./args 
//西安邮电大学 
//西安邮电大学 666 
//西安邮电大学 666 旭日苑 
```

## 4.设计模式

### 4.1六大原则

- 单一职责原则（SingleResponsibilityPrinciple  ）；

  a.类的职责应该单⼀，⼀个⽅法只做⼀件事。职责划分清晰了，每次改动到最⼩单位的⽅法或类。

  b.使⽤建议：两个完全不⼀样的功能不应该放⼀个类中，⼀个类中应该是⼀组相关性很⾼的函数、数据的封装。

  c.例子：⽹络聊天：⽹络通信&聊天，应该分割成为⽹络通信类&聊天类  

- 开闭原则（OpenClosedPrinciple  ）

  a.对扩展开放，对修改封闭  

  b.使⽤建议：对软件实体的改动，最好⽤扩展⽽⾮修改的⽅式  

  c.⽤例：超时卖货：商品价格---不是修改商品的原来价格，⽽是新增促销价格  

- 里氏替换原则（LiskovSubstitutionPrinciple  ）

  a.通俗点讲，就是只要⽗类能出现的地⽅，⼦类就可以出现，⽽且替换为⼦类也不会产⽣任何错误或异常  

  b.在继承类时，务必重写⽗类中所有的⽅法，尤其需要注意⽗类的protected⽅法，⼦类尽量不要暴露⾃⼰的public⽅法供外界调⽤ 

  c.使⽤建议：⼦类必须完全实现⽗类的⽅法，孩⼦类可以有⾃⼰的个性。覆盖或实现⽗类的⽅法时，输⼊参数可以被放⼤，输出可以缩⼩ 

  d.⽤例：跑步运动员类-会跑步，⼦类⻓跑运动员-会跑步且擅⻓⻓跑，⼦类短跑运动员-会跑步且擅⻓短跑  

- 依赖倒置原则（Dependence Inversion Principle）

​	   a.⾼层模块不应该依赖低层模块，两者都应该依赖其抽象.不可分割的原⼦逻辑就是低层模式，原⼦逻辑组装成的就是⾼层模块  

​	   b.模块间依赖通过抽象（接⼝）发⽣，具体类之间不直接依赖  

​       c.使⽤建议：每个类都尽量有抽象类，任何类都不应该从具体类派⽣。尽量不要重写基类的⽅法。结合⾥⽒替换原则使⽤

​	   d.⽤例：奔驰⻋司机类--只能开奔驰；司机类--给什么⻋，就开什么⻋；开⻋的⼈：司机--依赖于抽象    

- 迪⽶特法则（LawofDemeter），⼜叫“最少知道法则”  

  a.尽量减少对象之间的交互，从⽽减⼩类之间的耦合。⼀个对象应该对其他对象有最少的了解。
  对类的低耦合提出了明确要求：

  ​	1.只和直接的朋友交流，朋友之间也是有距离的。⾃⼰的就是⾃⼰的（如果⼀个⽅法放在本类中，既不增加类间关系，也对本类不产⽣负⾯影响，那就放置在本类中）

  b.⽤例：⽼师让班⻓点名--⽼师给班⻓⼀个名单，班⻓完成点名勾选，返回结果，⽽不是班⻓点名，⽼师勾选    

- 接⼝隔离原则（Interface Segregation Principle );

​		a.客⼾端不应该依赖它不需要的接⼝，类间的依赖关系应该建⽴在最⼩的接⼝上

​		b.使⽤建议：接⼝设计尽量精简单⼀，但是不要对外暴露没有实际意义的接⼝  

​		c.⽤例：修改密码，不应该提供修改⽤⼾信息接⼝，⽽就是单⼀的最⼩修改密码接⼝，更不要暴露数据库操作  

从整体上来理解六⼤设计原则，可以简要的概括为⼀句话，⽤抽象构建框架，⽤实现扩展细节，具体
到每⼀条设计原则，则对应⼀条注意事项：  

- 单⼀职责原则告诉我们实现类要职责单⼀；  
- ⾥⽒替换原则告诉我们不要破坏继承体系；  
- 依赖倒置原则告诉我们要⾯向接⼝编程；  
- 接⼝隔离原则告诉我们在设计接⼝的时候要精简单⼀；  
- 迪⽶特法则告诉我们要降低耦合；  
- 开闭原则是总纲，告诉我们要对扩展开放，对修改关闭;

### 4.2单例模式

⼀个类只能创建⼀个对象，即单例模式，该设计模式可以保证系统中该类只有⼀个实例，并提供⼀个访问它的全局访问点，该实例被所有程序模块共享。⽐如在某个服务器程序中，该服务器的配置信息存放在⼀个⽂件中，这些配置数据由⼀个单例对象统⼀读取，然后服务进程中的其他对象再通过这个单例对象获取这些配置信息，这种⽅式简化了在复杂环境下的配置管理。
单例模式有两种实现模式：饿汉模式和懒汉模式 

- 饿汉模式:程序启动时就会创建⼀个唯⼀的实例对象。因为单例对象已经确定，所以⽐较适⽤于多
  线程环境中，多线程获取单例对象不需要加锁，可以有效的避免资源竞争，提⾼性能。  

```cpp
#include<iostream>
#include<cstdarg>

//饿汉
class Singleton
{
private:
    Singleton()
    {
        std::cout << "创建了Singleton对象" << std::endl;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
public:
static Singleton& Getinstance()
{
    return instance;
}
    ~Singleton()
    {}

private:
    int num;
    static Singleton instance;
};

Singleton Singleton::instance;

int main()
{
    return 0;
}

```

- 懒汉模式:第⼀次使⽤要使⽤单例对象的时候创建实例对象。如果单例对象构造特别耗时或者耗费济源(加载插件、加载⽹络资源等)，可以选择懒汉模式，在第⼀次使⽤的时候才创建对象。

  a.这⾥介绍的是《EffectiveC++》⼀书作者`ScottMeyers`提出的⼀种更加优雅简便的单例模式`Meyers'SingletoninC++`。  

  b.C++11Staticlocalvariables特性以确保C++11起，静态变量将能够在满⾜thread-safe的前提下唯⼀地被构造和析构。

```cpp
#include<iostream>
#include<cstdarg>

class Singleton
{
private:
    Singleton()
    {
        std::cout << "创建了Singleton对象" << std::endl;
    }
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

public:
    static Singleton &Getinstance()
    {
        static Singleton instance;
        return instance;
    }
    ~Singleton()
    {
    }

private:
    int num;
    static Singleton instance;
};

int main()
{
    Singleton::Getinstance();
    Singleton::Getinstance();
    Singleton::Getinstance();
    Singleton::Getinstance();
    return 0;
}
```

### 4.3工厂模式

⼯⼚模式是⼀种创建型设计模式，它提供了⼀种创建对象的最佳⽅式。在⼯⼚模式中，我们创建对象时不会对上层暴露创建逻辑，⽽是通过使⽤⼀个共同结构来指向新创建的对象，以此实现创建-使⽤的分离。

工厂模式可以分为：

- 简单工厂模式：

  简单⼯⼚模式实现由⼀个⼯⼚对象通过类型决定创建出来指定产品类的实例。假设有个⼯⼚能⽣产出⽔果，当客⼾需要产品的时候明确告知⼯⼚⽣产哪类⽔果，⼯⼚需要接收⽤⼾提供的类别信息，当新增产品的时候，⼯⼚内部去添加新产品的⽣产⽅式 。 

  优点：简单粗暴，直观易懂。使⽤⼀个⼯⼚⽣产同⼀等级结构下的任意产品  

  缺点：1.所有东西⽣产在⼀起，产品太多会导致代码量庞⼤  

  ​		   2.开闭原则遵循(开放拓展，关闭修改)的不是太好，要新增产品就必须修改⼯⼚⽅法  

```cpp
#include<iostream>
#include<cstdarg>
#include<string>
#include<memory>


class Fruit
{
    public:
        Fruit(){}
        virtual void show() = 0;
};

class Apple : public Fruit
{
    public:
        Apple(){}
        virtual void show()
        {
            std::cout << "我是一个苹果" <<  std::endl;
        }
};

class Banana : public Fruit
{
    public:
        Banana(){}
        virtual void show()
        {
            std::cout << "我是一个香蕉" <<  std::endl;
        }
};

class FruitFactory
{
    public:
        static std::shared_ptr<Fruit> create(const std::string& name)
        {
            if(name == "苹果")
            {
                return std::make_shared<Apple>();
            }
            else if(name == "香蕉")
            {
                return std::make_shared<Banana>();
            }
            return std::shared_ptr<Fruit>();
        }
};

int main()
{

    std::shared_ptr<Fruit> fruit = FruitFactory::create("苹果");
    fruit->show();
    fruit =  FruitFactory::create("香蕉");
    fruit->show();
    return 0;
}
```

这个模式的结构和管理产品对象的⽅式⼗分简单，但是它的扩展性⾮常差，当我们需要新增产品的时
候，就需要去修改⼯⼚类新增⼀个类型的产品创建逻辑，**违背了开闭原则**  。

### 4.4⼯⼚⽅法模式

在简单⼯⼚模式下新增多个⼯⼚，多个产品，每个产品对应⼀个⼯⼚。假设现在有A、B两种产品，则开两个⼯⼚，⼯⼚A负责⽣产产品A，⼯⼚B负责⽣产产品B，⽤⼾只知道产品的⼯⼚名，⽽不知道具体的产品信息，⼯⼚不需要再接收客⼾的产品类别，⽽只负责⽣产产品。  

```cpp
class Fruit
{
    public:
        Fruit(){}
        virtual void show() = 0;
};

class Apple : public Fruit
{
    public:
        Apple(){}
        virtual void show()
        {
            std::cout << "我是一个苹果" <<  std::endl;
        }
};

class Banana : public Fruit
{
    public:
        Banana(){}
        virtual void show()
        {
            std::cout << "我是一个香蕉" <<  std::endl;
        }
};

class FruitFactory
{
public:
    virtual std::shared_ptr<Fruit> create() = 0;
};

class AppleFactory : public FruitFactory
{
public:
    std::shared_ptr<Fruit> create() override
    {
        return std::make_shared<Apple>();
    }
};

class BananaFactory : public FruitFactory
{
public:
    std::shared_ptr<Fruit> create() override
    {
        return std::make_shared<Banana>();
    }
};

int main()
{
    std::shared_ptr<FruitFactory> factory(new AppleFactory());
    std::shared_ptr<Fruit>fruit = factory->create();
    fruit->show();
    factory.reset(new BananaFactory());
    fruit = factory->create();
    fruit->show();
    return 0;
}


```

⼯⼚⽅法模式每次增加⼀个产品时，都需要增加⼀个具体产品类和⼯⼚类，这会使得系统中类的个数
成倍增加，在⼀定程度上增加了系统的耦合度。

### 4.5抽象⼯⼚模式

⼯⼚⽅法模式通过引⼊⼯⼚等级结构，解决了简单⼯⼚模式中⼯⼚类职责太重的问题，但由于⼯⼚⽅法模式中的每个⼯⼚只⽣产⼀类产品，可能会导致系统中存在⼤量的⼯⼚类，势必会增加系统的开销。此时，我们可以考虑将⼀些相关的产品组成⼀个产品族（位于不同产品等级结构中功能相关联的产品组成的家族），由同⼀个⼯⼚来统⼀⽣产，这就是抽象⼯⼚模式的基本思想。

```cpp
#include<iostream>
#include<cstdarg>
#include<string>
#include<memory>

class Fruit
{
public:
    Fruit() {}
    virtual void name() = 0;
};

class Apple : public Fruit
{
public:
    Apple() {}
    virtual void name()
    {
        std::cout << "我是一个苹果" << std::endl;
    }
};

class Banana : public Fruit
{
public:
    Banana() {}
    virtual void name()
    {
        std::cout << "我是一个香蕉" << std::endl;
    }
};

class Animal
{
public:
    virtual void name() = 0; //纯虚函数，要求子类必须重写
};

class Lamp : public Animal
{
public:
    virtual void name()
    {
        std::cout << "我是山羊!!" << std::endl;
    }
};

class Dog : public Animal
{
public:
    virtual void name()
    {
        std::cout << "我是小狗!!" << std::endl;
    }
};

class Factory
{
public:
    virtual std::shared_ptr<Fruit> GetFruit(const std::string &name) = 0;
    virtual std::shared_ptr<Animal> GetAnimal(const std::string &name) = 0;
};

class FruitFactory : public Factory
{
public:
    virtual std::shared_ptr<Animal> GetAnimal(const std::string &name)
    {
        return std::shared_ptr<Animal>();
    }
    virtual std::shared_ptr<Fruit> GetFruit(const std::string &name)
    {
        if(name ==  "苹果")
        {
            return std::make_shared<Apple>();
        }
        else if(name == "香蕉")
        {
            return std::make_shared<Banana>();
        }

        return std::shared_ptr<Fruit>(); //相当于空指针;
    }
};

class AnimalFactory : public Factory
{
public:
    virtual std::shared_ptr<Fruit> GetFruit(const std::string &name)
    {
        return std::shared_ptr<Fruit>();
    }
    virtual std::shared_ptr<Animal> GetAnimal(const std::string &name)
    {
        if(name ==  "山羊")
        {
            return std::make_shared<Lamp>();
        }
        else if(name == "小狗")
        {
            return std::make_shared<Dog>();
        }

        return std::shared_ptr<Animal>(); //相当于空指针;
    }
};

class FactoryProducer
{
public:
    static std::shared_ptr<Factory> getFactory(const std::string &name)
    {
        if(name == "动物")
        {
            return std::make_shared<AnimalFactory>();
        }
        else 
        {
            return std::make_shared<FruitFactory>();
        }
    }
};


int main()
{
    std::shared_ptr<Factory> ff = FactoryProducer::getFactory("水果");
    std::shared_ptr<Fruit> f = ff->GetFruit("香蕉");
    f->name();
    f = ff->GetFruit("苹果");
    f->name();

    std::shared_ptr<Factory> af = FactoryProducer::getFactory("动物");
    std::shared_ptr<Animal> a = af->GetAnimal("山羊");
    a->name();
    a = af->GetAnimal("小狗");
    a->name();

    return 0;
}

//运行结果
//[mi@lavm-5wklnbmaja PreStudy]$ ./args 
//我是一个香蕉
//我是一个苹果
//我是山羊!!
//我是小狗!!
```

  抽象⼯⼚模式适⽤于⽣产多个⼯⼚系列产品衍⽣的设计模式，增加新的产品等级结构复杂，需要对原有系统进⾏较⼤的修改，甚⾄需要修改抽象层代码，**违背了“开闭原则”**；

### 4.6建造者模式

建造者模式是⼀种创建型设计模式，使⽤多个简单的对象⼀步⼀步构建成⼀个复杂的对象，能够将⼀个复杂的对象的构建与它的表⽰分离，提供⼀种创建对象的最佳⽅式。**主要⽤于解决对象的构建过于复杂的问题** 。

建造者模式主要基于四个核⼼类实现：  

- 抽象产品类：  
- 具体产品类：⼀个具体的产品对象类  
- 抽象Builder类：创建⼀个产品对象所需的各个部件的抽象接⼝  
- 具体产品的Builder类：实现抽象接⼝，构建各个部件  
- 指挥者Director类：统⼀组建过程，提供给调⽤者使⽤，通过指挥者来构造产品  

```cpp
#include<iostream>
#include<cstdarg>
#include<string>
#include<memory>

//抽象类
class Computer
{
public:
    using ptr = std::shared_ptr<Computer>; //using 关键字的用法，相当于prt等价std::shared_ptr<Computer>
    Computer() {}
    void setBoard(const std::string &board) { _board = board; }
    void setDisplay(const std::string &display) { _display = display; }
    virtual void setOS() = 0; //纯虚函数，要求子类必须重写

    std::string toString()   //生成构造好的电脑字符串
    {
        std::string computer = "Computer: \n { \n";
        computer += "\tboard = " + _board + ",\n";
        computer += "\tdisplay = " + _display + ",\n";
        computer += "\tOS = " + _os + ",\n } \n";
        return computer; 
    }

protected:
    std::string _board;
    std::string _display;
    std::string _os;
};

//具体的macbook类继承抽象父类
class MacBook : public Computer
{
public:
    using ptr = std::shared_ptr<MacBook>;
    MacBook() {}
    virtual void setOS() //重写父类的纯虚函数
    {
        _os = "Max OS X12";
    }
};

//抽象建造这类：包含创建一个产品对象的各个部件的抽象接口
class Builder
{
public:
    using ptr = std::shared_ptr<Builder>;
    virtual void buildBoard(const std::string & board) = 0; //纯虚函数
    virtual void buildDisplay(const std::string & display) = 0; //纯虚函数
    virtual void buildOS() = 0;
    virtual Computer::ptr build() = 0;
};


//具体的产品的具体建造者类：实现抽象接口，构建和组装各个组件
class MacBookBuilder : public Builder
{
public:
    using prt = std::shared_ptr<MacBookBuilder>;
    MacBookBuilder(): _computer(new MacBook()) {}; //构造一个具体的实例
    virtual void buildBoard(const std::string & board)
    {
        _computer->setBoard(board);
    }
    virtual void buildDisplay(const std::string & display)
    {
        _computer->setDisplay(display);
    }
    virtual void buildOS()
    {
         _computer->setOS(); //无参
    }

    virtual Computer::ptr build()
    {
        return _computer;
    }
private:
    Computer::ptr _computer;  //类型是std::shared_ptr<Computer>
};

//指挥者类，提供给调用者使用，通过指挥者来构造复杂产品
class Director
{
public:
    Director(Builder* builder):_builder(builder){}
    void construct(const std::string & board, const std::string & display)
    {
        _builder->buildBoard(board);
        _builder->buildDisplay(display);
        _builder->buildOS();
    }

private:
    Builder::ptr _builder;
};


int main()
{
    Builder *builder = new MacBookBuilder(); // 先构造一个具体的电脑builder类
    std::unique_ptr<Director> dp(new Director(builder)); //构造一个指挥者类
    dp->construct("华硕主板", "飞利浦显示器");
    Computer::ptr computer = builder->build(); //将builder构建出来的电脑实例用一个对象指针来接受
    std::cout << computer->toString(); //打印出对应的电脑信息

    return 0;
}
```

### 4.7代理模式  

代理模式指代理控制对其他对象的访问，也就是代理对象控制对原对象的引⽤。在某些情况下，⼀个对象不适合或者不能直接被引⽤访问，⽽代理对象可以在客⼾端和⽬标对象之间起到中介的作⽤。代理模式的结构包括⼀个是真正的你要访问的对象(⽬标类)、⼀个是代理对象。⽬标对象与代理对象实现同⼀个接⼝，先访问代理类再通过代理类访问⽬标对象。代理模式分为静态代理、动态代理： 

- 静态代理指的是，在编译时就已经确定好了代理类和被代理类的关系。也就是说，在编译时就已经确定了代理类要代理的是哪个被代理类。
- 动态代理指的是，在运⾏时才动态⽣成代理类，并将其与被代理类绑定。这意味着，在运⾏时才能
  确定代理类要代理的是哪个被代理类 。

以租房为例，房东将房⼦租出去，但是要租房⼦出去，需要发布招租启⽰，带⼈看房，负责维修，这些⼯作中有些操作并⾮房东能完成，因此房东为了图省事，将房⼦委托给中介进⾏租赁。代理模式实现。    

```cpp
//代理模式
class RentHouse
{
public:
    virtual void rentHouse() = 0; //纯虚函数子类必须实现
};

//中介
class Landlord : public RentHouse
{
public:
    void rentHouse()
    {
        std::cout << "将房子租出去\n";
    }
};

//中介代理类
class Intermediary : public RentHouse
{
public:
    void rentHouse()
    {
        std::cout << "发布招租启示\n";
        std::cout << "带人看房\n";
        _landlord.rentHouse();
        std::cout << "负责租后维修\n";
    }

private:
    Landlord _landlord;
};

int main()
{
    Intermediary intermediary;

    intermediary.rentHouse();

    return 0;
}
```







  