## 1.项目介绍

### 1.1项目功能

本项⽬主要实现⼀个⽇志系统，其主要⽀持以下功能:  

- ⽀持多级别⽇志消息 ，如Info，error，fatal，debug， warn，off等 
- ⽀持同步⽇志和异步日志，将日志消息产生后直接输出到落地目标地或者多个线程去执行
- ⽀持可靠写⼊⽇志到控制台，⽂件以及滚动⽂件中，滚动文件包括文件大小，或者时间
- ⽀持多线程程序并发写⽇志，构建cp模型多个线程并发去完成日志的落地
- ⽀持扩展不同的⽇志落地⽬标地，我们可以让日志落地到本地的文件，后期也可以输出到远端的文件

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

## 2.项目目录结构梳理

- Example

  用于存储我们的样例代码文件

- Extend

  存储我们的拓展代码文件

- Logs

  存储项目的所有的头文件

- `PreSturdy`

  项目前期需要补充学习的代码内容文件

![image-20240305180006504](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240305180006504.png)

## 3.使用方法

### 3.1克隆仓库：

- 复制以下命令到终端执行

  ```bash
  git clone git@gitee.com:slow-heating-shaanxi-people/learn.git
  ```

### 3.2构建目录：

①将我们项目的Logs文件夹放入到自己对应项目的目录下；

②引用对应的头文件：`#include"../Logs/xupt.h"` (具体路径会根据每个人放置Logs文件夹的位置而不同)

### 3.3代码示例：

```cpp
#include"../Logs/xupt.h"
#include<unistd.h>

using namespace std;

void testLog(const string& name)
{
    //获取日志器，因为这里采用了单例模式实现，所以调用对应的getInstance;
    xupt::Logger::ptr logger = xupt::LoggerManager::getInstance().getLogger(name);
    //打印info级别的日志
    logger->info("开始打印");
	
    //打印对应各个级别的日志
    logger->debug( "%s","测试日志");
    logger->info( "%s","测试日志");
    logger->warn( "%s","测试日志");
    logger->error("%s","测试日志");
    logger->fatal("%s","测试日志"); 
	
    logger->info("打印结束");
}

int main()
{
	//1.创建一个日志器建造者指针
    std::unique_ptr<xupt::LoggerBuilder> builder(new xupt::GlobalLoggerBuilder());
    //2.初始化被建造的日志器的名称
    builder->buildLoggerName("async_logger");
    //3.初始化被建造的日志器的日志输出等级
    builder->buildLoggerLevel(xupt::LogLevel::value::INFO);
    //4.初始化被建造者日志器是同步模式还是异步模式，这里对应的是异步模式
    builder->buildLoggerType(xupt::LoggerType::LOGGER_ASYNC);
    //5.初始化对应的日志输出的格式
    builder->buildFomatter("[%f:%l][%c][%p]%m%n");
    //6.文件输出模式
    builder->buildSink<xupt::FileSink>("./logfile/async.log");
    //7.标准输出模式
    builder->buildSink<xupt::StdoutSink>();
    //8.滚动文件输出模式
    builder->buildSink<xupt::RollSinkBySize>("./logfile/roll-async-by-size", 1024 * 1024);
    //9.构建日志器
    builder->build();
    //10.调用测试日志器的函数
    testLog("async_logger");

    return 0;
}
```

