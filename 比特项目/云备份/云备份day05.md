# 云备份day05

## 1.Json实用工具类设计与实现

> 上面的代码定义了一个名为JsonUtil的类，其中包含了两个静态方法：Serialize和UnSerialize，用于序列化和反序列化JSON对象。
>
> - `Serialize` 方法接受一个 `Json::Value` 类型的参数 `root`，表示要序列化的JSON对象，以及一个指向 `std::string` 对象的指针 `str`，用于存储序列化后的JSON字符串。在方法内部，它使用JSONcpp提供的 `Json::StreamWriter` 将JSON对象序列化到一个 `std::stringstream` 对象中，并将其转换为字符串存储到指定的 `std::string` 对象中。
>
> - `UnSerialize` 方法接受一个 `std::string` 类型的参数 `str`，表示要反序列化的JSON字符串，以及一个指向 `Json::Value` 对象的指针 `root`，用于存储反序列化后的JSON对象。在方法内部，它使用JSONcpp提供的 `Json::CharReader` 尝试解析传入的JSON字符串，并将解析后的结果存储到指定的 `Json::Value` 对象中。
>
> 这些方法都是静态的，因此可以直接通过类名调用，无需创建类的实例。这段代码使用了JSONcpp库来处理JSON数据的序列化和反序列化，但在编译时出现了链接错误，可能是由于未正确链接JSONcpp库或未正确包含JSONcpp的头文件导致的。

```cpp
    class JsonUtil
    {
        public:
            static bool Serialize(const Json::Value &root, std::string *str)
            {
                // 序列化JSON对象。
                Json::StreamWriterBuilder swb; 
                // 创建StreamWriterBuilder对象，用于构建StreamWriter。
                std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter()); 
                // 使用StreamWriterBuilder创建StreamWriter的unique_ptr。
                std::stringstream ss;                                          
                // 创建一个stringstream对象，用作序列化的输出缓冲区。
                if(0 != sw->write(root, &ss))
                    // 使用StreamWriter将JSON对象序列化到stringstream。
                {
                    std::cout << "Json Write failed!\n";
                    return false;
                }                                    
                *str = ss.str();
                return true;
            }
            static bool UnSerialize(std::string &str, Json::Value *root)
            {
                Json::CharReaderBuilder crb;                          
                // 创建CharReaderBuilder对象。
                std::unique_ptr<Json::CharReader> cr(crb.newCharReader()); 
                // 使用CharReaderBuilder创建CharReader的unique_ptr。
                std::string err;                                           
                // 定义字符串以存储可能的错误信息。
                // 尝试解析JSON字符串。
                bool ret = cr->parse(str.c_str(), str.c_str() + str.size(), root, &err);
                if (ret == false) // 检查解析是否成功，这里有个逻辑错误，应该使用==比较。
                {
                    std::cout << err << std::endl; // 如果解析失败，输出错误信息。
                    return false;      
                }
                return true;
            }
    };
```

## 2.配置文件加载模块

### 2.1基本需求：

> 配置信息
> 1.热点判断时间热点管理:多长时间没有被访问的文件算是非热点文件
>
> 2.文件下载的url前缀路径--用于表示客户端请求是一个下载请求url: http://192.168.122.136:9090/path当用户发来一个备份列表查看请求 /listshow，我们如何判断这个不是一个listshow的文件下载请求
> /download/test.txt,/download/listshow
>
> 3.压缩包后缀名:订立的压缩包命名规则，就是在文件原名称之后加上后缀。".z'4.上传文件存放路径:决定了文件上传之后实际存储在服务器的哪里
>
> 5.压缩包存放路径:决定非热点文件压缩后存放的路径
> 6.服务端备份信息存放文件:服务端记录的备份文件信息的持久化存储7.服务器的监听IP地址:当程序要运行在其他主机上，则不需要修改程序
> 8.服务器的监听端口

### 2.2系统运行配置信息的单例类的设计

```cpp
// 系统运行配置信息的单例类设计
class Config
{
public:
    // 获取Config类的唯一实例
    static Config *GetInstance();

    // 获取热点判断时间
    int GetHotTime();

    // 获取服务器监听端口
    int GetServerPort();

    // 获取下载的URL前缀路径
    std::string GetDownloadPrefix();

    // 获取压缩包后缀名称
    std::string GetPackFileSuffix();

    // 获取备份文件存放目录
    std::string GetBackDir();

    // 获取压缩包存放目录
    std::string GetPackDir();

    // 获取服务器IP地址
    std::string GetServerIp();

    // 获取数据信息存放文件
    std::string GetBackupFile();

private:
    // 私有构造函数，防止外部实例化
    Config() {}

private:
    static std::mutex _mutex; // 互斥锁，用于保护_instance的创建过程
    static Config *_instance; // 唯一实例指针

    int _hotTime;                 // 热点判断时间
    int _serverPort;              // 服务器监听端口
    std::string _downloadPrefix;  // 下载的URL前缀路径
    std::string _packFileSuffix;  // 压缩包后缀名称
    std::string _backDir;         // 备份文件存放目录
    std::string _packDir;         // 压缩包存放目录
    std::string _serverIp;        // 服务器IP地址
    std::string _backupFile;      // 数据信息存放文件
};
```

填充：

> 这个类主要实现了以下功能：
>
> 1. **读取配置文件：** 类内部有一个私有方法 `ReadConfigFile()`，用于读取配置文件的内容，并将配置项的值保存在类的成员变量中。
>
> 2. **单例模式：** 类内部使用了单例模式，通过私有的构造函数和静态的 `GetInstance()` 方法确保只有一个 `Config` 实例被创建。在 `GetInstance()` 方法中，通过加锁的方式保证了在多线程环境下的线程安全性。
>
> 3. **配置项访问：** 类提供了一系列的公有方法，用于获取各种配置项的值，例如获取热点时间、服务器端口、服务器 IP 等。
>
> 4. **互斥锁保护：** 类内部使用了 `std::mutex` 作为静态成员变量 `_mutex`，用于保护单例模式的实现，在多线程环境下确保线程安全。
>
> 5. **错误处理：** 在读取配置文件过程中，如果出现错误，会输出相应的错误信息，并返回 `false`，表示读取配置文件失败。
>
> 总的来说，这个类的主要功能是读取配置文件并提供配置项的访问方法，同时使用了单例模式和互斥锁保证了线程安全性。

```cpp
#ifndef MY_CONFIG__
#define MY_CONFIG__

#include <mutex>
#include "Util.hpp"

namespace cloud
{
    // 定义配置文件路径
    #define CONFIG_FILE "./cloud.conf"

    // 配置类，用于读取配置文件
    class Config
    {
        private:
            // 构造函数私有化，确保单例模式
            Config()
            {
                ReadConfigFile();
            }

            // 读取配置文件的私有方法
            bool ReadConfigFile()
            {
                // 使用文件工具类读取配置文件内容
                FileUtil fu(CONFIG_FILE);
                std::string body;
                // 如果读取配置文件失败，则返回 false
                if(fu.GetContent(&body) == false)
                {
                    std::cout << "load config file failed!\n";
                    return false;
                }
                // 使用 JSON 解析工具类解析配置文件内容
                Json::Value root;
                if(JsonUtil::UnSerialize(body, &root) == false)
                {
                    std::cout << "parse config file failed!\n";
                    return false;
                }
                // 读取配置项并赋值
                _hot_time = root["hot_time"].asInt();
                _server_port = root["server_port"].asInt();
                _server_ip = root["server_ip"].asString();
                _download_prefix = root["download_prefix"].asString();
                _packfile_suffix = root["packfile_suffix"].asString();
                _back_dir = root["back_dir"].asString();
                _pack_dir = root["pack_dir"].asString();
                _backup_file = root["backup_file"].asString();

                return true;
            }

        public:
            // 获取配置单例的静态方法
            static Config* GetInstance()
            {
                // 如果实例不存在，则创建实例
                if(NULL == _instance)
                {
                    _mutex.lock();
                    if (NULL == _instance)
                    {
                        _instance = new Config();
                    }
                    _mutex.unlock();
                }

                return _instance;
            }

            // 获取热点时间配置项
            int GetHotTime()
            {
                return _hot_time;
            }

            // 获取服务器端口配置项
            int GetServerPort()
            {
                return _server_port;
            }

            // 获取服务器 IP 配置项
            std::string GetServerIp()
            {
                return _server_ip;
            }

            // 获取下载前缀配置项
            std::string GetDownloadPrefix()
            {
                return _download_prefix;
            }

            // 获取打包文件后缀配置项
            std::string GetPackFileSuffix()
            {
                return _packfile_suffix;
            }

            // 获取打包目录配置项
            std::string GetPackDir()
            {
                return _pack_dir;
            }

            // 获取备份目录配置项
            std::string GetBackDir()
            {
                return _back_dir;
            }

            // 获取备份文件配置项
            std::string GetBackupFile()
            {
                return _backup_file;
            }

        private:
            // 配置项成员变量
            int _hot_time;
            int _server_port;
            std::string _server_ip;
            std::string _download_prefix;
            std::string _packfile_suffix;
            std::string _back_dir;
            std::string _pack_dir;
            std::string _backup_file;

            // 单例实例和互斥锁静态成员变量
            static Config* _instance;
            static std::mutex _mutex;    
    };

    // 静态成员变量初始化
    Config* Config::_instance = NULL;
    std::mutex Config::_mutex;
}

#endif
```

## 3.数据管理类的设计

### 3.1基本需求

> **数据管理模块:**需要管理的数据有哪些
> 管理哪些数据，是因为后期要用到哪些数据
> 1.文件的实际存储路径:当客户端要下载文件时，则从这个文件中读取数据进行响应
>
> 2.文件压缩包存放路径名:如果这个文件是一个非热点文件会被压缩，则这个就是压缩包路径名称如果客户端要下载文件，则需要先解压缩，然后读取解压后的文件数据。
>
> 3.文件是否压缩的标志位:判断文件是否已经被压缩了
> 4.文件大小
> 5.文件最后一次修改时间
> 6.文件最后一次访问时间
> 7.文件访问URL中资源路径path:/download/a.txt
>
> 如何管理数据:
> 1.用于数据信息访问:使用hash表在内存中管理数据，以url的path作为key值--查询速度快
>
> 2.持久化存储管理:使用ison序列化将所有数据信息保存在文件中

### 3.2数据管理类的设计

```mysql
// 数据信息结构体: 用于存储备份信息
typedef struct BackupInfo_t
{
    bool pack_flag;          // 是否压缩标志
    size_t fsize;            // 文件大小
    time_t atime;            // 最后一次访问时间
    time_t mtime;            // 最后一次修改时间
    std::string real_path;   // 文件实际存储路径名称
    std::string pack_path;   // 压缩包存储路径名称
<<<<<<< HEAD
    std::string url;    // URL路径
=======
    std::string url_path;    // URL路径
>>>>>>> 88cf706b46ca916ba52c492de16f7fa03814e361
} BackupInfo;

// 数据管理类: 管理服务端系统中会用到的数据
class DataManager
{
private:
    std::string backup_file;                                 // 持久化存储文件
    std::unordered_map<std::string, BackupInfo> table;       // 内存中以哈希表存储
    pthread_rwlock_t rwlock;                                 // 读写锁--读共享，写互斥。
public:    
    // 构造函数
    DataManager();
    
    // 每次数据新增或修改都要重新持久化存储，避免数据丢失
    bool Storage(); 
    
    // 初始化加载，在每次系统重启都要加载以前的数据
    bool InitLoad();
    
<<<<<<< HEAD
    //将数据持久化到文件
    bool Storage();
    
=======
>>>>>>> 88cf706b46ca916ba52c492de16f7fa03814e361
    // 新增备份信息
    bool Insert(const BackupInfo &info);
    
    // 修改备份信息
    bool Update(const BackupInfo &info);
    
    // 根据URL获取备份信息
    bool GetOneByUrl(const std::string &url, BackupInfo *info);
    
    // 根据实际路径获取备份信息
    bool GetOneByRealpath(const std::string &path, BackupInfo *info);
    
    // 获取所有备份信息
    bool GetAll(std::vector<BackupInfo> *array);
};

// 创建新的备份信息对象
void NewBackupInfo(const std::string realpath, BackupInfo *info);

```

<<<<<<< HEAD
实现：

```cpp
#ifndef _MY_DATA__
#define _MY_DATA__

#include "Util.hpp"
#include <unordered_map>
#include <pthread.h>
#include "config.hpp"


namespace cloud
{
    typedef struct BackupInfo
    {
        bool pack_flag;        // 是否压缩标志
        size_t fsize;          // 文件大小
        time_t atime;          // 最后一次访问时间
        time_t mtime;          // 最后一次修改时间
        std::string real_path; // 文件实际存储路径名称
        std::string pack_path; // 压缩包存储路径名称
        std::string url;  // URL路径

        // 创建新的备份信息对象
        bool NewBackupInfo(const std::string& realpath)
        {
            cloud::FileUtil fu(realpath);
            if(fu.Exists() == false)
            {
                std::cout << "New BackupInfo File not exists" << std::endl;
                return false;
            }            
            Config* config = Config::GetInstance();
            std::string packdir = config->GetPackDir();
            std::string packsuffix = config->GetPackFileSuffix();
            std::string download_preffix = config->GetDownloadPrefix();


            this->pack_flag = false;
            this->fsize = fu.FileSize();
            this->mtime = fu.LastMTime();
            this->atime = fu.LastATime();
            this->real_path = realpath;
            // ./packdir/a.txt ---> ./packdir/a.txt.lz
            this->pack_path = packdir + fu.FileName() + packsuffix;
            // ./packdir/a.txt ---> /download/a.txt
            this->url = download_preffix + fu.FileName();

            return true;
        }
    }BackupInfo;

    class DataManager
    {
    public:
        DataManager()
        {
            _backup_file = Config::GetInstance()->GetBackupFile();
            pthread_rwlock_init(&_rwlock, nullptr); //初始化读写锁
            InitLoad();
        }
        ~DataManager()
        {
            pthread_rwlock_destroy(&_rwlock); //销毁读写锁
        }
        bool Insert(const BackupInfo &info)
        {
            pthread_rwlock_wrlock(&_rwlock);
            _table[info.url] = info;
            pthread_rwlock_unlock(&_rwlock);
            Storage();
            return true;
        }
        bool Updata(const BackupInfo &info)
        {
            pthread_rwlock_wrlock(&_rwlock);
            _table[info.url] = info;
            pthread_rwlock_unlock(&_rwlock);
            Storage();
            return true;
        }
        bool GetOneByURL(const std::string& url, BackupInfo *info)
        {
            pthread_rwlock_wrlock(&_rwlock); 
            //因为url是key,所以我们可以选择直接查找的方法
            auto it = _table.find(url);
            if(it == _table.end())
            {
                pthread_rwlock_unlock(&_rwlock);
                return false;
            }   
            *info = it->second;
            pthread_rwlock_unlock(&_rwlock);
            return true;

        }
        bool GetOneByRealPath(const std::string& realpath, BackupInfo *info)
        {
            pthread_rwlock_wrlock(&_rwlock); 
            auto it = _table.begin();
            for(; it != _table.end(); ++it)
            {
                if(it->second.real_path == realpath)
                {
                    *info = it->second;
                    pthread_rwlock_unlock(&_rwlock);
                    return true;
                }
            }
            pthread_rwlock_unlock(&_rwlock);
            return false;
        }

    bool GetAll(std::vector<BackupInfo> *arry)
    {
        pthread_rwlock_wrlock(&_rwlock);
        auto it = _table.begin();
        for (; it != _table.end(); ++it)
        arry->push_back(it->second);

        pthread_rwlock_unlock(&_rwlock);
        return true; 
    }

    bool InitLoad()
    {
        //1.将数据文件中的数据读取出来
        FileUtil fu(_backup_file);
        if(fu.Exists() == false)
        {
            std::cout << "InitLoad file not eixst!\n";
            return false;
        }
        std::string body;
        fu.GetContent(&body);
        //2.反序列化
        Json::Value root;
        JsonUtil::UnSerialize(body, &root);
        //3.将反序列化得到的内容添加到table中
        for(int i = 0; i < root.size(); ++i)
        {
            BackupInfo info;
            info.pack_flag = root[i]["pack_flag"].asBool();
            info.fsize = root[i]["fsize"].asInt64();
            info.atime = root[i]["atime"].asInt64();
            info.mtime = root[i]["mtime"].asInt64();
            info.real_path = root[i]["real_path"].asString();
            info.pack_path = root[i]["pack_path"].asString();
            info.url = root[i]["url"].asString();
            Insert(info);
        }

        return true;
    }

    bool Storage()
    {
        //1.获取所有数据
        std::vector<BackupInfo> arry;
        this->GetAll(&arry);
        //2.添加到Json::Value
        Json::Value root;
        for(int i = 0; i < arry.size(); ++i)
        {
            Json::Value item;
            item["pack_flag"] = arry[i].pack_flag;
            item["fsize"] = (Json::Int64)arry[i].fsize;
            item["atime"] = (Json::Int64)arry[i].atime;
            item["mtime"] = (Json::Int64)arry[i].mtime;
            item["real_path"] = arry[i].real_path;
            item["pack_path"] = arry[i].pack_path;
            item["url"] = arry[i].url;

            root.append(item); //添加数组元素
        }
        //3.对Json::Value进行序列化
        std::string body;
        JsonUtil::Serialize(root, &body);

        //4.写文件
        FileUtil fu(_backup_file);
        fu.SetContent(body);
        return true;
    }

    private:
        std::string _backup_file;
        pthread_rwlock_t _rwlock; 
        std::unordered_map<std::string, BackupInfo> _table;
    };


}





#endif
```

## 4.热点管理模块

> 热点管理模块:对服务器上备份的文件进行检测，哪些文件长时间没有被访问，则认为是非热点文件，则压缩存储，节省磁盘空间。
> 实现思路:
> 	遍历所有的文件，检测文件的最后一次访问时间，与当前时间进行相减得到差值，这个差值如果大于设定好的非热点判断时间则认为是非热点文件，则进行压缩存放到压缩路径中，除源文件遍历所有的文件:
> 	1.从数据管理模块中遍历所有的备份文件信息
>
> ​	2.遍历备份文件夹，获取所有的文件进行属性获取，最终判断
>
> ​	选择第二种:遍历文件夹，每次获取文件的最新数据进行判断，并且还可以解决数据信息缺漏的问题
>
> 1.遍历备份目录，获取所有文件路径名称
> 2.逐个文件获取最后一次访问时间与当前系统时间进行比较判断
> 3.对非热点文件进行压缩处理，删除源文件
> 4.修改数据管理模块对应的文件信息(压缩标志->true)

```cpp
extern DataManager *data;
class HotManager
{
private:
    std::string back dir; // 备份文件路径
    std::string _pack_dir;//压缩文件路径
    std::string _pack_suffix;//压缩包后缀名
    int hot time; //热点判断时间
public:
    HotManager() 
    bool RunModule();
}
```

实现：

```cpp
#ifndef __MY_HOT__
#define __MY_HOT__

#include "data.hpp"
#include <unistd.h>


extern cloud::DataManager* _data;
namespace cloud
{

    class HotManager
    {
    public:
        HotManager()
        {
            Config* config = Config::GetInstance();
            _back_dir = config->GetBackDir();
            _pack_dir = config->GetPackDir();
            _pack_suffix = config->GetPackFileSuffix();
            _hot_time = config->GetHotTime();
            FileUtil tmp1(_back_dir);
            FileUtil tmp2(_pack_dir);
            tmp1.CreateDirectory();
            tmp2.CreateDirectory();
        }
        //非热点文件返回真，热点文件返回假
        bool HotJudge(const std::string &filename)
        {
            FileUtil fu(filename);
            time_t last_atime = fu.LastATime();
            time_t cur_time = time(NULL);
            if(cur_time - last_atime > _hot_time)
            {
                return true;
            }
            return false;
        }
        bool RunModule()
        {
            while(1)
            {
                // 1.遍历备份文件，获取所有文件名
                FileUtil fu(_back_dir);
                std::vector<std::string> arry;
                fu.ScanDirectory(&arry);
                // 2.遍历判断文件是否是非热点文件
                for (auto &a : arry)
                {
                    if (HotJudge(a) == false)
                    {
                        continue; // 热点文件不需要处理
                    }
                    // 4.获取文件备份信息
                    BackupInfo bi;
                    if (false == _data->GetOneByRealPath(a,&bi))
                    {
                        // 现在有一个文件存在，但是没有备份信息
                        bi.NewBackupInfo(a); // 设置一个新的备份信息
                    }
                    // 4.对非热点文件进行压缩处理
                    FileUtil tmp(a);
                    tmp.Compress(bi.pack_path);

                    // 4.删除源文件
                    tmp.Remeove();
                    bi.pack_flag = true;
                    _data->Updata(bi);
                }
                usleep(1000); //避免空目录，频繁循环，消耗cpu资源
            }
            return true;
        }

    private:
        std::string _back_dir;
        std::string _pack_dir;
        std::string _pack_suffix;
        int _hot_time;
    };




}

#endif
```

## 5.服务端业务处理模块

### 5.1服务端业务处理模块的设计

> 服务端业务处理模块：将网络通信模块和业务进行了合并（网络通信通过httplib库完成）
>
> 1.搭建网络通信服务器: 借助httplib完成
>
> 2.业务请求处理
>
> ​	1.文件上传请求:备份客户端上传的文件响应上传成功
> ​	2.文件列表请求:客户端浏览器请求一个备份文件的展示页面，响应页面载，响应客户端要下载的文件数据                   	3.文件下载请求:通过展示页面，点击下载， 响应客户端要下载的文件数据

![image-20240421214318587](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240421214318587.png)

代码框架设计：

![image-20240421214930641](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240421214930641.png)

Etag字段：

![image-20240422113709461](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240422113709461.png)

### 5.2服务端业务处理模块的实现

上传，下载，展示

```cpp
#ifndef __MY_SERVER__
#define __MY_SERVER__


#include<ctime>
#include<string>
#include "data.hpp"
#include "httplib.h"


extern cloud::DataManager* _data;
namespace cloud
{
    class Service
    {
    private:
        static void Upload(const httplib::Request &req, httplib::Response &rsp)
        {
            auto ret = req.has_file("file"); // 判断有没有上传文件区域
            if (ret == false)
            {
                rsp.status = 400;
                return;
            }

            // 在云端创建对应的文件，并且将上传的文件中的信息拷贝到对应的文件
            const auto &file = req.get_file_value("file");
            std::string back_dir = Config::GetInstance()->GetBackDir();
            std::string realpath = back_dir + FileUtil(file.filename).FileName();
            FileUtil fu(realpath);
            fu.SetContent(file.content);

            // 将备份文件的文件信息插入到表中
            BackupInfo info;
            info.NewBackupInfo(realpath);
            _data->Insert(info);
        }
        static std::string GetTimeStr(const time_t time)
        {
            std::string t = ctime(&time);
            return t;
        }
        static void ListShow(const httplib::Request &req, httplib::Response &rsp)
        {
            //1.获取所有文件备份信息
            std::vector<BackupInfo> arry;
            _data->GetAll(&arry);
            //2.根据所有备份信息。组织html文件数据
            std::stringstream ss;
            ss << "<html><head><title>Download</title></head>";
            ss << "<body><h1>Download</h1><table>";
            for(auto& a : arry)
            {
                ss << "<tr>";
                std::string filename = FileUtil(a.real_path).FileName();
                ss << "<td><a href='" << a.url << "'>" << filename << "</a></td>";
                ss << "<td align='right'>"<< GetTimeStr(a.mtime) << "</td>";
                ss << "<td align='right'>"<< a.fsize / 1024 << "k</td>";
                ss << "</tr>";
            }
            ss << "</table></body></html>";
            rsp.body = ss.str();
            //设置响应报头
            rsp.set_header("Content-Type", "text.html");
            rsp.status = 200;
        }
        static std::string GetEtag(const BackupInfo& info)
        {
            //etag = filename + fsize + mtime;
            FileUtil fu(info.real_path);
            std::string etag = fu.FileName();
            etag += "-" + std::to_string(info.fsize);
            etag += "-" + std::to_string(info.mtime);
            return etag;
        }
        static void Download(const httplib::Request &req, httplib::Response &rsp)
        {
            //1.获取客户端请求的资源路径path ---> req.path
            //2.根据资源路径获取，获取文件备份信息
            BackupInfo info;
            _data->GetOneByURL(req.path, &info);
            //3.判断文件是否被压缩，如果被压缩则解压
            if(info.pack_flag ==true)
            {
                //解压缩
                FileUtil fu(info.pack_path);
                fu.UnCompress(info.real_path);
                //删除压缩包
                fu.Remeove();
                //修改是否被压缩的标志位
                info.pack_flag = false;
                //更新文件属性
                _data->Updata(info);
            }
            //4.读取文件数据放入到rsp.body中
            FileUtil fu(info.real_path);
            fu.GetContent(&rsp.body);
            //6.设置响应头部字段：Etag, Accept-Ranges: bytes
            rsp.set_header("Accept-Ranges", "bytes");
            rsp.set_header("ETag", GetEtag(info));
            rsp.set_header("Content-Type", "application/octet-stream");
            rsp.status = 200;
        }

    public:
        Service()
        {
            Config* config = Config::GetInstance();
            _server_port = config->GetServerPort();
            _server_ip = config->GetServerIp();
            _download_prefix = config->GetDownloadPrefix();
        }
        bool RunModule()
        {
            _server.Post("/upload", Upload);
            _server.Get("/listshow",ListShow);
            _server.Get("/", ListShow);
            std::string download_url = _download_prefix + "(.*)";
            _server.Get(download_url, Download);

            _server.listen("0.0.0.0", _server_port);
            return true;    
        }
   
    private:
        int _server_port;
        std::string _server_ip;
        std::string _download_prefix;
        httplib::Server _server;
    };
}


#endif
```

断点续传：

> **功能:**
>
> ​	当文件下载过程中，因为某种异常而断，如果再次进行从头下载，效率较低，因为需要将之前已经传输过的数据再次传输一遍，因此断点续传就是从上次下载断开的位置，重新下载即可，之前已经传输过的数据将不需要在重新传输。
>
> **目的:**
>
> ​	提高文件重新传输效率
> **实现思想:**
>
> ​	数据写入文件后记录自己当前下载的数据量。客户端在下载文件的时候，要每次接收至当异常下载中断时，下次断点续传的时候将要重新下载的数据区间(下载起始位置，结束位置)发送给服务器，服务器收到后，仅回传客户端需要的区间数据即可需要考虑的问题:如果上次下载文件之后，这个文件在服务器上被修改了，则这时候将不能重新断点续传，而是应该重新进行于文件下载操作。

在http协议中断点续传的实现：

 	主要关键点：

​			1.在于能够告诉服务器下载区间

​			2.服务器上要能够检测上一次下载之后这个文件是否被修改过

实现：

![image-20240422121659575](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240422121659575.png)

实现：

在download函数的基础上去添加：

```cpp
static void Download(const httplib::Request &req, httplib::Response &rsp)
        {
            //1.获取客户端请求的资源路径path ---> req.path
            //2.根据资源路径获取，获取文件备份信息
            BackupInfo info;
            _data->GetOneByURL(req.path, &info);
            //3.判断文件是否被压缩，如果被压缩则解压
            if(info.pack_flag ==true)
            {
                //解压缩
                FileUtil fu(info.pack_path);
                fu.UnCompress(info.real_path);
                //删除压缩包
                fu.Remeove();
                //修改是否被压缩的标志位
                info.pack_flag = false;
                //更新文件属性
                _data->Updata(info);
            }

            //4.读取文件数据放入到rsp.body中
            FileUtil fu(info.real_path);



            bool retrans = false;
            std::string old_etag; //原先下载的etag;
            //判断是否是断点续传
            if(req.has_header("If-Range"))
            {   
                old_etag = req.get_header_value("If-Range");
                //etag一致
                if(old_etag == GetEtag(info))
                {
                    retrans = true;
                }                
            }
            //若没有If-Range字段或者有该字段，但是etag不一致，则从头下载
            
            if(retrans == false)
            {
                fu.GetContent(&rsp.body);
                // 6.设置响应头部字段：Etag, Accept-Ranges: bytes
                rsp.set_header("Accept-Ranges", "bytes");
                rsp.set_header("ETag", GetEtag(info));
                rsp.set_header("Content-Type", "application/octet-stream");
                rsp.status = 200;
            }
            else 
            {
                //httplib内部实现了对于区间请求的处理
                //只需要我们用户将文件所有数据读取到rsp.body中，它内部会自动根据请求区间
                //从body中取出指定区间的数据进行相应
                //std::string range = req.get_header_val("Range"); //bytes=start-end;
                fu.GetContent(&rsp.body);
                rsp.set_header("Accept-Ranges", "bytes");
                rsp.set_header("ETag", GetEtag(info));
                rsp.set_header("Content-Type", "application/octet-stream");
                //rsp.set_header("Content-Range", "bytes start-end/fsize");
                rsp.status = 206;  //区间下载的响应
            }
        }
```

## 6.客户端业务处理模块

### 6.1客户端功能和模块化设计

![image-20240422170815794](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240422170815794.png)

### 6.2移植一部分代码到windows下的vs

Util.hpp:

- 删除了不必要的压缩/解压缩函数
- 删除了JsonUtil类

```hpp
#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
//这个宏是因为在新版本里vs下已经弃用#include <experimental/filesystem>头文件
//但是我们还是继续使用，所以声明一下，防止警告

#include <iostream>
#include <fstream>
#include <vector>
#include <experimental/filesystem>

#include <sys/types.h>
#include <sys/stat.h>




namespace cloud
{
    namespace fs = std::experimental::filesystem;

    class FileUtil
    {

    public:
        FileUtil(const std::string& filename)
            :_filename(filename)
        {}
        // 删除文件
        bool Remeove()
        {
            if (this->Exists() == false)
            {
                return true;
            }
            remove(_filename.c_str());
            return true;
        }
        // 1.获取文件大小
        size_t FileSize()
        {
            struct stat st;
            if (stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "Got file size failed!\n";
                return 0;
            }
            return st.st_size;
        }
        // 获取文件最后一次修改时间                                        
        time_t LastMTime()
        {
            struct stat st;
            if (stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "Got file size failed!\n";
                return -1;
            }
            return st.st_mtime;
        }
        // 获取文件最后一次访问时间                                    
        time_t LastATime()
        {
            struct stat st;
            if (stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "Got file size failed!\n";
                return -1;
            }
            return st.st_atime;
        }
        // 获取文件的名称 /abc/test.txt -> test.txt                           
        std::string FileName()
        {
            size_t pos = _filename.find_last_of("/");
            if (pos == std::string::npos)
            {
                return _filename;
            }
            return _filename.substr(pos + 1);
        }
        // 获取文件指定位置 指定长度的数据
        bool GetPosLen(std::string* body, size_t pos, size_t len)
        {
            size_t fsize = this->FileSize();
            if (pos + len > fsize)
            {
                std::cout << "get file len is error\n";
                return false;
            }
            std::ifstream ifs;
            ifs.open(_filename, std::ios::binary);
            if (ifs.is_open() == false)
            {
                std::cout << "read open file failed\n";
                return false;
            }
            ifs.seekg(pos, std::ios::beg);
            body->resize(len);
            ifs.read(&(*body)[0], len);
            if (ifs.good() == false)
            {
                std::cout << "read file is failed\n";
                ifs.close();
                return false;
            }
            ifs.close();
            return true;
        }
        // 从文件中读取数据
        bool GetContent(std::string* body)
        {
            size_t fsize = this->FileSize();
            return GetPosLen(body, 0, fsize);
        }
        // 向文件写入数据                                      
        bool SetContent(const std::string& body)
        {
            std::ofstream ofs;
            ofs.open(_filename, std::ios::binary);
            if (ofs.is_open() == false)
            {
                std::cout << "write open file error\n";
                return false;
            }
            ofs.write(&body[0], body.size());
            if (ofs.good() == false)
            {
                std::cout << "write file error\n";
                ofs.close();
                return false;
            }
            ofs.close();
            return true;
        }

        // 判断文件是否存在
        bool Exists()
        {
            return fs::exists(_filename);
        }
        // 创建目录                                              
        bool CreateDirectory()
        {
            if (this->Exists()) return true;
            else return fs::create_directories(_filename);
        }
        // 浏览获取目录下所有文件路径名                        
        bool ScanDirectory(std::vector<std::string>* arry)
        {
            for (auto& p : fs::directory_iterator(_filename))
            {
                if (fs::is_directory(p) == true)
                {
                    continue;
                }
                //relative.path带有路径的文件名
                arry->push_back(fs::path(p).relative_path().string());
            }
            return true;
        }

    private:
        std::string _filename;
    };

}
```

### 6.3数据管理类的设计

> 这个类 `Datamanager` 用于管理备份信息，它主要负责在文件系统中存储和检索数据。以下是其主要功能和设计要点：
>
> 1. **备份文件的管理**：
>    - `Datamanager` 类通过 `_backupFile` 成员变量维护一个备份文件的路径。
>    - 它负责在对象初始化时加载备份文件的内容，并在对象销毁时将数据存储到备份文件中。
>
> 2. **数据的存储与检索**：
>    - `Datamanager` 使用一个 `std::unordered_map<std::string, std::string>` 作为内部数据结构，用于存储键值对信息。键通常代表数据的唯一标识符，值可以是相关的元数据或文件路径等。
>    - 提供了插入、更新和检索的方法，如 `Insert`、`UpData`、`GetOneByKey`。
>
> 3. **文件持久化和初始化**：
>    - 在对象构造时，`InitLoad` 方法从备份文件中加载数据，解析后存入 `_table`。
>    - 在对象析构时，`Storage` 方法将 `_table` 中的数据保存到备份文件中，确保持久化。
>    - `Split` 方法用于将字符串按特定分隔符进行分割，为数据加载和解析提供支持。
>

```cpp
#ifndef __MY_DATA__
#define __MY_DATA__


#include "Util.hpp"
#include <unordered_map>
#include <sstream>

namespace cloud
{
	class Datamanager
	{
	public:
		Datamanager(const std::string& backup_file)
			:_backup_file(backup_file)
		{
			InitLoad();
		}
		~Datamanager()
		{
			Storage();
		}
		bool Storage()
		{
			//1.获取所有的备份信息
			std::stringstream ss;
			auto it = _table.begin();
			for (; it != _table.end(); ++it)
			{
				//2.将所有的信息进行持久化格式组织
				ss << it->first << " " << it->second << "\n";
			}
			//3.将所有的信息进行持久化存储
			FileUtil fu(_backup_file);
			fu.SetContent(ss.str());

			return true;
		}
		int Split(const std::string& body, const std::string& sep, std::vector<std::string>* arry)
		{
			int count = 0;
			size_t pos = 0, idx = 0;
			while ((pos = body.find(sep, idx)) != std::string::npos)
			{
				std::string tmp = body.substr(idx, pos - idx);
				arry->push_back(tmp);
				count++;
				idx = pos + sep.size();
			}
			// 如果最后一段不是空的，才添加到数组
			if (idx < body.size())
			{
				std::string tmp = body.substr(idx);
				arry->push_back(tmp);
				count++;
			}
			return count;
		}

		bool InitLoad()
		{
			//1.获取备份信息
			FileUtil fu(_backup_file);
			std::string body;
			fu.GetContent(&body);
			//2.按照格式分割
			std::vector<std::string> arry;
			Split(body, "\n", &arry);
			//3.将分割后的结果插入到_table中
			for (auto& a : arry)
			{
				//b.txt b.txt-34567-345636
				std::vector<std::string> tmp;
				Split(a, " ", &tmp);
				if (tmp.size() != 2)
				{
					continue;
				}
				_table[tmp[0]] = tmp[1];
			}
			return true;
		}
		bool Insert(const std::string& key, const std::string& val)
		{
			_table[key] = val;
			return true;
		}
		bool UpData(const std::string& key, const std::string& val)
		{
			_table[key] = val;
			return true;
		}
		bool GetOneByKey(const std::string& key, std::string* val)
		{
			auto it = _table.find(key);
			if (it == _table.end())
			{
				return false;
			}
			*val = it->second;
			return true;
		}
	private:
		std::string _backup_file; //备份信息的持久化文件
		std::unordered_map<std::string, std::string> _table; //存储文件唯一标识符：文件路径+唯一标识
	};
}

#endif 
```

### 6.4文件备份类的设计

![image-20240422205547856](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240422205547856.png)

实现：

```cpp
#ifndef __MY_CLOUD__
#define __MY_CLOUD__


#define SERVER_ADDR "117.72.37.100"
#define SERVER_PORT 9090

#include "data.hpp"
#include "httplib.h"


#include <windows.h>



namespace cloud
{

	class Backup
	{
	public:
		Backup(const std::string back_dir, const std::string back_file)
			:_back_dir(back_dir)
		{
			_data = new Datamanager(back_file);
		}
		std::string GetFileIdentifier(const std::string filename)
		{
			//b.txt-b.size-b.mtime
			FileUtil fu(filename);
			std::stringstream ss;
			ss << fu.FileName() << "-" << fu.FileSize() << "-" << fu.LastMTime();

			return ss.str();
		}

		bool UpLoad(const std::string &filename)
		{
			//1.获取文件数据
			FileUtil fu(filename);
			std::string body;
			fu.GetContent(&body);
			//2.搭建http客户端上传文件数据
			httplib::Client client(SERVER_ADDR, SERVER_PORT);
			httplib::MultipartFormData item;
			item.content = body;
			item.filename = fu.FileName();
			item.name = "file";
			item.content_type = "application/octet-stream";
			httplib::MultipartFormDataItems items;
			items.push_back(item);
			auto res = client.Post("./upload", items);
			if (!res || res->status != 200)
			{
				return false;
			}
			return true;
		}

		bool IsNeedUpLoad(const std::string filename)
		{
			//我们判断文件是否是新增的，若不是新增的看他是否被修改了
			std::string id;
			if (_data->GetOneByKey(filename, &id) != false)
			{
				//有历史记录
				std::string new_id = GetFileIdentifier(filename);
				//证明存在且未被修改,则不需要被更新
				if (new_id == id)
				{
					return false;
				}
			}
			//但是对于大文件，当它进行上传的时候，因为我们每次检测，他都存在，并且近期都被修改
			//那么简单的方式就是，我们可以设定一个时间阈值，用于简单的判断是否需要更新
			FileUtil fu(filename);
			if (time(NULL) - fu.LastMTime() < 3)
			{
				return false;
			}

			return true;

		}
		bool RunMudule()
		{
			while (1)
			{
				//1.遍历获取指定文件夹中的所有文件
				//2.逐个判断文件是否需要上传
				//3.如果需要上传则上传文件
				FileUtil fu(_back_dir);
				std::vector <std::string> arry;
				fu.ScanDirectory(&arry);
				for (auto& a : arry)
				{
					if (IsNeedUpLoad(a) == false)
					{
						continue;
					}
					if (UpLoad(a) == true)
					{
						_data->Insert(a, GetFileIdentifier(a));
					}
				}
				Sleep(1);
				_data->Storage();
			}
		}


	private:
		std::string _back_dir; //备份文件夹
		Datamanager* _data;  //数据管理对象
	};


}


#endif 


```
