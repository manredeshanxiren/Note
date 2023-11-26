# linux 管道(2)

## 1.管道程序的再优化

### 1.1void ctrlprocess函数

> 分三步：
>
> - 确定任务
> - 确定执行任务的子进程.轮询式的
> - 执行任务

```c++
void ctrlprocess(const vector<EndPoint>& end_points)
{
    // 2.写成自动化，也可以搞成交互式的
    int cnt = 0;
    while (true)
    {
        //1.确定任务
        int command = ShowBoard();
        if(command == 3) break;
        if(command < 0 || command > 2) continue;

        //2.确定执行任务的子进程.轮询式的
        int child = cnt++;
        cnt %= end_points.size();
        cout << "选择了进程：" << end_points[child].name() <<"| 处理任务：" << command << endl;

        //3.执行任务
        write(end_points[child]._write_fd, &command, sizeof(command));

        sleep(1);
    }
}
```

### 1.2EndPoint类

> - 增加static成员number：用于统计子进程个数
> - 增加string processname：用于存储进程的名字
> - 增加name函数：用于打印子进程的名字

```c++
// 先描述
class EndPoint
{
private:
    static int number;
public:
    pid_t _child;  // 子进程pid
    int _write_fd; // 对应的文件描述符
    string processname;
public:
    // 构造
    EndPoint(int id, int fd)
        : _child(id), _write_fd(fd)
    {
        char namebuffer[64];
        snprintf(namebuffer, sizeof(namebuffer), "process-%d[%d:%d]", number++, id, fd);
        processname = namebuffer;
    }

    string name() const 
    {
        return processname;
    }

    // 析构
    ~EndPoint()
    {
    }
};
```

### 1.3RecailmTask函数

> 用于子进程的回收：
>
> - ①关闭写描述符：根据前面讲的父进程关闭了管道对应的写描述符之后，子进程也就退出了
> - ②回收子进程：waitpid对应的函数进行回收！

方案一：两种操作分开执行

```c++
void RecailmTask(const vector<EndPoint>& end_points)
{
    //1.关闭写描述符
    for(int i = 0; i < end_points.size(); ++i) close(end_points[i]._write_fd);
    cout << "父进程让所有的进程退出了" << endl;

    sleep(5);
    //2.回收子进程
    for(int i = 0; i < end_points.size(); ++i) waitpid(end_points[i]._child, nullptr, 0);
    sleep(5);
}
```

运行结果：

子进程在父进程的操控下，正常退出了

![image-20231126215109563](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231126215109563.png)

### 1.4子进程继承父进程文件描述符问题的解决

但是当我们把这两个过程合并的时候问题出现了：

然后就卡住了

```c++
void RecailmTask(const vector<EndPoint>& end_points)
{
    //1.关闭写描述符
    for(int i = 0; i < end_points.size(); ++i) 
    {
        close(end_points[i]._write_fd);
        waitpid(end_points[i]._child, nullptr, 0);
    }
    cout << "父进程让所有的进程退出了" << endl;

    sleep(5);
}
```

![image-20231126220145423](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231126220145423.png)

这是为什么呢？其实我们想一想，**父进程在创建子进程的时候子进程也会把父进程的文件描述符也会拷贝一份**

![image-20231126221535592](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231126221535592.png)

所以除了第一个子进程当我们父进程关闭对应的写端的时候子进程不会关闭，原因是其他的子进程也继承了对应的写端的文件描述符。所以写端并没有完全关闭，所以这时候父进程去等待回收子进程的时候就会一直在等待，造成了程序卡住的状态！那么我们怎么解决呢？

方案一：反着顺序关闭写端

```c++
void RecailmTask(const vector<EndPoint>& end_points)
{

    //1.关闭写描述符
    for(int end = end_points.size() - 1; end >= 0; --end) 
    {
        close(end_points[end]._write_fd);

        waitpid(end_points[end]._child, nullptr, 0);
    }
    cout << "父进程让所有的进程退出了" << endl;
    sleep(5);
}
```

![image-20231126222855900](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231126222855900.png)

可是我们这种办法只是解决了表象，我们没有解决的根本的情况，我们只是让程序可以正常的关闭，但是子进程的那种继承父进程管道的文件描述符的问题还是没有解决，并且这也是有一定不安全的情况在里面的，因为管道不是一对一的情况了，变成了多对一的情况，可能会造成其他的子进程向其他管道中错误写入的问题：

所以我们我解决这个问题，这个问题我们应该在创建子进程管道的时候就解决好！

思路：我们每创建一个子进程，把其对应的文件描述符存储在一个vector内部，然后再创建第二个子进程的时候，遍历vector的时候，将他们依次关掉即可！

```c++
void creatProcesses(vector<EndPoint> &end_points)
{
    //用于存储文件描述符
    vector<int>fds;
    // 1.先进行构建控制结构,父进程写，子进程读
    for (int i = 0; i < gnum; ++i)
    {
        // 1.1创建管道
        int pipefd[2] = {0};
        int ret = pipe(pipefd);
        assert(ret == 0); // 0正常 -1不正常
        (void)ret;

        // 1.2创建进程
        pid_t id = fork();
        assert(id != -1);

        if (id == 0)
        {

            //关闭不必要的描述符
            for(auto& fd : fds) close(fd);

            // 子进程
            // 1.3关闭不要的fd
            close(pipefd[1]);
            // 我们期望，所有的子进程读取“指令”的时候，都从标准输入读取

            // 1.3.1所以我们进行输入重定向
            dup2(pipefd[0], 0);

            // 1.3.2子进程开始等待获取命令
            WaitCommend();

            close(pipefd[0]);
            exit(0);
        }

        // 父进程
        // 1.3关闭不要的fd
        close(pipefd[0]);

        // 1.4将新的子进程和他的管道写端构建对象。
        end_points.push_back(EndPoint(id, pipefd[1]));

        fds.push_back(pipefd[1]);
    }
}
```

运行结果：

![image-20231126224519572](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231126224519572.png)

---

