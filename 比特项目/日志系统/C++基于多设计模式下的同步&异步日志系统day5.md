# C++基于多设计模式下的同步&异步日志系统day5

## 1.双缓冲区异步任务处理器（AsyncLooper）设计  

设计思想：异步处理线程+数据池  

使⽤者将需要完成的任务添加到任务池中，由异步线程来完成任务的实际执⾏操作  

任务池的设计思想：双缓冲区阻塞数据池  

优势：避免了空间的频繁申请释放，且尽可能的减少了⽣产者与消费者之间锁冲突的概率，提⾼了任务处理效率  

在任务池的设计中，有很多备选⽅案，⽐如循环队列等等，但是不管是哪⼀种都会涉及到锁冲突的情况，因为在⽣产者与消费者模型中，任何两个⻆⾊之间都具有互斥关系，因此每⼀次的任务添加与取出都有可能涉及锁的冲突，⽽双缓冲区不同，双缓冲区是处理器将⼀个缓冲区中的任务全部处理完毕后，然后交换两个缓冲区，重新对新的缓冲区中的任务进⾏处理，虽然同时多线程写⼊也会冲突，但是冲突并不会像每次只处理⼀条的时候频繁（减少了⽣产者与消费者之间的锁冲突），且不涉及到空间的频繁申请释放所带来的消耗。 

![image-20240304093702506](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240304093702506.png)

- 单个缓冲区的进一步设计
>
>设计一个缓冲区：直接存放格式化后的日志消息字符串
>
>​	好处：
>
>​		1.减少了LogMsg对象频繁的构造的消耗
>
>​		2.可以针对缓冲区中的日志消息，一次性进行IO，减少IO次数，提高效率
>
>缓冲区类的设计
>
>​	    1.管理一个存放字符串数据的缓冲区（使用vector进行空间管理）
>
>​	    2.当前的写入数据位置的指针（指向可写区域的起始位置，避免数据的写入覆盖）
>
>​		3.当前的读取数据位置的指针（指向可读数据区域的起始位置，当读取指针与写入指针指向相同位置的时候，代表没有数据了）
>
>提供的操作
>
>​		1.向缓冲区中写入数据
>
>​        2.获取可读数据起始地址的接口
>
>​        3.获取可读数据长度的接口
>
>​		4.移动读写位置的接口
>
>​		5.初始化缓冲区（将读写缓冲区的所有数据处理完毕之后）
>
>​		6.提供交换缓冲区的操作（交换空间地址，并不交换空间数据）

```cpp
/*实现异步日志器的缓冲区*/

#ifndef __M_BUF_H__
#define __M_BUF_H__


#include<iostream>
#include<vector>
#include<cassert>

namespace xupt
{

    class Buffer
    {
        #define DEFAULT_BUFFER_SZIE (1 * 1024 * 1024) //100M
        #define THREASHOLD_BUFFER_SIZE (8 * 1024 * 1024)
        #define INCREAMENT_BUFFER_SIZE (1 * 1024 * 1024)
        public:
            Buffer() : _buffer(DEFAULT_BUFFER_SZIE), _reader_idx(0), _writer_idx(0) {}
 
            //向缓冲区写入数据
            void push(const char *data, size_t len)
            {   
                //缓冲区空间不：1.扩容; 2.阻塞;
                //if(len > writeAbleSize()) return;
                //2.动态空间，测试性能极限
                ensureEnough(len);
                //1.将数据拷贝到缓冲区
                std::copy(data, data + len, &_buffer[_writer_idx]);
                //2.移动写指针
                moveWriter(len);
            }
            //返回当前可写入的大小
            size_t writeAbleSize()
            {
                //对于扩容的思路来说，我们不需要这个接口，因为总是可写的
                //因此这个接口，仅仅针对固定大小的缓冲区大小所提供的
                return(_buffer.size() - _writer_idx);
            }
            //返回可读数据的起始地址
            const char* begin()
            {
                return &_buffer[_reader_idx];
            }
            //返回可读数据的长度
            size_t readAbleSize()
            {
                //当前的缓冲区并不环形的
                //当前实现的缓冲区，是双缓冲区的思路，处理完就交换，所以不存在空间循环使用的情况
                return (_writer_idx - _reader_idx);
            }
            //移动读位置的接口
            void moveReader(size_t len)
            {
                assert(len <= readAbleSize());
                _reader_idx += len;
            }
            //重置读写位置，初始化缓冲区
            void reset()
            {
                _writer_idx = 0; //缓冲区所有的空间都是空闲的
                _reader_idx = 0; //与_writer_idx相等代表没有可读的数据
            }
            //对Buffer实现交换操作
            void swap(Buffer &buffer)
            {
                _buffer.swap(buffer._buffer);
                std::swap(_reader_idx, buffer._reader_idx);
                std::swap(_writer_idx, buffer._writer_idx);
            }
            //判断缓冲区是否为空,当任务写入缓冲区不为零的时候，我们才有交换的意义。
            bool empty()
            {
                return (_reader_idx == _writer_idx);
            }
        private:
            //对空间进行扩容
            void ensureEnough(size_t len)
            {
                if(len <= writeAbleSize()) return ;  //如果空间足够，那么直接返回不扩容
                size_t newsize = 0;
                if(_buffer.size() < THREASHOLD_BUFFER_SIZE) 
                {
                    //当buffer的空间大小小于阈值的时候，二倍增长
                    newsize = _buffer.size() * 2 + len;   //+len,防止翻倍之后依旧不满足len的情况
                }
                else
                {
                    //当buffer的空间大小大于阈值的时候，线性增长
                    newsize = _buffer.size() + INCREAMENT_BUFFER_SIZE + len;
                }
                _buffer.resize(newsize);
            }
            //移动写位置的接口
            void moveWriter(size_t len)
            {
                assert((len + _writer_idx) <= _buffer.size());
                _writer_idx += len;
            }
        private:
            std::vector<char> _buffer;  //存储缓冲区数据的vector
            size_t _reader_idx;  //读取数据的下标
            size_t _writer_idx;  //写入数据的下标
    };



}

#endif
```

## 2.日志器模块异步工作器设计与实现

①异步使用双缓冲区的思想

> 外界将任务数据，添加到输出缓冲器中
>
> 异步线程对处理缓冲区中的数据进行处理，若处理缓冲区中没有数据了，则交换缓冲区

②实现

> - 管理的成员：
>
>   a.双缓冲区（生产，消费）
>
>   b.互斥锁 ,保证线程安全
>
>   c.条件变量-生产&消费（生产缓冲区没有数据，处理完处理完消费缓冲区后就休眠）
>
>   d.回调函数（针对缓冲区中数据的处理接口，外界传入一个函数，告诉工作器如何处理数据）
>
> - 提供的操作：
>
>   a.停止异步工作器
>
>   b.添加数据到缓冲区
>
> - 私有操作：
>
>   创建线程，线程入口函数中  交换缓冲区，对消费缓区，使用回调函数进行处理
>

③异步日志器的设计

> - 继承于`Logger`日志器类
>
>   对于写日志操作进行函数重写（不再将数据写入文件，而是通过异步消息处理器，放到缓冲区中）
>
> - 通过异步消息处理器，进行日志消息的实际落地
>
> - 管理的成员
>
>   a.异步工作器(异步消息处理器)

```cpp
//logger.hpp
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
#include"looper.hpp"


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


    class AsyncLogger : public Logger
    {
    public:
        AsyncLogger(const std::string& logger_name,
            LogLevel::value level,
            Formatter::ptr &formatter,
            std::vector<LogSink::ptr> sinks,
            AsyncType looper_type = AsyncType::ASYNC_SAFE
            ):
            Logger(logger_name, level, formatter, sinks),
            _looper(std::make_shared<AsyncLooper>(std::bind(&AsyncLogger::realLog, this, std::placeholders::_1), looper_type))
            {}

        //将数据写入缓冲区
        virtual void log(const char* data, size_t len)
        {
            _looper->push(data, len);
        }

        //设计一个实际落地的函数（将缓冲区中的数据落地）
        void realLog(Buffer& buf)
        {
            //这里不需要加锁，因为这里不是多线程，是串行进行的
            if(_sinks.empty()) return;
            for(auto &sink : _sinks)
            {
                sink->log(buf.begin(), buf.readAbleSize());
            }
        }
    private:
        AsyncLooper::ptr _looper; //
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
                _limit_level(LogLevel::value::DEBUG),
                _looper_type(AsyncType::ASYNC_SAFE)
            {}
            void buildLoggerType(LoggerType type) { _logger_type = type; }
            void buildEnableUnsafeAsync() { _looper_type = AsyncType::ASYNC_UNSAFE; }
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
            AsyncType _looper_type;
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
                    return std::make_shared<AsyncLogger>(_logger_name, _limit_level, _formatter, _sinks);
                }

                //返回同步日志器
                return std::make_shared<SyncLogger>(_logger_name, _limit_level, _formatter, _sinks);
            }
    };  

}


#endif


//looper.hpp
/*实现异步工作器*/


#ifndef __M_LOP_H__
#define __M_LOP_H__


#include"buffer.hpp"

#include<mutex>
#include<condition_variable>
#include<thread>
#include<functional>
#include<atomic>

namespace xupt
{
    using Functor = std::function<void(Buffer&)>;

    enum class AsyncType
    {
        ASYNC_SAFE,  // 安全模式，表示缓冲区满了则退出
        ASYNC_UNSAFE // 无限扩容
    };

    class AsyncLooper
    {
    public:
        using ptr = std::shared_ptr<AsyncLooper>;
        AsyncLooper(const Functor & cb, AsyncType looper_type = AsyncType::ASYNC_SAFE):
        _looper_type(looper_type),
        _stop(false),
        _thread(std::thread(&AsyncLooper::threadEntry, this)),
        _callback(cb){}
        ~AsyncLooper() { stop(); }
        void stop()
        {
            _stop = true;
            _cond_con.notify_all(); //唤醒所有的消费线程
            _thread.join();//等待工作线程的退出
        }
        void push(const char *data, size_t len) //
        {
            //1.无限扩容 -- 不安全  2.固定大小 -- 缓冲区满的时候阻塞
            std::unique_lock<std::mutex> lock(_mutex);

            if(_looper_type == AsyncType::ASYNC_SAFE)
            //条件变量控制，如果当前缓冲区的大小，足够放入数据，则可以添加数据
            _cond_pro.wait(lock, [&](){ return _pro_buf.writeAbleSize() >= len; });
            //将数据写入到缓冲区
            _pro_buf.push(data, len);
            //唤醒消费者对缓冲区中的数据进行处理
            _cond_con.notify_one();
        } 
    private:
        // 线程入口函数
        void threadEntry()
        {
            while(true)
            {
                // 为互斥锁设置一个声明周期，当缓冲区交换完，就解锁（并不对数据的处理过程加锁）
                {
                    //退出标志已设置，并且缓冲区中没有数据，则退出
                    if(_stop && _pro_buf.empty()) break;
                    
                    // 1.判断缓冲区是否有数据，有数据则交换，无则阻塞
                    std::unique_lock<std::mutex> lock(_mutex);
                    _cond_con.wait(lock, [&]() { return _stop || !_pro_buf.empty(); });
                    _con_buf.swap(_pro_buf);

                    if(_looper_type == AsyncType::ASYNC_SAFE)
                    // 唤醒生产者
                    _cond_pro.notify_all(); 
                }
                // 2.被唤醒后，对消费缓冲区进行处理
                _callback(_con_buf);
                // 3.初始化消费者缓冲区
                _con_buf.reset();
            }
        }
    private:
        Functor _callback; //具体对缓冲区数据处理的函数，由异步工作器的使用这传入  
    private:
        AsyncType _looper_type; 
        std::atomic<bool> _stop;                        // 是否停止
        Buffer _pro_buf;                   // 生产缓冲区
        Buffer _con_buf;                   // 消费缓冲区
        std::mutex _mutex;                 // 互斥锁
        std::condition_variable _cond_pro; // 生产条件变量
        std::condition_variable _cond_con; // 消费条件变量
        std::thread _thread;               // 异步工作器对应的工作线程
    };

}

```

