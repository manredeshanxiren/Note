# 云备份day01

## 1.服务端功能和模块细分

### 1.1服务端功能的细分

> - 支持客户端文件上传功能
> - 支持客户端文件备份列表查看功能
> - 支持客户端文件下载功能(断点续传)
> - 热点文件管理功能(对长时间无访问的文件进行压缩存储)

### 1.2服务端模块的划分

> - 数据管理模块(管理备份的文件信息，以便于随时获取)
> - 网络通信模块(实现与客户端的网络通信)
> - 业务处理模块(上传，列表，下载(断点续传))
> - 热点管理模块(对长时间没访问的文件进行压缩存储)

## 2.客户端功能和模块细分

### 2.1客户端功能的细分

> - 指定文件夹中的文件检测(获取文件夹中有什么文件)
> - 判断指定的文件是否需要备份(新增，已经备份的被修改了，上次上传后又修改过，但是已经间隔3秒钟没有被修改了）
> - 将需要备份的文件上传到备份服务器

### 2.2客户端模块的划分

> - 数据管理模块(备份的文件信息)
> - 文件检测模块(监控指定的文件夹)
> - 文件备份模块(上传需要备份的文件数据)

## 3.环境搭建

### 3.1升级gcc和g++

```cpp
# 安装SCL存储库
sudo yum install centos-release-scl-rh centos-release-scl

# 安装DevToolSet-7的GCC和GCC++编译器
sudo yum install devtoolset-7-gcc devtoolset-7-gcc-c++

# 启用DevToolSet-7
source /opt/rh/devtoolset-7/enable

# 将DevToolSet-7添加到bashrc文件中，使其永久生效
echo "source /opt/rh/devtoolset-7/enable" >> ~/.bashrc
```

### 3.2安装jasoncpp库

```bash
# 安装EPEL存储库
sudo yum install epel-release

# 安装JSONCPP开发包
sudo yum install jsoncpp-devel

# 列出jsoncpp库的头文件目录
ls /usr/include/jsoncpp/json/
assertions.h config.h forwards.h reader.h version.h
autolink.h features.h json.h value.h writer.h
```

确认是否安装成功：

```bash
[mi@lavm-5wklnbmaja ~]$ ls /usr/lib64/libjson*
/usr/lib64/libjsoncpp.so    /usr/lib64/libjsoncpp.so.0.10.5  /usr/lib64/libjson-c.so.2.0.1  /usr/lib64/libjson.so.0.1.0
/usr/lib64/libjsoncpp.so.0  /usr/lib64/libjson-c.so.2        /usr/lib64/libjson.so.0
```

### 3.3环境搭建下载bundle数据压缩库

> [仓库链接](https://github.com/r-lyeh-archived/bundle)
>
> ```bash
> sudo yum install git
> git clone https://github.com/r-lyeh-archived/bundle.git
> ```

### 3.4环境搭建下载httplib库

> [仓库链接](https://github.com/yhirose/cpp-httplib)
>
> ```bash
> git clone https://github.com/yhirose/cpp-httplib.git
> ```

大概率情况下，由于网络问题可能clone不成功，所以我们采用直接在github上下载的方式，然后上传到服务器上即可。

