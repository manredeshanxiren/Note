# Linux程序的地址空间

## 1.一个测试代码

> 我们先运行一个测试代码：
>
> ```c
> #include<stdio.h>
> #include<unistd.h>
> #include<assert.h>
> 
> 
> 
> int g_val = 100; //全局变量
> 
> int main()
> {
>   
>   pid_t id = fork();
> 
>   assert(id >= 0);
> 
>   if(id == 0)
>   {
>     //child
>     while(1)
>     {
>       //此处反斜杠是为了让代码可以换行
>       printf("我是子进程，我的PID是:%d,我的PPID是:%d,g_val:%d, &g_val:%p\n",\
>           getpid(), getppid(), g_val, &g_val);                                                                      
>       sleep(1);
>       g_val++;
>     }
>   }
>   else
>  {
>     while(1)
>     {
>       //父进程
>       printf("我是父进程，我的PID是:%d,我的PPID是:%d,g_val:%d, &g_val:%p\n",\
>           getpid(), getppid(), g_val, &g_val);
>       sleep(1);
>     }
>   }
>   return 0;
> }
> 
> ```
> ### 运行的结果：
>
> ![image-20231106170746600](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231106170746600.png)

### 1.1可以得到的结论

我们发现竟然指向相同地址的变量，却出现了不同值的情况，这其实是因为发生了**<u>写时拷贝</u>**，并且我们佐证了，子进程可以对全局变量进行修改并且不影响父进程全局变量的值，从而证明了进程具有独立性。

同时我们也可以看出这里的地址不是物理地址，因为假设是物理地址，我们不可能在同一块物理地址上，取到不同的值，所以我们认为这是一个**虚拟地址**or**线性地址**。

## 2.引入地址空间

### 2.1小故事

> 我们用一个大富翁和私生子的故事来引入地址空间的概念：
>
> 大富翁有十亿美金，但是他有四个私生子(A , B, C , D);
>
> ![image-20231106223351434](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231106223351434.png)
>
> A：做生意的    |    B：卖化妆品    |     C：哈弗读书     |     D：高中辍学的混混
>
> 并且ABCD并不知道彼此的存在，并且他们都坚信自己最终能够继承大富翁的遗产；
>
> 某一天大富翁对着A说：你好好做你的生意，将来等我去世的时候我的十亿家产全都是你的。
>
> 对B说：你的化妆品卖的挺好，你听老爹的话，等老爹到时候西去的时候，我的家产都是你的。
>
> 同样对C和D都是一样的画饼。
>
> 所以现在ABCD每个人脑子里都有一张饼，就是老爹的十亿。:laughing:
>
> ....
>
> 有一天A来找到大富翁，告诉他说自己需要五万美金，做生意需要买一块手表，会见自己和合作伙伴，但是自己的生意目前周转不开，大富翁一想，才五万美金，大手一挥直接给A了。
>
> C联系大富翁，并给他说：自己在学校念书，没有生活费了，想问老爹要1000美金的生活费。老爹一听，小钱大手一挥又给了。**（以上两个是进程向操作系统申请较小的内存空间，操作系统当然允许）**
>
> 有一天D找到大富翁说:我在社会上遇到一点事，需要一些钱摆平一下，你给我5000万美金吧。老爹一听，说：一边去，我这么多钱也不能这么花。D骂骂咧咧的走了。**(进程向操作系统申请很大的内存空间，操作系统拒绝了)**
>
> ..这是ABCD也都会认为自己最终如果想要那十亿美金**(饼)**，他们都还是会获得的。
>
> 但其实如果ABCD每个人向大富翁索要几千几万，大富翁都会给予。但是如果向大富翁索要很多，几千万几亿的情况下，大富翁是完全可以直接拒绝的。
>
> 这里我们引入第一个概念：
>
> **进程的地址空间**：就是大富翁给每个私生子画的饼。
>
> 那么最后揭晓：十亿美金相当于**内存**，大富翁相当于**操作系统**
>
> ----
>
> 那么我们想想大富翁有没有必要将自己的“饼”管理起来呢？
>
> **答案是有必要的<u>先描述在组织</u>**，因为如果大富翁的儿子多了，并且他画的饼多了之后，他也会忘记的。

### 2.2 代码区，数据区，堆区，等等这些区域该如何理解

> 我们用一个小故事引入：
>
> 时间来到小学时代，有一对同桌，小花和小胖，两个人**共用一张大桌子。(内存空间)**然而小胖，不爱干净，男孩子很爱运动，所以身上老有一股汗味。小花就不满意了，二话不说先把小胖揍了一顿，告诉他:回家之后好好洗一洗，回去告诉你爸爸妈妈，我们上幼儿园要讲卫生。第二点，你从今天开始不能再欺负我了，要不然我再揍你一顿。
>
> <img src="https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231107135225278.png" alt="image-20231107135225278" style="zoom:50%;" />
>
> 小花告诉小胖：我们以中间这根线为界谁都不许越界，不能把你的东西放到我这边来，也不能把你的胳膊放到我这边来。
>
> **小花画这条线的本质：叫做区域划分！如下这样：**
>
> ```c
> struct area
> {
>     int start;
>     int end;
> }
> struct area xiaohua_area = {1, 50};
> struct area xiaopang_area = {50, 100};
> ```
>
> **地址空间本质就是线性结构的**
>
>  <img src="https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231107145145203.png" alt="image-20231107145145203" style="zoom:50%;" />
>
> 不管是32位还是64位机器，我们并不需要记录地址空间是多少GB的，原因是：不管是多少位的机器，在操作系统刚刚开始加载的时候就确定好了。 
>
> <font color='red'>**那么有了之前的铺垫，我们就可以地址空间是怎么在mm_struct中划分的;**</font>
>
> ```c
> struct mm_struct ---- 4GB
> {
>     long code_start; //代码区的起始
>     long code_end; //代码区的结束
>     long init_start;
>     long init_end;
>     ....
>     long brk_start; //堆区的起始
>     long brk_end; //堆区的结束
>     long stack_start; //栈区的起始
>     long stack_end; //栈区的结束
> }
> ```
>
> 那么如果限定了区域，那么区域之间的数据是什么？
>
> 例如[1000,2000],1000和2000被称作我们的地址，那么他们之间的区域就被称作<font color='cornflowerblue'>**虚拟地址or线性地址**</font>
>
> 那么我们故事在继续...
>
> 小花和小胖画好了三八线之后，一段时间，两个人都很受规矩，但是小胖却很调皮，小胖总是将自己的胳膊放到三八线左侧，或者将自己的书包和都扔到小花的桌子上，这强烈的引起了小花的不满，小花就将小胖揍了一顿，然后更改了三八线的位置...
>
> <img src="https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231107151152918.png" alt="image-20231107151152918" style="zoom:50%;" />
>
> 那么对于小花来说，叫做区域扩大。对于小胖来说叫做区域缩小；
>
> 体现在`mm_struct`中呢就是：
>
> ```c
> xiaohua_area.end = 75;
> xiaopang_area.start = 75;
> ```
>
> <font color='cornflowerblue'>**那么对应到我们的地址空间上，对于我们的栈区，堆区等的区域扩大和缩小也是一样的，都是更改我们的`start`和`end`.**</font>

## 3. 地址空间是什么？为什么？怎么办？

> 数据和代码真正是存储在内存中。
>
> **是什么？**
>
> 虚拟地址空间是一种在操作系统内部为进程创建出来的具体的数据结构对象，让进程以同意的视角来查看内存。
>
> ![image-20231107153416685](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231107153416685.png)
> 
>
> 虚拟地址会经过也变转化为物理地址，我们平时使用的地址都是虚拟地址，当我们有了虚拟地址，我们就可以通过页表找到物理地址中对应的位置，从而将物理地址中存储的内容放到我们的CPU中。
>
> 接下来我们用一幅图来解释最开始的父进程和子进程，查看相同地址的内存，却看到不同值的问题。
>
> **首先操作系统为每一个进程维护一张页表，用来存储虚拟地址和物理地址的映射关系。**
>
> ![image-20231107155544493](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231107155544493.png)
>
> 最开始的时候父子进程的虚拟地址都是指向最开始的`g_val`的位置，但是当子进程尝试修改`g_val`的时候，这时候操作系统会在物理地址的另一个位置拷贝一份`g_val`的值，然后修改，并且将这个物理地址存储到子进程页表刚刚指向原物理地址的位置。
>
> **并且这里也是体现了进程的独立性。**
>
> 同时我们这里也可以解释最开始的之前的代码`fork()`函数貌似有两个返回值的问题：
>
> 首先`fork`在返回的时候，父子进程都有了，return两次，id是不是也是pid_t类型定义的变量呢？那么返回的本质就是写入，父子进程谁先写入，就发生了写时拷贝。造成了一个变量，有两个值的情况。
>
> ```c
> #include<stdio.h>
> #include<unistd.h>
> #include<sys/types.h>
> int main()
> {     
> 	pid_t ret = fork();
> 	if(ret == 0)
> 	{ 
> 		//子进程
> 		printf("我是子进程，我的pid是：%d，我的父进程是：%d",getpid(),getppid()); 
> 		sleep(1);        
> 	}
> 	else if(ret > 0)
> 	{    
> 		//父进程
> 		while(1)
> 		{ 
> 			printf("我是父进程，我的pid是：%d，我的父进程是：%d"，getpid(),getppid());
> 			sleep(2);   
> 		}                          
> 	}                                                                                    
> 	return 0;                                                                                  
> } 
> ```

### 3.1为什么要有地址空间

> 如果没有地址空间，我们的进程的`pcb`只能直接指向物理内存中的该进程相关的数据和代码。
>
> 这样就会产生很多不安全的问题，以及效率问题。
>
> ①防止地址随意访问，保护物理内存和其他进程
>
> ②将进程管理和内存管理**解耦合**
>
> ③可以让进程以统一的视角，看待自己的代码和数据

### 3.2 malloc的本质

> 当我们向操作系统申请内存，操作系统立马给你，还是需要的时候再给你呢？
>
> **那么毫无疑问肯定是我们需要的时候才会给我们。**
>
> ①操作一同不会允许任何的浪费和不高效
>
> ②申请内存 != 立马使用
>
> ③在你申请成功之后，和你使用之前，有很小一段的时间窗口，----这个空间没有被正常使用，但是别人也用不了，---闲置状态。
>
> 那么malloc的时候申请空间，我们操作系统只会再地址空间上对应的堆区进行申请空间，那么这里就是我们之前提到的，操作系统会为我们的堆区进行区域扩大，也就是去操作我们的`mm_struct`中的`brk_end`和`brk_start`,然后去页表中存储，但是不会再物理地址上去帮我们申请。相当于做了一半的映射，这时候直到当我们的进程需要用到这片地址的时候，操作系统才会帮我们去完成页表和物理地址的映射。 
>
> 当操作系统检测到我们需要内存的时候才给我们申请，或者检测到我们的代码和数据不在内存，当我们需要的时候给我们换入，这样的操作叫做**缺页中断**。
>
> 总结一句就是malloc在申请空间的时候，我们的操作系统不会立马分配空间，而是在我们需要使用的时候才给我们分配，这就是malloc的本质。
>
> 并且正是因为有了地址空间的存在，我们的进程也不关心，它的数据和代码存储在物理地址的任意位置。
>
> **进程管理：**操作系统管理进程和地址空间的过程，叫做进程管理。
>
> **内存管理:**  操作系统管理页表和物理内存之间的映射的过程，叫做内存管理。
>
> 并且操作系统将进程和物理内存之间分成**进程管理**和**内存管理**的过程，叫做**解耦合。**

### 3.3 重新理解地址空间

> 首先我们想一想我们的程序在被编译的时候，没有被加载到内存，请问，我们的程序内部有没有地址呢？
>
> 答案肯定是有的，**源代码被编译的时候，就是按照虚拟地址空间的方式进行对代码和数据早就已经编好了对应的编址。**
>
> 我们不能认为虚拟地址这样的策略只会影响操作系统，**还要让我们的编译器也要遵守这样的规则。**(Linux系统中的可执行文件的格式是ELF格式)
>
> **进程的代码和数据需要一直在内存中吗？**
>
> 答案是不一定的，就算是我们要运行一个进程，我们操作系统也不会一次将这个进程所对应的代码和数据加载到内存中。
>
> 操作系统一般是边加载边执行，这也是地址空间的意义。



