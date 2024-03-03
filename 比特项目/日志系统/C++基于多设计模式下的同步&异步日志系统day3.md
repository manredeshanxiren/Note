# C++基于多设计模式下的同步&异步日志系统day3

# 1.代码设计

### 1.1日志输出格式化类设计

⽇志格式化（Formatter）类主要负责格式化⽇志消息。其主要包含以下内容  

- pattern成员：保存⽇志输出的格式字符串。
  ◦ %d⽇期
  ◦ %T缩进
  ◦ %t线程id
  ◦ %p⽇志级别
  ◦ %c⽇志器名称
  ◦ %f⽂件名
  ◦ %l⾏号
  ◦ %m⽇志消息
  ◦ %n换⾏  

- std::vector<FormatItem::ptr>items成员：⽤于按序保存格式化字符串对应的⼦格式化对象  

`FormatItem`类主要负责⽇志消息⼦项的获取及格式化。其包含以下⼦类 

- `MsgFormatItem：`表⽰要从`LogMsg`中取出有效⽇志数据  
- `LevelFormatItem：`表⽰要从`LogMsg`中取出⽇志等级  
- `NameFormatItem：`表⽰要从`LogMsg`中取出⽇志器名称
- `ThreadFormatItem：`表⽰要从`LogMsg`中取出线程ID
- `TimeFormatItem：`表⽰要从`LogMsg`中取出时间戳并按照指定格式进⾏格式化
- ` CFileFormatItem：`表⽰要从`LogMsg`中取出源码所在⽂件名
- `CLineFormatItem：`表⽰要从`LogMsg`中取出源码所在⾏号
- `TabFormatItem：`表⽰⼀个制表符缩进
- `NLineFormatItem：`表⽰⼀个换⾏
- `OtherFormatItem：`表⽰⾮格式化的原始字符串  

⽰例："[%d{%H:%M:%S}]%m%n"  

```cpp
pattern = "[%d{%H:%M:%S}] %m%n"
items = {
{OtherFormatItem(), "["},
{TimeFormatItem(), "%H:%M:%S"},
{OtherFormatItem(), "]"},
{MsgFormatItem (), ""},
{NLineFormatItem (), ""}
}
LogMsg msg = {
size_t _line = 22;
size_t _ctime = 12345678;
std::thread::id _tid = 0x12345678;
std::string _name = "logger";
std::string _file = "main.cpp";
std::string _payload = "创建套接字失败";
LogLevel::value _level = ERROR;
};
```

格式化的过程其实就是按次序从Msg中取出需要的数据进⾏字符串的连接的过程。
最终组织出来的格式化消息："[22:32:54]创建套接字失败\n"  

代码实现：

```cpp
#ifndef __M_FMT_H__
#define __M_FMT_H__


#include"message.hpp"
#include<time.h>
#include<vector>
#include<cassert>
#include<sstream>


namespace xupt
{
    //抽象格式化子类基类
    class FormatItem
    {
        public:
            using ptr = std::shared_ptr<FormatItem>;
            virtual void format(std::ostream& out, const LogMsg &msg) = 0; 
    };

    //派生格式化子类项---消息，等级，时间，文件名，行号，线程ID，日志器名，制表符，换行，其他
    class MsgFormatItem : public FormatItem
    {
        public:
            virtual void format(std:: ostream& out, const LogMsg &msg)
            {
                out << msg._payload;
            }
    };

    class LevelFormatItem : public FormatItem
    {
        public:
            virtual void format(std:: ostream& out, const LogMsg &msg)
            {
                out << LogLevel::toString(msg._level);
            }
    };

    class TimeFormatItem : public FormatItem
    {
        public:
            TimeFormatItem(const std::string &fmt = "%H:%M:%S"):_time_fmt(fmt){}
            virtual void format(std:: ostream& out, const LogMsg &msg)
            {
                struct tm t;
                localtime_r(&msg._ctime, &t);
                char tmp[128];
                strftime(tmp, 127, _time_fmt.c_str(), &t);
                out << tmp;
            }
        private:
            std::string _time_fmt;
    };    

    class FileFormatItem : public FormatItem
    {
        public:
            virtual void format(std:: ostream& out, const LogMsg &msg)
            {
                out << msg._file;
            }
    };   

    class LineFormatItem : public FormatItem
    {
        public:
            virtual void format(std:: ostream& out, const LogMsg &msg)
            {
                out << msg._line;
            }
    };   

    class ThreadFormatItem : public FormatItem
    {
        public:
            virtual void format(std:: ostream& out, const LogMsg &msg)
            {
                out << msg._tid;
            }
    };   

    class LoggerFormatItem : public FormatItem
    {
        public:
            virtual void format(std:: ostream& out, const LogMsg &msg)
            {
                out << msg._logger;
            }
    };  

    class TabFormatItem : public FormatItem
    {
        public:
            virtual void format(std:: ostream& out, const LogMsg &msg)
            {
                out << "\t";
            }
    };    

    class NewLineFormatItem : public FormatItem
    {
        public:
            virtual void format(std:: ostream& out, const LogMsg &msg)
            {
                out << "\n";
            }
    };   

    class OtherFormatItem : public FormatItem
    {
        public:
            OtherFormatItem(const std::string str):_str(str){}
            virtual void format(std:: ostream& out, const LogMsg &msg)
            {
                out << _str;
            }
        private:
            std::string _str;
    };   



    class Formatter
    {
        public:
            Formatter(const std::string & pattern = "[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n")
            :_pattern(pattern)
            {
                assert(parsePattern()); //如果解析失败直接跳断言
            }

            //对msg进行格式化
            void format(std::ostream& out ,const  LogMsg & Msg)
            {
                for(auto& item : _items)
                {
                    item->format(out, Msg);
                }
            }
            std::string format(const LogMsg & Msg)
            {
                std::stringstream ss;
                format(ss, Msg);
                return ss.str();
            }
        private:
            //对格式化规则字符串进行解析
            bool parsePattern()
            {
                //1.对格式化规则字符串进行解析
                //abcd[%d{%H:%M:%S}][%p]%T%m%n
                std::vector<std::pair<std::string, std::string>> fmt_order;
                size_t pos = 0;

                std::string key, val;                
                while(pos < _pattern.size())
                {
                    //1.处理原始字符串，如果不是%，那么就是原始字符
                    if(_pattern[pos] != '%')
                    {
                        val.push_back(_pattern[pos++]);
                        continue;
                    }
                    //%的情况
                    //1.双%的情况
                    if(pos + 1 < _pattern.size() && _pattern[pos + 1] == '%')
                    {
                        val.push_back('%'); pos += 2; continue;
                    }

                    //能走到这里，代表%后是一个格式化字符串，这时候代表原始字符串处理完毕
                    if(val.empty() == false)
                    {
                        fmt_order.push_back(std::make_pair("", val));
                        val.clear();
                    }

                    //2.%后是格式化字符的情况
                    pos += 1;//这一步之后，pos指向格式化字符位置
                    if(pos == _pattern.size())
                    {
                        std::cout << "%后没有对应的格式化字符" << std::endl;
                        return false;
                    }

                    key = _pattern[pos];
                    //2.{}的情况
                    pos += 1;
                    if(_pattern[pos] == '{')
                    {
                        pos += 1; //这时候pos指向子规则的起始位置
                        while(pos < _pattern.size() && _pattern[pos] != '}')
                        {
                            val.push_back(_pattern[pos++]);
                        }

                        //走到了末尾跳出了循环，则代表没有遇到}，代表格式是错误的
                        if(pos == _pattern.size())  //没有找到}
                        {
                            std::cout << "子规则{}匹配失败" << std::endl;
                            return false;
                        }
                        pos += 1;
                    }
                    fmt_order.push_back(std::make_pair(key, val));
                    key.clear();
                    val.clear();
                }
                //2.根据解析得到的数据初始化子项数组成员
                for(auto &it : fmt_order)
                {
                    _items.push_back(createItem(it.first,it.second));
                }

                return true;
            }
        
            //根据不同的格式化字符创建不同的格式化子项对象
            FormatItem::ptr createItem(const std::string & key, const std::string &val)
            {
               if(key == "d") return std::make_shared<TimeFormatItem>(val);
               if(key == "t") return std::make_shared<ThreadFormatItem>();
               if(key == "c") return std::make_shared<LoggerFormatItem>();
               if(key == "f") return std::make_shared<FileFormatItem>();
               if(key == "l") return std::make_shared<LineFormatItem>();
               if(key == "p") return std::make_shared<LevelFormatItem>();
               if(key == "T") return std::make_shared<TabFormatItem>();
               if(key == "m") return std::make_shared<MsgFormatItem>();
               if(key == "n") return std::make_shared<NewLineFormatItem>();
               if(key.empty()) return std::make_shared<OtherFormatItem>(val);

               std::cout << "没有对应的格式化字符串：%" << key << std::endl;
               abort();
               return FormatItem::ptr();
            }
        private:
            std::string _pattern; //格式化规则字符串
            std::vector<FormatItem::ptr> _items; 
    };


}


#endif
```

### 1.2日志落地(LogSink)类设计(简单工厂模式)

⽇志落地类主要负责落地⽇志消息到⽬的地

它主要包括以下内容：

- Formatter⽇志格式化器：主要是负责格式化⽇志消息。
- mutex互斥锁：保证多线程⽇志落地过程中的线程安全，避免出现交叉输出的情况。这个类⽀持可扩展，其成员函数log设置为纯虚函数，当我们需要增加⼀个log输出⽬标，可以增加⼀个类继承⾃该类并重写log⽅法实现具体的落地⽇志逻辑。

目前实现了三个不同方向上的日志落地：

- 标准输出：`StdoutSink`

- 固定文件：`FileSink`

- 滚动文件：`RollSinkBySize`

  ◦  滚动⽇志⽂件输出的必要性：  

  ​	•  由于机器磁盘空间有限，我们不可能⼀直⽆限地向⼀个⽂件中增加数据  

  ​    •  如果⼀个⽇志⽂件体积太⼤，⼀⽅⾯是不好打开，另⼀⽅⾯是即时打开了由于包含数据巨⼤，也不利于查找我们需要的信息  

  ​	•  所以实际开发中会对单个⽇志⽂件的⼤⼩也会做⼀些控制，即当⼤⼩超过某个⼤⼩时（如1GB），我们就重新创建⼀个新的⽇志⽂件来滚动写⽇志。对于那些过期的⽇志，⼤部分企业内部都有专⻔的运维⼈员去定时清理过期的⽇志，或者设置系统定时任务，定时清理过期⽇志  

  ◦ ⽇志⽂件的滚动思想： 

  ⽇志⽂件滚动的条件有两个:⽂件⼤⼩和时间。我们可以选择：  

  - ⽇志⽂件在⼤于1GB的时候会更换新的⽂件  
  - 每天定点滚动⼀个⽇志⽂件 

  本项⽬基于⽂件⼤⼩的判断滚动⽣成新的⽂件  

  ```cpp
  /*  
      日志落地模块的实现
      1.抽象落地基类
      2.派生落地子类（根据不同的落地方向）
      3.使用工厂模式进行使用和生产的分离
  */
  
  #ifndef __M_SIK_H__
  #define __M_SIK_H__
  
  
  #include"util.hpp"
  #include<memory>
  #include<fstream>
  #include<cassert>
  #include<sstream>
  
  
  
  namespace xupt 
  {
      class LogSink
      {
          public:
              using ptr = std::shared_ptr<LogSink>;
              LogSink() {}
              virtual ~LogSink() {}
              virtual void log(const char* data, size_t len) = 0; 
      };  
  
      //落地方向：标准输出
      class StdoutSink : public LogSink
      {
          public:
              virtual void log(const char* data, size_t len)
              {
                  std::cout.write(data, len);
              }
      };
      //落地方向：指定文件
      class FileSink : public LogSink
      {
          public:
              FileSink(const std::string &pathname):_pathname(pathname)
              {
                  //1.创建日志文件所在的目录
                  util::File::CreateDirectory(util::File::path(_pathname));
                  //2.创建并打开日志文件
                  _ofs.open(_pathname, std::ios::binary | std::ios::app); //以二进制追加的方式写入
                  //断言以下是否打开成功
                  assert(_ofs.is_open());
              }
              //写入到文件
              virtual void log(const char* data, size_t len)
              {
                  _ofs.write(data, len);
                  assert(_ofs.good());
              }
          private:
              std::string _pathname;
              std::ofstream _ofs;
      };
      //落地方向：滚动文件（以大小进行滚动）
      class RollSinkBySize : public LogSink
      {
          public:
              //构造函数，传入文件名，文件大小，并管理文件输出句柄
              RollSinkBySize(const std::string &basename, size_t max_size):
              _basename(basename), _max_size(max_size),_cur_size(0),_name_count(0)
              {
                  std::string pathname = createNewFile();
                  //1.创建日志文件所在的目录                
                  util::File::CreateDirectory(util::File::path(pathname));
                  //2.创建并打开日志文件
                  _ofs.open(pathname, std::ios::binary | std::ios::app); //以二进制追加的方式写入
                  //断言以下是否打开成功
                  assert(_ofs.is_open());                    
              }
              //将日志消息写入到文件，写入前判断文件大小，超过了最大大小，就要切换文件
              virtual void log(const char* data, size_t len)
              {
                  if(_cur_size >= _max_size)
                  {
                      //先关闭之前的文件
                      _ofs.close();
                      //创建新的文件
                      std::string pathname = createNewFile();
                      _ofs.open(pathname,std::ios::binary | std::ios::app);
                      //断言以下是否打开成功
                      assert(_ofs.is_open()); 
                      //置零当前文件大小
                      _cur_size = 0;
                  }
                  //写入？没有更改_cur_size
                  _ofs.write(data, len);
                  //断言以下是否成功写入
                  assert(_ofs.good());   
                  _cur_size += len;            
              }
  
          private:
              //当文件大小达到限制的时候，用于切换文件使用（创建新的文件名，并不能创建文件）    
              std::string createNewFile()
              {
                  //获取系统时间
                  time_t t = util::Date::now();
                  struct tm lt;
                  localtime_r(&t, &lt);
                  //创建一个字符串流
                  std::stringstream filename;
                  filename << _basename;
                  filename << lt.tm_year + 1900;
                  filename << lt.tm_mon + 1;
                  filename << lt.tm_mday;
                  filename << lt.tm_hour;
                  filename << lt.tm_min;
                  filename << lt.tm_sec;
                  filename << "-";
                  filename << _name_count++;
                  filename << ".log";
  
                  return filename.str();
              }
          private:
              size_t _name_count ; //用于区别文件名的
              std::string _basename; //存储日志信息的文件, 命名规则我们一般是加一个固定的头，尾部用时间来区分
              std::ofstream _ofs; //维护一个我们的文件输出句柄
              size_t _max_size;     //单个文件的最大大小
              size_t _cur_size;    //当前文件的大小，算是一个优化，这样我们就不用去频繁查看文件属性，从而降低效率
      };
  
  /*
      拓展一个以时间作为文件滚动切换类型的日志落地模块
  */  
      class RollSinkByTime : public LogSink
      {
          public:
              enum class TimeGap
              {
                  GAP_SECOND,
                  GAP_MINUTE,
                  GAP_HOUR,
                  GAP_DAY,
              };
          
          public:
              RollSinkByTime(const std::string &basename, TimeGap gap_type):_basename(basename)
              {
                  switch(gap_type) 
                  {
                      case TimeGap::GAP_SECOND : _gap_size = 1; break;
                      case TimeGap::GAP_MINUTE : _gap_size = 60; break;
                      case TimeGap::GAP_HOUR : _gap_size = 3600; break;
                      case TimeGap::GAP_DAY : _gap_size = 3600 * 24; break;
                  }
                  
                  _cur_gap = _gap_size == 1 ? util::Date::now() : util::Date::now() % _gap_size;  //初始化当前的时间片
                  util::File::CreateDirectory(util::File::path(_basename)); //1.创建日志文件所在的目录      
                  std::string filename = createNewFile();  //2.创建文件名，并且打开文件
                  _ofs.open(filename,std::ios::binary | std::ios::app);
                  assert(_ofs.is_open());                 
  
              }
              //写入到文件,判断当前的时间段，是否是当前文件的时间段，不是则切换文件
              virtual void log(const char* data, size_t len)
              {
                  //获取系统时间
                  time_t cur = util::Date::now();                
  
                  if(cur % _gap_size != _cur_gap)
                  {
                      _ofs.close();
                      std::string filename = createNewFile();  
                      _ofs.open(filename,std::ios::binary | std::ios::app);
                      assert(_ofs.is_open());                                      
                  }
                  //写入文件
                  _ofs.write(data, len);
                  assert(_ofs.good());
              }
  
          private:
              //当时间片达到限制的时候，用于切换文件使用,（创建新的文件名，并不能创建文件）   
              std::string createNewFile()
              {
                  //获取系统时间
                  time_t t = util::Date::now();
                  struct tm lt;
                  localtime_r(&t, &lt);
                  //创建一个字符串流
                  std::stringstream filename;
                  filename << _basename;
                  filename << lt.tm_year + 1900;
                  filename << lt.tm_mon + 1;
                  filename << lt.tm_mday;
                  filename << lt.tm_hour;
                  filename << lt.tm_min;
                  filename << lt.tm_sec;
                  filename << "-";
                  filename << ".log";
  
                  return filename.str();
              }
          private:
              std::string _basename;
              std::ofstream _ofs;
              size_t _cur_gap;
              size_t _gap_size;
      };
  
      class SinkFactory
      {
          public:
              template<typename SinkType, typename ...Args>
              static LogSink::ptr create(Args && ...args) //右值引用，保持原来类型的属性，用于后来的完美转发
              {
                  return std::make_shared<SinkType>(std::forward<Args>(args)...);
              }
      };
  }
  
  
  
  #endif
  ```
  
  ​	