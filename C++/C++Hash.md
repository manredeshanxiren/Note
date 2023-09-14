# C++Hash

## Ⅰ.unordered系列关联式容器

> 在C++98中，STL提供了底层为红黑树结构的一系列关联式容器，在查询时效率可达到$log_2  N$，即最差情况下需要比较红黑树的高度次，当树中的节点非常多时，查询效率也不理想。最好 的查询是，进行很少的比较次数就能够将元素找到，因此在C++11中，STL又提供了4个 unordered系列的关联式容器，这四个容器与红黑树结构的关联式容器使用方式基本类似，只是其底层结构不同，本文中只对[unordered_map](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/)和[unordered_set](https://legacy.cplusplus.com/reference/unordered_set/unordered_set/)进行介绍， [unordered_multimap](https://legacy.cplusplus.com/reference/unordered_map/unordered_multimap/)和[unordered_multiset](https://legacy.cplusplus.com/reference/unordered_set/unordered_multiset/)可查看文档介绍。
>
> 因为unordered_map和unordered_set的底层实现采用的时hash_table的方式去实现的，所以他们的查找效率都非常的高(其他的方面效率提升不明显)

### Ⅰ.Ⅰunordered_map

#### Ⅰ.Ⅰ.Ⅰunordered_map的文档介绍

> [unordered_map在线文档说明](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/)
>
> ①unordered_map是存储键值对的关联式容器，其允许通过keys快速的索引到与 其对应的value。
>
> ②在unordered_map中，键值通常用于唯一地标识元素，而映射值是一个对象，其内容与此键关联。键和映射值的类型可能不同。
>
> ③在内部,unordered_map没有对按照任何特定的顺序排序, 为了能在常数范围内 找到key所对应的value，unordered_map将相同哈希值的键值对放在相同的桶中。
>
> ④unordered_map容器通过key访问单个元素要比map快，但它通常在遍历元素子集的范围迭代方面效率较低。
>
> ⑤unordered_map实现了直接访问操作符(operator[])，它允许使用key作为参数直接访问 value。
>
> ⑥它的迭代器至少是前向迭代器。

#### Ⅰ. Ⅰ. Ⅱ unordered_map的接口介绍

> ①unordered_map的构造
>
> |                           函数声明                           |            函数功能             |
> | :----------------------------------------------------------: | :-----------------------------: |
> | [unordered_map](https://cplusplus.com/reference/unordered_map/unordered_map/unordered_map/) | 构造不同格式的unordered_map对象 |
>
> ②unordered_map的容量
>
> |                           函数声明                           |                 函数功能                  |
> | :----------------------------------------------------------: | :---------------------------------------: |
> | [unordered_map::empty](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/empty/) |         检测unordered_map是否为空         |
> | [unordered_map::size](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/size/) |     检测unordered_map中有效元素的个数     |
> | [unordered_map::max_size](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/max_size/) | 返回unordered_map容器可以容纳的最大元素数 |
>
> ③unordered_map的迭代器
>
> |                           函数声明                           |                       函数功能                       |
> | :----------------------------------------------------------: | :--------------------------------------------------: |
> | [unordered_map::begin](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/begin/) |         返回unordered_map第一个元素的迭代器          |
> | [unordered_map::end](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/end/) |   返回unordered_map最后一个元素下一个位置的迭代器    |
> | [unordered_map::cbegin](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/cbegin/) |       返回unordered_map第一个元素的const迭代器       |
> | [unordered_map::cend](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/cend/) | 返回unordered_map最后一个元素下一个位置的const迭代器 |
>
> ④unordered_map的元素访问
>
> |                           函数声明                           |               函数功能               |
> | :----------------------------------------------------------: | :----------------------------------: |
> | [unordered_map::operator[]](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/operator[]/) | 返回与key对应的value，没有一个默认值 |
> | [unordered_map::at](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/at/) | 返回与key对应的value，没有一个默认值 |
>
> 注意：该函数(`[]`)中实际调用哈希桶的插入操作(map和set的底层实现也是如此)，用参数key与V()构造一个默认值往底层哈希桶中插入，如果key不在哈希桶中，插入成功，返回V()，插入失败，说明key已经在哈希桶中， 将key对应的value返回。
>
> ⑤unordered_map的查询
>
> |                           函数声明                           |               函数功能                |
> | :----------------------------------------------------------: | :-----------------------------------: |
> | [unordered_map::find](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/find/) |        返回key在哈希桶中的位置        |
> | [unordered_map::count](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/count/) | 返回哈希桶中关键码为key的键值对的个数 |
> | [unordered_map::equal_range](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/equal_range/) |  返回包含容器中所有元素的区域的边界   |
>
> 注意：因为unordered_map中key是不能重复的，因此count函数的返回值最大为1
>
> ⑥unordered_map的修改操作
>
> |                           函数声明                           |                 函数功能                 |
> | :----------------------------------------------------------: | :--------------------------------------: |
> | [unordered_map::insert](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/insert/) |            向容器中插入键值对            |
> | [unordered_map::erase](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/erase/) |            删除容器中的键值对            |
> | [unordered_map::clear](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/clear/) |          清空容器中有效元素个数          |
> | [unordered_map::swap](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/swap/) |         交换**两个容器**中的元素         |
> | [unordered_map::emplace](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/emplace/) | 构造并向容器中插入键值对(效率比insert高) |
> | [unordered_map::emplace_hint](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/emplace_hint/) | 构造并向容器中插入键值对(效率比insert高) |
>
> ⑦ unordered_map的桶操作
>
> |                           函数声明                           |          函数功能           |
> | :----------------------------------------------------------: | :-------------------------: |
> | [unordered_map::bucket_count](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/bucket_count/) |   返回哈希桶中桶的总个数    |
> | [unordered_map::max_bucket_count](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/max_bucket_count/) | 返回哈希桶中桶的最大总个数  |
> | [unordered_map::bucket_size](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/bucket_size/) | 返回n号桶中有效元素的总个数 |
> | [unordered_map::bucket](https://legacy.cplusplus.com/reference/unordered_map/unordered_map/bucket/) |    返回元素key所在的桶号    |

### Ⅰ. Ⅱ unordered_set

> unordered_set和unordered_map的接口极其类似
>
> 这里就不过多赘述具体可参看文档[unordered_set](https://legacy.cplusplus.com/reference/unordered_set/unordered_set/)

### Ⅰ. Ⅲ 一些相关的OJ题

> [349. 两个数组的交集 - 力扣（LeetCode）](https://leetcode.cn/problems/intersection-of-two-arrays/submissions/)
>
> [961. 在长度 2N 的数组中找出重复 N 次的元素 - 力扣（LeetCode）](https://leetcode.cn/problems/n-repeated-element-in-size-2n-array/submissions/)
>
> [[350. 两个数组的交集 II - 力扣（LeetCode）](https://leetcode.cn/problems/intersection-of-two-arrays-ii/)](https://leetcode.cn/problems/intersection-of-two-arrays/)
>
> [217. 存在重复元素 - 力扣（LeetCode）](https://leetcode.cn/problems/contains-duplicate/)
>
> [884. 两句话中的不常见单词 - 力扣（LeetCode）](https://leetcode.cn/problems/uncommon-words-from-two-sentences/)

## Ⅱ.底层结构

> unordered系列的关联式容器之所以效率比较高，是因为其底层使用了哈希结构。

### Ⅱ. Ⅰ哈希概念

> 顺序结构以及平衡树中，元素关键码与其存储位置之间没有对应的关系，因此在查找一个元素 时，必须要经过关键码的多次比较。顺序查找时间复杂度为O(N)，平衡树中为树的高度，即 O($log_2 N$)，搜索的效率取决于搜索过程中元素的比较次数。
> 理想的搜索方法：可以不经过任何比较，一次直接从表中得到要搜索的元素。 如果构造一种存储结构，通过某种函数(hashFunc)使元素的存储位置与它的关键码之间能够建立 一一映射的关系，那么在查找时通过该函数可以很快找到该元素。
>
> 当向该结构中：
>
> - 插入元素
>
>   根据待插入元素的关键码，以此函数计算出该元素的存储位置并按此位置进行存放
>
> - 搜索元素
>
>   对元素的关键码进行同样的计算，把求得的函数值当做元素的存储位置，在结构中按此位置 取元素比较，若关键码相等，则搜索成功
>
> 该方式即为哈希(散列)方法，哈希方法中使用的转换函数称为哈希(散列)函数，构造出来的结构称 为哈希表(Hash Table)(或者称散列表)
>
> 例如：数据集合{1，7，6，4，5，9}；
>
> 哈希函数设置为：`hash(key) = key % capacity;` `capacity`为存储元素底层空间总的大小。
>
> ![image-20230914165947879](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20230914165947879.png)
> 用该方法进行搜索不必进行多次关键码的比较，因此搜索的速度比较快 
>
> 问题：按照上述哈希方式，向集合中插入元素44，会出现什么问题？
>
> 会出现冲突的情况！，因为44计算下来应该放在4当前放的位置；

### Ⅱ. Ⅱ哈希冲突

> 对于两个数据元素的关键字$k_i$和 $k_j$(i != j)，有$k_i$ != $k_j$，但有：Hash($k_i$) ==  Hash($k_j$)，即：不同关键字通过相同哈希哈数计算出相同的哈希地址，该种现象称为哈希冲突或哈希碰撞。
>
> 把具有不同关键码而具有相同哈希地址的数据元素称为“同义词”。 
>
> 发生哈希冲突该如何处理呢？
>
> 引入哈希函数的概念来解决哈希冲突！

### Ⅱ. Ⅲ哈希函数

> 引起哈希冲突的一个原因可能是：哈希函数设计不够合理。 哈希函数设计原则：
>
> - 哈希函数的定义域必须包括需要存储的全部关键码，而如果散列表允许有m个地址时，其值 域必须在0到m-1之间
> - 哈希函数计算出来的地址能均匀分布在整个空间中
> - 哈希函数应该比较简单
>
> 常见哈希函数：
>
> - 直接定值法：
>
>   取关键字的某个线性函数为散列地址：Hash（Key）= A*Key + B 
>
>   优点：简单、均匀
>
>   缺点：需要事先知道关键字的分布情况 
>
>   使用场景：适合查找比较小且连续的情况
>
>   面试题：[387. 字符串中的第一个唯一字符 - 力扣（LeetCode）](https://leetcode.cn/problems/first-unique-character-in-a-string/)
>
> - 除留余数法--(常用)
>
>   设散列表中允许的**地址数为m，**取一个不大于m，但最**接近或者等于m的质数p作为除数**， 按照哈希函数：**Hash(key) = key% p(p<=m),**将关键码转换成哈希地址
>
> - 平方取中法--(了解)
>
>   假设关键字为1234，对它平方就是1522756，抽取中间的3位227作为哈希地址； 再比如关键字为4321，对它平方就是18671041，抽取中间的3位671(或710)作为哈希地址 
>
>   **平方取中法比较适合：不知道关键字的分布，而位数又不是很大的情况**
>
> - 折叠法--(了解)
>
>   折叠法是将关键字从左到右分割成位数相等的几部分(最后一部分位数可以短些)，然后将这 几部分叠加求和，并按散列表表长，取后几位作为散列地址。 
>
>   **折叠法适合事先不需要知道关键字的分布，适合关键字位数比较多的情况**
>
> - 随机数法--(了解)
>
>   选择一个随机函数，取关键字的随机函数值为它的哈希地址，即H(key) = random(key),其中 random为随机数函数。 
>
>   通常应用于关键字长度不等时采用此法
>
> -  数学分析法--(了解)
>
>   设有n个d位数，每一位可能有r种不同的符号，这r种不同的符号在各位上出现的频率不一定 相同，可能在某些位上分布比较均匀，每种符号出现的机会均等，在某些位上分布不均匀只 有某几种符号经常出现。可根据散列表的大小，选择其中各种符号分布均匀的若干位作为散列地址。例如：
>
>   <img src="https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20230914180314139.png" alt="image-20230914180314139" style="zoom:50%;" />
>   假设要存储某家公司员工登记表，如果用手机号作为关键字，那么极有可能前7位都是 相同的，那么我们可以选择后面的四位作为散列地址，如果这样的抽取工作还容易出现 冲突，还 可以对抽取出来的数字进行反转(如1234改成4321)、右环位移(如1234改成4123)、左环移 位、前两数与后两数叠加(如1234改成12+34=46)等方法。
>
>   **数字分析法通常适合处理关键字位数比较大的情况，如果事先知道关键字的分布且关键字的 若干位分布较均匀的情况**
>
> **注意：哈希函数设计的越精妙，产生哈希冲突的可能性就越低，但是无法避免哈希冲突**

### Ⅱ. Ⅳ 哈希冲突解决

> 解决哈希冲突两种常见的方法是：闭散列和开散列

### Ⅱ. Ⅳ. Ⅰ闭散列

> **闭散列：也叫开放定址法，当发生哈希冲突时，如果哈希表未被装满，说明在哈希表中必然还有 空位置，那么可以把key存放到冲突位置中的“下一个” 空位置中去。**那如何寻找下一个空位置 呢？
>
> **①线性探测**
>
> ​	比如2.1中的场景，现在需要插入元素44，先通过哈希函数计算哈希地址，hashAddr为4， 因此44理论上应该插在该位置，但是该位置已经放了值为4的元素，即发生哈希冲突。
>
> **线性探测**：从发生冲突的位置开始，依次向后探测，直到寻找到下一个空位置为止。
>
> - 插入
>
>   1.通过哈希函数获取待插入元素在哈希表中的位置
>
>   2.如果该位置中没有元素则直接插入新元素，如果该位置中有元素发生哈希冲突， 使用线性探测找到下一个空位置，插入新元素
>
>   ![image-20230914182150461](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20230914182150461.png)
>
> - 删除
>
>    采用闭散列处理哈希冲突时，不能随便物理删除哈希表中已有的元素，若直接删除元素 会影响其他元素的搜索。比如删除元素4，如果直接删除掉，44查找起来可能会受影 响。因此线性探测采用标记的伪删除法来删除一个元素。
>
>   ```cpp
>   // 哈希表每个空间给个标记
>   // EMPTY此位置空， EXIST此位置已经有元素， DELETE元素已经删除
>   enum State{EMPTY, EXIST, DELETE}; 
>   ```
>
>   线性探测的实现
>
>   ```cpp
>   bool Insert(const pair<K, V> kv)
>   		{
>   			Hash hash;
>   			//查看是否需要扩容，超过size的0.7就进行扩容
>   			if (_tables.size() == 0 || _n * 10 / _tables.size() >= 7)
>   			{
>   				size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;
>   				HashTable<K, V> newht;
>   				newht._tables.resize(newsize);
>   				//遍历旧表映射到新表
>   				for (auto& date : _tables)
>   				{
>   					if (date._state == EXIST)
>   						newht.Insert(date._kv);
>   				}
>   				_tables.swap(newht._tables);
>   			}
>   
>   			size_t hashi = kv.first % _tables.size();
>   
>   			//线性探测
>   			size_t i = 1;
>   			size_t index = hashi;
>   			while (_tables[index]._state == EXIST)
>   			{
>   				index = hashi + i;
>   				index %= _tables.size();
>   				++i;
>   			}
>   			_tables[index]._kv = kv;
>   			_tables[index]._state = EXIST;
>   			++_n;
>   			return true;
>   		}
>   
>   		HashDate<K, V>* find(const K& key)
>   		{
>   			if (_tables.size() == 0)
>   			{
>   				return false;
>   			}
>   			size_t hashi = key % _tables.size();
>   
>   			//线性探测
>   			size_t i = 1;
>   			size_t index = hashi;
>   			while (_tables[index]._state == EXIST)
>   			{
>   				if (_tables[index]._state == EXIST
>   					&& _tables[index].first == key)
>   				{
>   					return &_tables[index];
>   				}
>   				index = hashi + i;
>   				index %= _tables.size();
>   				++i;
>   
>   				//查找了一圈，证明表中的状态只有EXIST和DELET的状态
>   				if (index == hashi)
>   				{
>   					break;
>   				}
>   			}
>   			return nullptr;
>   		}
>   
>   		bool Erase(const K& key)
>   		{
>   			HashDate<K, V>* ret = find(key);
>   			if (ret)
>   			{
>   				ret->_state = DELETE;
>   				--_n;
>   				return true;
>   			}
>   			else
>   			{
>   				return false;
>   			}
>   		}
>   ```
>
>   ==思考：哈希表什么情况下进行扩容？如何扩容？==
>
>   这里引入一个新的概念：载荷因子α
>   $$
>   α = 填入表中元素的个数 / 散列表的长度
>   $$
>   所以α越大，表明填入表中的元素就越多，产生冲突的可能行就越大。反之α越小，表明表中的元素就越少，产生冲突的概率就越小。实际上，散列表的平均查找长度就是α的函数。
>
>   一般限定阿尔法的值为==0.75==左右。
>
>   ```cpp
>   void CheckCapacity()
>   {
>       if(_size * 10 / _ht.capacity() >= 7)
>      {
>           HashTable<K, V, HF> newHt(GetNextPrime(ht.capacity));
>           for(size_t i = 0; i < _ht.capacity(); ++i)
>          {
>               if(_ht[i]._state == EXIST)
>                   newHt.Insert(_ht[i]._val);
>          }
>           
>           Swap(newHt);
>      }
>   }
>   
>   ```
>
>   





