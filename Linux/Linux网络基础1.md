# Linux网络基础1

## 1.计算机网络背景

### 1.1网络发展

- 独立模式：计算机之间相互独立;  

![image-20240228214414575](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240228214414575.png)

- 网络互联：多台计算机连接在一起, 完成数据共享;  

![image-20240228214527131](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240228214527131.png)

- 局域网LAN：计算机数量更多了, 通过交换机和路由器连接在一起;  

![image-20240228214731545](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240228214731545.png)

- 广域网WAN:将远隔千里的计算机都连在一起 ;

![image-20240228214811433](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240228214811433.png)

## 2.认识协议

"协议"就是一种约定。

计算机之间的传输媒介是光信号和电信号. 通过 "频率" 和 "强弱" 来表示 0 和 1 这样的信息. 要想传递各种不同的信息, 就需要约定好双方的数据格式 。

> - 计算机生产厂商有很多;  
> - 计算机操作系统, 也有很多;  
> - 计算机网络硬件设备, 还是有很多 ；
> - 如何让这些不同厂商之间生产的计算机能够相互顺畅的通信? 就需要有人站出来, 约定一个共同的标准,大家都来遵守, 这就是 **网络协议**;  

### 2.1网络协议初识

![image-20240228215453606](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240228215453606.png)