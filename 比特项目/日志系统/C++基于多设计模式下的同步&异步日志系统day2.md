# C++基于多设计模式下的同步&异步日志系统day2

## 1.日志系统框架设计

本项⽬实现的是⼀个多⽇志器⽇志系统，主要实现的功能是让程序员能够轻松的将程序运⾏⽇志信息落地到指定的位置，且⽀持同步与异步两种⽅式的⽇志落地⽅式。项⽬的框架设计将项⽬分为以下⼏个模块来实现。

### 1.1模块划分

- **⽇志等级模块**：对输出⽇志的等级进⾏划分，以便于控制⽇志的输出，并提供等级枚举转字符串功能。

  1. OFF:关闭
  2. DEBUG:调试，调试时的关键信息输出
  3. INFO:提示，普通的提示性日志信息
  4. WARN:警告，不影响运行，但是需要注意一下的日志
  5. ERROR:错误，程序运行出现错误的日志
  6. FATAL:致命，一般是代码异常导致程序无法继续推进的运行的日志

- **日志消息模块**：中间存储日志输出所需的个各项要素信息

  1. 时间：描述本条日志的输出时间
  2. 线程ID：描述本条日志是哪个线程输出的
  3. 日志等级：描述本条日志的等级
  4. 日志数据：本条日志的有效载荷数据
  5. 日志文件名：描述本条日志在哪个源码文件中输出的
  6. 日志行号：描述本条日志在源码文件的哪一行输出的

- **⽇志消息格式化模块**：设置⽇志输出格式，并提供对⽇志消息进⾏格式化功能  

  1. 系统的默认⽇志输出格式：%d{%H:%M:%S}%T[%t]%T[%p]%T[%c]%T%f:%l%T%m%n  
  2. ->13:26:32  [2343223321]  [FATAL]  [root]  main.c:76套接字创建失败\n  
  3. %d{%H:%M:%S}：表⽰⽇期时间，花括号中的内容表⽰⽇期时间的格式  
  4. %T：表⽰制表符缩进  
  5. %t：表⽰线程ID  
  6. %p：表⽰⽇志级别  
  7. %c：表⽰⽇志器名称，不同的开发组可以创建⾃⼰的⽇志器进⾏⽇志输出，⼩组之间互不影响
  8. %f：表⽰⽇志输出时的源代码⽂件名  
  9. %l：表⽰⽇志输出时的源代码⾏号  
  10. %m：表⽰给与的⽇志有效载荷数据  
  11. %n：表⽰换⾏  
  12. 设计思想：设计不同的⼦类，不同的⼦类从⽇志消息中取出不同的数据进⾏处理  

- **⽇志消息落地模块**：决定了⽇志的落地⽅向，可以是标准输出，也可以是⽇志⽂件，也可以滚动⽂
  件输出....  

  1. 标准输出：表⽰将⽇志进⾏标准输出的打印
  2. ⽇志⽂件输出：表⽰将⽇志写⼊指定的⽂件末尾  
  3. 滚动⽂件输出：当前以⽂件⼤⼩进⾏控制，当⼀个⽇志⽂件⼤⼩达到指定⼤⼩，则切换下⼀个⽂件进⾏输出  
  4. 后期，也可以扩展远程⽇志输出，创建客⼾端，将⽇志消息发送给远程的⽇志分析服务器  
  5. 设计思想：设计不同的⼦类，不同的⼦类控制不同的⽇志落地⽅向  

- **⽇志器模块：**

  1. 此模块是对以上⼏个模块的整合模块，⽤⼾通过⽇志器进⾏⽇志的输出，有效降低⽤⼾的使⽤难度 
  2. 包含有：⽇志消息落地模块对象，⽇志消息格式化模块对象，⽇志输出等级   

- **⽇志器管理模块：**  

  1. 为了降低项⽬开发的⽇志耦合，不同的项⽬组可以有⾃⼰的⽇志器来控制输出格式以及落地⽅向，因此本项⽬是⼀个多⽇志器的⽇志系统  
  2. 管理模块就是对创建的所有⽇志器进⾏统⼀管理。并提供⼀个默认⽇志器提供标准输出的⽇志输出  

- **异步线程模块：**  

  1. 实现对⽇志的异步输出功能，⽤⼾只需要将输出⽇志任务放⼊任务池，异步线程负责⽇志的落地输出功能，以此提供更加⾼效的⾮阻塞⽇志输出  

### 1.2模块关系图

![image-20240301151659398](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240301151659398.png)

##  2.代码设计

### 2.1实用类设计

提前完成⼀些零碎的功能接⼝，以便于项⽬中会⽤到  

- 获取系统时间
-  判断⽂件是否存在
-  获取⽂件的所在⽬录路径
-  创建⽬录  

```cpp
/*实用工具类的实现：
    1.获取系统时间
    2.判断文件是否存在
    3.获取文件所在路径
    4.创建目录
*/

#include<iostream>
#include<ctime>
#include<unistd.h>
#include<sys/stat.h>

namespace xupt
{
    namespace util
    {
        class Date
        {
            public:
                static size_t GetTime()
                {
                    return (size_t)time(nullptr);
                }    
        };
        class File
        {
            public:
                static bool exist(const std::string &pathname)
                {
                    //确保平台移植性，我们不是用这个接口
                    //return (access(pathname.c_str(), F_OK) == 0); //F_OK,文件存在且权限允许，返回0，
                    struct st;
                    if(stat(pathname.c_str(), &st) < 0)
                    {
                        return false;
                    }
                    return true;
                }
                static std::string path(const std::string &pathname)
                {
                    size_t pos = pathname.find_last_of("/\\"); /*从路径字符串的后面开始寻找第一个/或者\ */
                    if(pos == std::string.npos) return "."; //如果没有找到，那么证明这个文件就在当前的目录
                    return pathname.substr(0, pos + 1); //输出包含最后一个/的位置的字符串
                }
                static void CreateDirectory(const std::string &pathname)
                {
                    size_t pos = 0, idx = 0;
                    while(idx < pathname.size())
                    {
                        pos = pathname.find_first_of("/\\"); //寻找第一个/或者\
                        
                        if(pos == std::string.npos)
                        {
                            mkdir(pathname, 0777); //如果没有分隔符，那么就直接创建，并且设置权限为0777
                        }
                        std::string parent_dir = pathname.substr(0, idx + pos + 1); //截取包含从开始到/的父目录
                        if(exist(parent_dir) == true) { idx = pos + 1; continue; } //如果当前的目录存在了，那么就跳过去找下一个
                        mkdir(parent_dir, 0777);
                        idx = pos + 1;
                    }
                }
        };
    }
}
```

### 2.2日志等级类设计

①日志等级总共分为7个等级，分别为：

- UNKNOW表示未知错误
- DRBUG进⾏debug时候打印⽇志的等级  
- INFO打印⼀些⽤⼾提⽰信息  
- WARN打印警告信息  
- ERROR打印错误信息  
- FATAL打印致命信息-导致程序崩溃的信息  
- OFF关闭所有日志输出

每一个项目中都会设置一个默认的日志输出等级，只有输出的日志等级大于等于默认限制等级的时候才可以进行输出

②提供一个接口，将对应等级的枚举，转换为一个对应的字符串。

```cpp
/*
    1.定义枚举类，枚举出日志等级
    2.提供转换接口:将枚举转换为对应字符串
*/
#ifndef __M_LEVEL_H__
#define __M_LEVEL_H__


namespace xupt
{
    class LogLevel
    {
        public:
            enum class value
            {
                UNKOWN = 0,
                DEBUG,
                INFO,
                WARN,
                ERROR,
                FATAL,
                OFF
            };
            static const char* toString(LogLevel::value level)
            {
                switch (level)
                {
                case LogLevel::value::DEBUG: return "DEBUG";
                case LogLevel::value::INFO: return "INFO";
                case LogLevel::value::WARN: return "WARN";
                case LogLevel::value::ERROR: return "ERROR";
                case LogLevel::value::FATAL: return "FATAL";
                case LogLevel::value::OFF: return "OFF";
                }
                return "UNKOWN";
            }
    };
}

#endif

```

### 2.3日志消息类的设计

⽇志消息类主要是封装⼀条完整的⽇志消息所需的内容，其中包括**⽇志等级**、对应的**loggername**、**打**
**印⽇志源⽂件的位置信息**（包括**⽂件名和⾏号**）、**线程ID**、**时间戳信息**、**具体的⽇志信息**等内容  

```cpp
定义日志消息类，进行日志中间信息的存储:
    1.日志的输出时间   用于过滤日志输出时间
    2.日志等级        用于进行日志过滤分析
    3.源文件名称 
    4.源代码行号      用于定位出现错误的代码位置
    5.线程ID          用于过滤出错的线程
    6.日志主体消息
    7.日志器名称  (当前支持多日志器的同时使用)
*/
#ifndef __M_MESSAGE_H__
#define __M_MESSAGE_H__


#include"util.hpp"
#include"level.hpp"
#include<iostream>
#include<string>
#include<thread>

namespace xupt
{
    struct LogMsg
    {
        size_t _ctime; //日志产生的时间
        LogLevel::value _level; //日志等级
        size_t _line;  //行号
        std::thread::id _tid; //线程id
        std::string _file; //文件名
        std::string _logger; //日志器名称
        std::string _payload; //有效消息处理


            //对应的构造函数
    LogMsg(LogLevel::value level,
        size_t line,
        std::string file,
        std::string logger,
        std::string msg):
        _ctime(util::Date::now()),
        _level(level),
        _line(line),
        _tid(std::this_thread::get_id()),
        _file(file),
        _logger(logger),
        _payload(msg)
        {}
    };

}

#endif
```

