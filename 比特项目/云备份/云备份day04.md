# 云备份day04

## 1.文件实用工具类

### 1.1文件实用工具类的整体框架

```cpp
class
{
private:
    std::string filename;
public: 
    size_t FileSize(); // 1.获取文件大小
    time_t LastMTime();// 获取文件最后一次修改时间
    time_t LastATime();// 获取文件最后一次访问时间std::string FileName();//获取文件路径名中的文件名称 /abc/test.txt -> test.txt
    std::string FileName();                                        // 获取文件的名称
    bool SetContent(const std::string &body); // 向文件写入数据
    bool GetContent(std::string *body)   //从文件中读取数据
    bool GetPosLen(std::string *body, size t pos, size t len); // 获取文件指定位置 指定长度的数据
    bool Exists();        // 判断文件是否存在
    bool CreateDirectory();   // 创建目录
    bool GetDirectory(std::vector<std::string> *arry);  // 浏览获取目录下所有文件路径名
    bool Compress(const std::string &packname);         // 压缩文件
    bool UnCompress(const std::string &filename);       // 解压缩文件
}
```

### 1.2文件工具类的实现

#### 1.2.1文件属性和名称获取

> `struct stat` 是在 C 和 C++ 中用来获取文件或文件系统状态信息的结构体。它通常用于系统编程和文件操作中，可以提供有关文件的诸多属性。
>
> 下面是 `struct stat` 结构体的一般定义：
>
> ```c
> struct stat {
>     dev_t         st_dev;         // 设备 ID
>     ino_t         st_ino;         // i 节点号
>     mode_t        st_mode;        // 文件模式
>     nlink_t       st_nlink;       // 链接数
>     uid_t         st_uid;         // 用户 ID
>     gid_t         st_gid;         // 组 ID
>     dev_t         st_rdev;        // 特殊设备 ID
>     off_t         st_size;        // 文件大小（字节数）
>     blksize_t     st_blksize;     // 文件系统 I/O 缓冲区大小
>     blkcnt_t      st_blocks;      // 文件所占块数
>     struct timespec st_atim;      // 最后一次访问时间
>     struct timespec st_mtim;      // 最后一次修改时间
>     struct timespec st_ctim;      // 最后一次状态变化时间
>     ...
> };
> ```
>
> 这里列出了一些常用的成员，但实际上 `struct stat` 可能会包含更多的成员，具体取决于操作系统和文件系统的支持。
>
> 下面是一些常用的 `struct stat` 成员：
>
> - `st_dev`：文件所在设备的设备号。
> - `st_ino`：文件的 i 节点号。
> - `st_mode`：文件的权限和文件类型。
> - `st_nlink`：文件的硬链接数。
> - `st_uid`：文件所有者的用户 ID。
> - `st_gid`：文件所有者的组 ID。
> - `st_size`：文件大小（以字节为单位）。
> - `st_blksize`：文件系统 I/O 缓冲区大小。
> - `st_blocks`：文件所占的块数。
> - `st_atim`、`st_mtim`、`st_ctim`：文件的最后访问时间、修改时间和状态变化时间。
>
> 通过 `stat()` 或 `fstat()` 等系统调用，可以填充 `struct stat` 结构体并获取文件的各种信息。这些信息可以用于程序中进行文件操作、权限检查等。

```cpp
        FileUtil(const string &filename)
        :_filename(filename)
        {}
        // 1.获取文件大小
        int64_t FileSize()
        {
            struct stat st;
            if(stat(_filename.c_str(), &st) < 0)
            {
                cout << "Got file size failed!" << endl;
                return -1;
            }
            return st.st_size;
        }
        // 获取文件最后一次修改时间                                        
        time_t LastMTime()
        {
            struct stat st;
            if (stat(_filename.c_str(), &st) < 0)
            {
                cout << "Got file size failed!" << endl;
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
                cout << "Got file size failed!" << endl;
                return -1;
            }
            return st.st_atime;
        }          
        // 获取文件的名称 /abc/test.txt -> test.txt                           
        std::string FileName()
        {
            size_t pos = _filename.find_last_of("/");
            if(pos == std::string::npos)
            {
                return _filename;
            }
            return _filename.substr(pos+1);
        }  
```

#### 1.2.2文件的读写操作

```cpp
        // 获取文件指定位置 指定长度的数据
        bool GetPosLen(std::string *body, size_t pos, size_t len)
        {
            size_t fsize = this->FileSize();
            if(pos + len > fsize)
            {
                std::cout << "get file len is error\n";
                return false;
            }
            std::ifstream ifs;
            ifs.open(_filename, std::ios::binary);
            if(ifs.is_open() == false)
            {
                std::cout << "read open file failed\n";
                return false;
            }
            ifs.seekg(pos, std::ios::beg);
            body->resize(len);
            ifs.read(&(*body)[0], len);
            if(ifs.good() == false)
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
        bool SetContent(const std::string &body)
        {
            std::ofstream ofs;
            ofs.open(_filename, std::ios::binary);
            if(ofs.is_open() == false) 
            {
                std::cout << "write open file error\n";
                return false;
            }
            ofs.write(&body[0], body.size());
            if(ofs.good() == false)
            {
                std::cout << "write file error\n";
                ofs.close();
                return false;
            }
            ofs.close();
            return true;
        }               

```

#### 1.2.3文件的解压缩

```cpp
        // 压缩文件
        bool Compress(const std::string &packname)
        {
            //1.获取源文件的数据
            std::string body;
            if(this->GetContent(&body) == false)
            {
                std::cout << "Compress GerContent failed\n";
                return false;
            }
            //2.对数据进行压缩
            std::string packed = bundle::pack(bundle::LZIP, body);
            //3.将压缩后的数据存储在压缩包文件中
            FileUtil fu(packname);
            if(fu.SetContent(body) == false)
            {
                std::cout << "Compress SetContent error\n";
                return false;
            }
            return true;
        }    
        // 解压缩文件                
        bool UnCompress(const std::string &filename)
        {
            //将当前压缩包的数据读取出来
            std::string body;
            if(this->GetContent(&body) == false)
            {
                std::cout << "Uncompress GetContent error\n";
                return false;
            }
            //解压数据
            std::string unpacked = bundle::unpack(body);
            //将解压后的数据放入到filename中
            FileUtil fu(filename);
            if(fu.SetContent(unpacked) == false)
            {
                std::cout << "Uncompress Setcontent error\n";
                return false;
            }
            return true;
        }
```

#### 1.2.4目录的操作

```cpp
        
		// 判断文件是否存在
        bool Exists()
        {
            return fs::exists(_filename);
        }
        // 创建目录                                              
        bool CreateDirectory()
        {
            if(this->Exists()) return true;
            else return fs::create_directories(_filename);
        }            
        // 浏览获取目录下所有文件路径名                        
        bool ScanDirectory(std::vector<std::string> *arry)
        {
            for(auto& p : fs::directory_iterator(_filename))
            {
                if(fs::is_directory(p) == true)
                {
                    continue;
                }
                //relative.path带有路径的文件名
                arry->push_back(fs::path(p).relative_path().string());
            }
            return true;
        }
```

