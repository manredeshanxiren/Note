# Linux进程概念(3)

## Ⅰ. 进程状态

> 阻塞：进程因为等待某种条件就绪，而导致的一种不推进的状态--进程卡住了--阻塞一定是在等待某种资源--为什么阻塞？进程要通过等待的方式，等具体的资源被别人是用完成之后的，再被自己使用。(用于理解：**pcb指针放在对应硬件的struct队列中进行等待**）
>
> 挂起：当内存中阻塞的进程过多时，这时候操作系统就会用自己的算法将进程的代码和数据交换到磁盘时，这个过程交挂起；

### Ⅰ. Ⅰ Linux的进程状态

> - 为了弄明白正在运行的进程是什么意思，我们需要知道进程的不同状态。一个进程可以有几个状态（在 Linux内核里，进程有时候也叫做任务）
>
>   状态在kernel源代码的定义：
>
>   ```livescript
>   /*
>   * The task state array is a strange "bitmap" of
>   * reasons to sleep. Thus "running" is zero, and
>   * you can test for combinations of others with
>   * simple bit tests.
>   */
>   static const char * const task_state_array[] = {
>   "R (running)", /* 0 */
>   "S (sleeping)", /* 1 */
>   "D (disk sleep)", /* 2 */
>   "T (stopped)", /* 4 */
>   "t (tracing stop)", /* 8 */
>   "X (dead)", /* 16 */
>   "Z (zombie)", /* 32 */
>   };
>   ```
>
>   - R运行状态（running）：它并不表示进程一直在运行，它表明进程要么是在运行要么就是在运行队列里。
>
>   - S睡眠状态（sleeping）：意味着进程在等待某事件（例如：向屏幕打印，等待键盘的输入等）完成（这里的睡眠也可以称为中断睡眠（interruptible sleep））
>
>   - D磁盘休眠状态（Disk Sleep）：**当进程的功能是向磁盘中读写数据的时候需要在内存中进行等待也就是（阻塞）当内存中阻塞的进程很多时这时候操作系统就会杀进程，但是对于这种进程操作系统会做特殊的处理将这个进程处理为D状态（这种状态无法被操作系统杀死）**，有时候也叫不可中断睡眠状态（uninterruptible sleep），在这个状态的进程通常会等待IO的结束。
>
>   - T停止状态（stopped）：可以通过发送 SIGSTOP 信号给进程来停止（T）进程。这个被暂停的进程可 以通过发送 SIGCONT 信号让进程继续运行。
>
>     这里我们需要用到`kill`命令：使用`kill -l`查看`kill`指令的参数：
>
>     ![image-20231102181837351](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231102181837351.png)
>
>     命令18和19分别就是继续和停止进程的指令；
>
>     那么我们如何使用这些指令呢？格式如下：
>
>     `kill -18 PID` 或者`kill -19 PID`
>
> 
>
>     ![image-20230414132243824](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230414132243824.png)
>
> 
>
>     - Z(zombie)-僵尸进程
>     
>       > - 僵死状态（Zombies）是一个比较特殊的状态。当进程退出并且父进程（使用wait()系统调用,后面讲） 没有读取到子进程退出的返回代码时就会产生僵死(尸)进程
>       > - 僵死进程会以终止状态保持在进程表中，并且会一直在等待父进程读取退出状态代码。
>       > - 所以，只要子进程退出，父进程还在运行，但父进程没有读取子进程状态，子进程进入Z状态
>
> 
>
>     ![image-20230414133230437](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230414133230437.png)
>
> 
>
>     僵尸进程
>     
>     > - 进程的退出状态必须被维持下去，因为他要告诉关心它的进程（父进程），你交给我的任务，我办的怎 么样了。可父进程如果一直不读取，那子进程就一直处于Z状态？是的！
>     > - 维护退出状态本身就是要用数据维护，也属于进程基本信息，所以保存在task_struct(PCB)中，换句话 说，Z状态一直不退出，PCB一直都要维护？是的！
>     > - 那一个父进程创建了很多子进程，就是不回收，是不是就会造成内存资源的浪费？是的！因为数据结构 对象本身就要占用内存，想想C中定义一个结构体变量（对象），是要在内存的某个位置进行开辟空 间！
>     > - 内存泄漏?是的！

