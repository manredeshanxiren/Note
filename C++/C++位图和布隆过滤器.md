# C++位图和布隆过滤器

## Ⅰ. 位图

### Ⅰ. Ⅰ 位图的概念

> - 位图的概念
>
>   ==所谓位图，就是用每一位来存放某种状态，适用于海量数据，数据无重复的场景。通常是用 来判断某个数据存不存在的。==
>
> 面试题：
>
> 给40亿个不重复的无符号整数，没排过序。给一个无符号整数，如何快速判断一个数是否在 这40亿个数中。【腾讯】
>
> 要求：
>
> ​	1.遍历，时间复杂度O(N)
>
> ​	2.排序(O($N log N$)), 二分查找O($logN$)
>
> ​	3.位图来解决
>
> 数据是否在给定的整形数据中，结果是在或者不在，刚好是两种状态，那么可以使用一 个二进制比特位来代表数据是否存在的信息，如果二进制比特位为1，代表存在，为0 代表不存在。比如：
>
> ![image-20230923162854557](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20230923162854557.png)

### Ⅰ. Ⅱ位图的简易实现

> ```cpp
> 	template<size_t N>
> 	class bitset
> 	{
>         //构造函数
> 		bitset()
> 		{
> 			_bit = new vector<char>(N / 8 + 1);
> 			_bit_count = 
> 		}
> 		//将x位反置位
> 		void reset(size_t x)
> 		{
> 			size_t i = x / 8;
> 			size_t j = x % 8;
> 			_bit[i] &= ~(1 << j);
> 		}
> 		//将x置位
> 		void set(size_t x)
> 		{
> 			size_t i = x / 8; // 计算x在第i个char内
> 			size_t j = x % 8;// 计算x在char的第j个位置
> 			_bit[i] |= (1 << j);
> 		}
> 		//查看x是否存在
> 		bool test(size_t x)
> 		{
> 			size_t i = x / 8;
> 			size_t j = x % 8;
> 			return _bit[i] & (1 << j);
> 		}
> 
> 	private:
> 		vector<char> _bit;
> 	};
> ```

### Ⅰ. Ⅲ位图的应用

> 1. 给定100亿个整数，设计算法找到只出现一次的整数？
>
>    ```
>    	template<size_t N>
>    	class twobit
>    	{
>    	public:
>    		void set(int x)
>    		{
>    			//00
>    			if (set1.test(x) == false && set2.test(x) == false)
>    			{
>    				set2.set(x);
>    			}
>    			//01
>    			else if (set1.test(x) == false && set2.test(x) == true)
>    			{
>    				set2.reset(x);
>    				set1.set(x);
>    			}
>    		}
>    		void Print()
>    		{
>    			for (int i = 0; i < N; ++i)
>    			{
>    				if (set1.test(i) == false && set2.test(i) == true)
>    					cout << i << " ";
>    			}
>    		}
>    
>    	public:
>    		bitset<N> set1;
>    		bitset<N> set2;
>    	};
>    
>    	void twobittest()
>    	{
>    		int a[] = { 3, 45,12, 12, 43,55,12,67,98,67,35,86 };
>    		twobit<100> set;
>    		for (auto& i : a)
>    		{
>    			set.set(i);
>    		}
>    		set.Print();
>    	}
>    ```
>
> 2. 给两个文件，分别有100亿个整数，我们只有1G内存，如何找到两个文件交集？
>
>    ①将第一个文件的所有整数存入到一个位图中。
>
>    然后遍历第二个文件查看是否在位图中？
>
>    如果在位图中，将位图中的这个整数`reset`，并记录当前的整数
>
>    ②将两个文件的所有整数，分别存储到两个位图中
>
>    再将两个位图按位与运算即可得到结果
>
> 3. 位图应用变形：1个文件有100亿个int，1G内存，设计算法找到出现次数不超过2次的所有整数
>
>    对1题进行改造
>
>    ```cpp
>    template<size_t N>
>    	class twobit
>    	{
>    	public:
>    		void set(int x)
>    		{
>    			//00
>    			if (set1.test(x) == false && set2.test(x) == false)
>    			{
>    				set2.set(x);
>    			}
>    			//01
>    			else if (set1.test(x) == false && set2.test(x) == true)
>    			{
>    				set2.reset(x);
>    				set1.set(x);
>    			}
>    			//10
>    			else if (set1.test(x) == true && set2.test(x) == false)
>    			{
>    				set2.set(x);
>    			}
>    		}
>    		void Print()
>    		{
>    			for (int i = 0; i < N; ++i)
>    			{
>    				if ((set1.test(i) == true && set2.test(i) == false) || (set1.test(i) == false && set2.test(i) == true))
>    					cout << i << " ";
>    			}
>    		}
>          
>    	public:
>    		bitset<N> set1;
>    		bitset<N> set2;
>    	};
>          
>    	void twobittest()
>    	{
>    		int a[] = { 3, 45,12, 12, 43,55,12,67,98,67,35,86 };
>    		twobit<100> set;
>    		for (auto& i : a)
>    		{
>    			set.set(i);
>    		}
>    		set.Print();
>    	}
>    ```

位图的优点：

速度快，占用空间小

位图的缺点：

只能映射整型，浮点数，string不能映射

因此我们引入布隆过滤器

## Ⅱ.布隆过滤器

### Ⅱ. Ⅰ 布隆过滤器的提出

> 我们在使用新闻客户端看新闻时，它会给我们不停地推荐新的内容，它每次推荐时要去重，去掉 那些已经看过的内容。问题来了，新闻客户端推荐系统如何实现推送去重的？ 用服务器记录了用 户看过的所有历史记录，当推荐系统推荐新闻时会从每个用户的历史记录里进行筛选，过滤掉那 些已经存在的记录。 如何快速查找呢？
>
> 1.用哈希表存储用户记录，缺点：浪费空间
>
> 2.用位图存储用户记录，缺点：位图一般只能处理整形，如果内容编号是字符串，就无法处理了。
>
> 3.将哈希与位图结合，即布隆过滤器

### Ⅱ. Ⅱ布隆过滤器的概念

> 布隆过滤器是由布隆（Burton Howard Bloom）在1970年提出的 一种紧凑型的、比较巧妙的概 率型数据结构，特点是高效地插入和查询，可以用来告诉你 “某样东西一定不存在或者可能存 在”，它是用多个哈希函数，将一个数据映射到位图结构中。此种方式不仅可以提升查询效率，也 可以节省大量的内存空间。
>
> ![image-20230923180148387](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20230923180148387.png)
>
> 布隆过滤器的详细介绍：https://zhuanlan.zhihu.com/p/43263751/

### Ⅱ. Ⅲ布隆过滤器的模拟实现

> ```cpp
> struct BKDRHash
> 	{
> 		size_t operator()(const string& s)
> 		{
> 			// BKDR
> 			size_t value = 0;
> 			for (auto ch : s)
> 			{
> 				value *= 31;
> 				value += ch;
> 			}
> 			return value;
> 		}
> 	};
> 
> 
> 	struct APHash
> 	{
> 		size_t operator()(const string& s)
> 		{
> 			size_t hash = 0;
> 			for (long i = 0; i < s.size(); i++)
> 			{
> 				if ((i & 1) == 0)
> 				{
> 					hash ^= ((hash << 7) ^ s[i] ^ (hash >> 3));
> 				}
> 				else
> 				{
> 					hash ^= (~((hash << 11) ^ s[i] ^ (hash >> 5)));
> 				}
> 			}
> 			return hash;
> 		}
> 	};
> 	struct DJBHash
> 	{
> 		size_t operator()(const string& s)
> 		{
> 			size_t hash = 5381;
> 			for (auto ch : s)
> 			{
> 				hash += (hash << 5) + ch;
> 			}
> 			return hash;
> 		}
> 	};
> 
> 
> 
> 	template<size_t N, class K,
> 	class Hash1 = BKDRHash,
> 	class Hash2 = APHash,
> 	class Hash3 = DJBHash>
> 	class BloomFilter
> 	{
>         //向布隆过滤器中插入元素
> 		void set(const K& key)
> 		{
> 			size_t hash1 = Hash1()(key) % N;
> 			_bs.set(hash1);
> 			size_t hash2 = Hash2()(key) % N;
> 			_bs.set(hash2);
> 			size_t hash3 = Hash3()(key) % N;
> 			_bs.set(hash3);
> 		}
> 
> 		//检查是否存在于布隆过滤器中
> 		bool test(const K& key)
> 		{
> 			size_t hash1 = Hash1()(key) % N;
> 			if (!_bs.test(hash1))
> 			{
> 				return false;
> 			}
> 			size_t hash2 = Hash2()(key) % N;
> 			if (!_bs.test(hash2))
> 			{
> 				return false;
> 			}
> 			size_t hash3 = Hash3()(key) % N;
> 			if (!_bs.test(hash3))
> 			{
> 				return false;
> 			}
> 			//一个不确定的true
> 			return true;
> 		}
> 
> 	private: 
> 		bitset<N> _bs;
> 	};
> 
> }
> ```
>

### Ⅱ. Ⅳ 布隆过滤器的应用

> **1.给两个文件，分别有100亿个query，我们只有1G内存，如何找到两个文件交集？分别给出 精确算法和近似算法**
>
> 首先我们分析：
>
> 对于100亿个query(query可以认为是一个字符串)，我们估计每个query平均的大小为50byte，那么100亿个query就是500亿byte，也就是大约500G。
>
> 对于这么庞大的数据量我们肯定不能全部放到内存中去计算，所以我们要想办法将他们分割
>
> ![image-20231004195344284](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231004195344284.png)
>
> 但是如上的操作也会产生问题：
>
> 因为不是平均的分割，可能出现冲突多，每个Ai，Bi文件过大。----假设两个都是4-5G的情况
>
> 出现这两种的情况的两个原因：
>
> ①单个文件中有大量的重复query
>
> ②单个文件中有大量的不重复query
>
> 那么我们具体如何操作呢？
>
> 直接用unordered_set/set，依次读取query，每次插入到set中。
>
> ①如果读取整个小文件中的query，都可以成功插入set，那么说明情况1；
>
> ②如果读取的整个小文件query，插入过程中出现了抛异常的情况，则是情况2。这时候我们可以选择换其他的哈希函数，再次分割，在求交集。
>
> **==说明：==**set插入key的时候，set中存在的话就会返回`false`,如果内存不够的话，就会抛`bad_alloc`的异常，剩下的都是成功插入。
>
> **2.如何扩展BloomFilter使得它支持删除元素的操作？**
>
> 我们可以采用多个位去统计出现的次数。
>
> **3.哈希切割：给一个超过100G大小的log file, log中存着IP地址, 设计算法找到出现次数最多的IP地址？ **
>
> **与上题条件相同，如何找到top K的IP？如何直接用Linux系统命令实现？**
>
> ![image-20231004212225081](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231004212225081.png)
>
> 对于第二问我们可以建立一个有k个元素的小堆，将文件中的IP地址向堆中插入我们就可以得到topK。



