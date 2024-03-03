# C++基于多设计模式下的同步&异步日志系统day4

## 1.⽇志器类(Logger)设计（建造者模式）  

⽇志器主要是⽤来和前端交互，当我们需要使⽤⽇志系统打印log的时候，只需要创建Logger对象，调⽤该对象debug、info、warn、error、fatal等⽅法输出⾃⼰想打印的⽇志即可，⽀持解析可变参数列表和输出格式，即可以做到像使⽤printf函数⼀样打印⽇志。
当前⽇志系统⽀持同步⽇志&异步⽇志两种模式，两个不同的⽇志器唯⼀不同的地⽅在于他们在⽇志的落地⽅式上有所不同：

- 同步⽇志器：直接对⽇志消息进⾏输出。
- 异步⽇志器：将⽇志消息放⼊缓冲区，由异步线程进⾏输出。

因此⽇志器类在设计的时候先设计出⼀个Logger基类，在Logger基类的基础上，继承出`SyncLogger`同步⽇志器和`AsyncLogger`异步⽇志器。且因为⽇志器模块是对前边多个模块的整合，想要创建⼀个⽇志器，需要设置⽇志器名称，设置⽇志输出等级，设置⽇志器类型，设置⽇志输出格式，设置落地⽅向，且落地⽅向有可能存在多个，整个⽇志器的创建过程较为复杂，为了保持良好的代码⻛格，编写出优雅的代码，因此⽇志器的创建这⾥采⽤了建造者模式来进⾏创建。  

```cpp
/*
    完成日志器模块：
        1.抽象日志器基类
        2.派生出不同的子类（同步日志器类 & 异步日志器类）

*/
#ifndef __M_LOGER_H__
#define __M_LOGER_H__

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include"format.hpp"
#include"level.hpp"
#include"message.hpp"
#include"sink.hpp"
#include"util.hpp"


#include<atomic>
#include<stdio.h>
#include<mutex>
#include<stdarg.h>
#include<cassert>


namespace xupt
{
    class Logger
    {
    public:
        using ptr = std::shared_ptr<Logger>;
        Logger(const std::string& logger_name,
            LogLevel::value level,
            Formatter::ptr &formatter,
            std::vector<LogSink::ptr> sinks):
            _logger_name(logger_name),
            _limit_level(level),
            _formatter(formatter),
            _sinks(sinks.begin(), sinks.end()){}
        /*完成日志构造消息，并进行格式化，得到格式化后的日志消息字符串---然后进行落地输出*/
        void debug(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            //1.判断当前的日志是否到达了输出等级
            if(LogLevel::value::DEBUG < _limit_level) { return ; }
            va_list ap;
            va_start(ap, fmt);
            char* res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if(ret == -1)
            {
                std::cout << "vasprintf failed!\n";
                return ;
            }
            va_end(ap); //将ap置空
            serialize(LogLevel::value::DEBUG, file, line, res);
            free(res);
        }
        void info(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            //1.判断当前的日志是否到达了输出等级
            if(LogLevel::value::INFO < _limit_level) { return ; }
            va_list ap;
            va_start(ap, fmt);
            char* res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if(ret == -1)
            {
                std::cout << "vasprintf failed!\n";
                return ;
            }
            va_end(ap); //将ap置空
            serialize(LogLevel::value::INFO, file, line, res);
            free(res);
        }        
        void warn(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            //1.判断当前的日志是否到达了输出等级
            if(LogLevel::value::WARN < _limit_level) { return ; }
            va_list ap;
            va_start(ap, fmt);
            char* res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if(ret == -1)
            {
                std::cout << "vasprintf failed!\n";
                return ;
            }
            va_end(ap); //将ap置空
            serialize(LogLevel::value::WARN, file, line, res);
            free(res);
        }             
        void error(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            //1.判断当前的日志是否到达了输出等级
            if(LogLevel::value::ERROR < _limit_level) { return ; }
            va_list ap;
            va_start(ap, fmt);
            char* res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if(ret == -1)
            {
                std::cout << "vasprintf failed!\n";
                return ;
            }
            va_end(ap); //将ap置空
            serialize(LogLevel::value::ERROR, file, line, res);
            free(res);
        }             
        void fatal(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            //1.判断当前的日志是否到达了输出等级
            if(LogLevel::value::FATAL < _limit_level) { return ; }
            va_list ap;
            va_start(ap, fmt);
            char* res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if(ret == -1)
            {
                std::cout << "vasprintf failed!\n";
                return ;
            }
            va_end(ap); //将ap置空
            serialize(LogLevel::value::FATAL, file, line, res);
            free(res);
        }     

    protected:
        void serialize(LogLevel::value level, const std::string &file, size_t line, char* str)
        {
            //2.构造LogMsg对象
            LogMsg msg(level, line, file, _logger_name, str);
            //3.通过格式化工具对LogMsg进行格式化,得到格式化后的日志字符串
            std::stringstream ss;
            _formatter->format(ss, msg);
            //5.进行日志落地
            log(ss.str().c_str(), ss.str().size());
        }
        /*抽象接口完成实际的落地输出--不同的日志器会有不同的实际落地方式*/
        virtual void log(const char *data, size_t len) = 0;

    protected:
        std::mutex _mutex;                         // 保证过程的线程安全
        std::string _logger_name;                  // 日志器名称
        std::atomic<LogLevel::value> _limit_level; // 日志等级
        Formatter::ptr _formatter;                 // 格式化
        std::vector<LogSink::ptr> _sinks;          // 用一个数组来存放日志落地位置
    };



    //同步日志器
    class SyncLogger : public Logger
    {
    public:
        SyncLogger(const std::string& logger_name,
            LogLevel::value level,
            Formatter::ptr &formatter,
            std::vector<LogSink::ptr> sinks):
            Logger(logger_name, level, formatter, sinks){}
    protected:
        virtual void log(const char *data, size_t len)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if(_sinks.empty()) return;
            for(auto &sink : _sinks)
            {
                sink->log(data,len);
            }
        }
    };



    enum class LoggerType
    {
        LOGGER_SYNC,
        LOGGER_ASYNC
    };
    /*使用建造者模式来建造日志器，而不用用户直接去构造日志器，减少用户的使用复杂度*/
    //1.抽象一个日志器建造者类（完成日志器对象所需零部件的构造 & 日志器的构建）
    //  1.设置日志器类型
    //  2.将不同类型日志器的创建放到同一个日志器建造类中完成
    class LoggerBuilder
    {
        public:
            LoggerBuilder():
                _logger_type(LoggerType::LOGGER_SYNC),
                _limit_level(LogLevel::value::DEBUG)
            {}
            void buildLoggerType(LoggerType type) { _logger_type = type; }
            void buildLoggerName(const std::string & name) { _logger_name = name; }
            void buildLoggerLevel(LogLevel::value level) { _limit_level = level; }
            void buildFomatter(const std::string &pattern) 
            { 
                _formatter = std::make_shared<Formatter>(pattern); 
            }
            template<typename SinkType, typename ...Args>
            void buildSink(Args &&... args)
            {
                LogSink::ptr psink = SinkFactory::create<SinkType>(std::forward<Args>(args)...);
                _sinks.push_back(psink);
            }
            virtual Logger::ptr build() = 0;
        protected:
            LoggerType _logger_type;
            std::string _logger_name;                  // 日志器名称
            std::atomic<LogLevel::value> _limit_level; // 日志等级
            Formatter::ptr _formatter;                 // 格式化
            std::vector<LogSink::ptr> _sinks;          // 用一个数组来存放日志落地位置
    };
    //2.派生出具体的建造者类---局部的日志器建造者&全局的日志器建造者（后边添加了全局单例管理器之后，将日志器添加全局管理）

    class LocalLoggerBuilder : public LoggerBuilder
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
                if(_logger_type == LoggerType::LOGGER_ASYNC)
                {
                    //返回异步日志器...
                }

                //返回同步日志器
                return std::make_shared<SyncLogger>(_logger_name, _limit_level, _formatter, _sinks);
            }
    };  

}


#endif
```

