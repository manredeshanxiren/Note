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
    std::string url_path;    // URL路径
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

