# C++基于多设计模式下的同步&异步日志系统day6

## 1.单例日志器管理类设计

⽇志的输出，我们希望能够在任意位置都可以进⾏，但是当我们创建了⼀个⽇志器之后，就会受到⽇志器所在作⽤域的访问属性限制。

因此，为了突破访问区域的限制，我们创建⼀个⽇志器管理类，且这个类是⼀个单例类，这样的话，我们就可以在任意位置来通过管理器单例获取到指定的⽇志器来进⾏⽇志输出了。

基于单例⽇志器管理器的设计思想，我们对于⽇志器建造者类进⾏继承，继承出⼀个全局⽇志器建造者类，实现⼀个⽇志器在创建完毕后，直接将其添加到单例的⽇志器管理器中，以便于能够在任何位置通过⽇志器名称能够获取到指定的⽇志器进⾏⽇志输出。

**作用和特性：**

​	作用①：对所有创建的日志器进行管理

​	特性：将管理器设计为单例

​	作用②：可以在程序的任意位置，获取相同的单例对象，获取其中的日志器进行日志输出

拓展：单例管理器创建的时候，默认先创建一个日志器（用于进行标准输出的打印）

目的：让用户在不创建任何日志器的情况下，也能进行标准输出的打印，方便用户使用

**管理的成员**

​	①默认的日志器

​	②管理日志器的数组

​	③互斥锁

**提供的接口**

​	①添加日志器管理

​	②判断是否管理了指定名称的日志器

​	③获取指定名称的日志器

​	④获取默认日志器

### 1.1日志器管理类的实现

```cpp
   class LoggerManager
    {
    public:
        //单例对象的获取
        static LoggerManager& getInstance()
        {
            //c++11之后，针对静态局部变量，在编译层面已经实现了线程安全
            //当静态局部变量没有完成构造之前，这时候其他的线程进入就会阻塞
            static LoggerManager eton;
            return eton;
        }
        //添加日志器
        void addLogger(Logger::ptr &logger)
        {
            //如果存在就不再添加
            if(hasLogger(logger->name())) return ;
            std::unique_lock<std::mutex> lock;
            _loggers.insert(std::make_pair(logger->name(), logger));
        }
        //查看指定名称的日志器是否存在
        bool hasLogger(const std::string & name)
        {
            std::unique_lock<std::mutex> lock;
            auto it = _loggers.find(name);
            if(it == _loggers.end())
            {
                return  false;
            }
            return true;
        }
        //返回指定名称的日志器
        Logger::ptr getLogger(const std::string & name)
        {
            std::unique_lock<std::mutex> lock;
            auto it = _loggers.find(name);
            if(it == _loggers.end())
            {
                return Logger::ptr();
            }
            return it->second;            
        }
        // 获取默认的日志器
        Logger::ptr rootLogger()
        {
            return _root_logger;
        }

    private:
        LoggerManager()
        {
            std::unique_ptr<xupt::LoggerBuilder> builder(new xupt::LocalLoggerBuilder());
            builder->buildLoggerName("root");
            _root_logger = builder->build();
            _loggers.insert(std::make_pair("root", _root_logger));
        }
    private:
        //互斥锁，防止我们在程序的任意位置获取日志器的时候造成线程安全问题
        std::mutex _mutex;
        //默认的日志器指针
        Logger::ptr _root_logger;
        //管理日志器的数组
        std::unordered_map<std::string, Logger::ptr> _loggers;
    };

```

### 1.2全局建造者的实现

```cpp
    /*设计一个全局日志器建造者---在局部的基础上增加了一个功能：将日志器添加到单例对象中*/
    class GlobalLoggerBuilder : public LoggerBuilder
    {
        public:
            Logger::ptr build() override
            {
                assert(_logger_name.empty() == false); //必须有日志器名称
                if(_formatter.get() == nullptr)
                {
                    _formatter = std::make_shared<Formatter>();
                }
                if(_sinks.empty())
                {
                    buildSink<StdoutSink>();
                }

                Logger::ptr logger;
                if(_logger_type == LoggerType::LOGGER_ASYNC)
                {
                    logger = std::make_shared<AsyncLogger>(_logger_name, _limit_level, _formatter, _sinks);
                }
                else 
                {
                    logger = std::make_shared<SyncLogger>(_logger_name, _limit_level, _formatter, _sinks);
                }

                LoggerManager::getInstance().addLogger(logger);
                //返回同步日志器
                return logger;
            }
    };  

```

## 2.全局接口的设计与实现

### 2.1全局接口的设计

> 提供全局接口&宏函数，对日志系统接口进行使用便捷性优化
>
> ①提供获取指定日志器的全局接口（避免用户自己操作单例对象）
>
> ②使用宏函数对日志器的接口进行代理（代理模式）
>
> ③提供宏函数直接实现日志的标准输出打印（不用获取日志器了）

提供全局的⽇志器获取接⼝。
使⽤代理模式通过全局函数或宏函数来代理Logger类的log、debug、info、warn、error、fatal等接⼝，以便于控制源码⽂件名称和⾏号的输出控制，简化⽤⼾操作。当仅需标准输出⽇志的时候可以通过主⽇志器来打印⽇志。且操作时只需要通过宏函数直接进⾏输出即可。 

```cpp
#ifndef __M_XUPT_H__
#define __M_XUPT_H__


// ①提供获取指定日志器的全局接口（避免用户自己操作单例对象）

// ②使用宏函数对日志器的接口进行代理（代理模式）

// ③提供宏函数直接实现日志的标准输出打印（不用获取日志器了）

#include"logger.hpp"

namespace xupt
{

    // ①提供获取指定日志器的全局接口（避免用户自己操作单例对象）
    Logger::ptr getLogger(const std:: string& name)
    {
        return xupt::LoggerManager::getInstance().getLogger(name);
    }
    Logger::ptr rootLogger()
    {
        return xupt::LoggerManager::getInstance().rootLogger();
    }

    // ②使用宏函数对日志器的接口进行代理（代理模式）
    #define debug(fmt,...) debug(__FILE__, __LINE__, fmt, ##__VA_ARGS__);
    #define info(fmt,...) info(__FILE__, __LINE__, fmt, ##__VA_ARGS__);
    #define warn(fmt,...) warn(__FILE__, __LINE__, fmt, ##__VA_ARGS__);
    #define error(fmt,...) error(__FILE__, __LINE__, fmt, ##__VA_ARGS__);
    #define fatal(fmt,...) fatal(__FILE__, __LINE__, fmt, ##__VA_ARGS__);

    // ③提供宏函数直接实现日志的标准输出打印（不用获取日志器了）
    #define DEBUG(fmt, ...) rootLogger()->debug(fmt, #__VA_ARGS__)
    #define INFO(fmt, ...) rootLogger()->info(fmt, #__VA_ARGS__)
    #define WARN(fmt, ...) rootLogger()->warn(fmt, #__VA_ARGS__)
    #define ERROR(fmt, ...) rootLogger()->error(fmt, #__VA_ARGS__)
    #define FATAL(fmt, ...) rootLogger()->fatal(fmt, #__VA_ARGS__)


}


```

