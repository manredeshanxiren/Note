# AVL树

## Ⅰ.avl树

> **底层结构**
>
> 前面对map/multimap/set/multiset进行了简单的介绍，在其文档介绍中发现，这几个容器有个 共同点是：其底层都是按照二叉搜索树来实现的，但是二叉搜索树有其自身的缺陷，假如往树中 插入的元素有序或者接近有序，二叉搜索树就会退化成单支树，时间复杂度会退化成O(N)，因此 map、set等关联式容器的底层结构是对二叉树进行了平衡处理，即采用平衡树来实现。

## Ⅱ. avl树的概念

> 二叉搜索树虽可以缩短查找的效率，但如果数据有序或接近有序二叉搜索树将退化为单支树，查 找元素相当于在顺序表中搜索元素，效率低下。因此，两位俄罗斯的数学家G.M.Adelson-Velskii 和E.M.Landis在1962年 发明了一种解决上述问题的方法：当向二叉搜索树中插入新结点后，如果能保证每个结点的左右 子树高度之差的绝对值不超过1(需要对树中的结点进行调整)，即可降低树的高度，从而减少平均 搜索长度。 一棵AVL树或者是空树，或者是具有以下性质的二叉搜索树：
>
> - 它的左右子树都是AVL树
> - 左右子树高度之差(简称平衡因子)的绝对值不超过1(-1/0/1)
> - ![image-20230830210407451](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/AMH8kJrICgwXWYi.png)
>   如果一棵二叉搜索树是高度平衡的，它就是AVL树。如果它有n个结点，其高度可保持在 $O(log_2 n)$，搜索时间复杂度O($log_2 n$)。

### Ⅱ. Ⅰ AVL树节点的定义

> 二叉树节点的定义：
>
> ```cpp
> template<class T>
> struct AVLTreeNode
> {
>  AVLTreeNode(const T& data)
>      : _pLeft(nullptr), _pRight(nullptr), _pParent(nullptr)
>  , _data(data), _bf(0)
>  {}
>  AVLTreeNode<T>* _pLeft;   // 该节点的左孩子
>  AVLTreeNode<T>* _pRight;  // 该节点的右孩子
>  AVLTreeNode<T>* _pParent; // 该节点的双亲
>  T _data;
>  int _bf;                  // 该节点的平衡因子
> };
> ```

Ⅱ. Ⅱ AVL树的插入

> AVL树就是在二叉搜索树的基础上引入了平衡因子，因此AVL树也可以看成是二叉搜索树。那么 AVL树的插入过程可以分为两步：
>
> ```cpp
> bool Insert(const T& data)
> {
>     // 1. 先按照二叉搜索树的规则将节点插入到AVL树中
>     // ...
>     
>     // 2. 新节点插入后，AVL树的平衡性可能会遭到破坏，此时就需要更新平衡因子，并检测是否
> 破坏了AVL树
>     //   的平衡性
>     
>  /*
>  pCur插入后，pParent的平衡因子一定需要调整，在插入之前，pParent
>  的平衡因子分为三种情况：-1，0, 1, 分以下两种情况：
>   1. 如果pCur插入到pParent的左侧，只需给pParent的平衡因子-1即可
>   2. 如果pCur插入到pParent的右侧，只需给pParent的平衡因子+1即可
>   
>  此时：pParent的平衡因子可能有三种情况：0，正负1， 正负2
>   1. 如果pParent的平衡因子为0，说明插入之前pParent的平衡因子为正负1，插入后被调整
> 成0，此时满足
>      AVL树的性质，插入成功
>   2. 如果pParent的平衡因子为正负1，说明插入前pParent的平衡因子一定为0，插入后被更
> 新成正负1，此
>      时以pParent为根的树的高度增加，需要继续向上更新
>   3. 如果pParent的平衡因子为正负2，则pParent的平衡因子违反平衡树的性质，需要对其进
> 行旋转处理
>  */
>  while (pParent)
>  {
>         // 更新双亲的平衡因子
>  if (pCur == pParent->_pLeft)
>  pParent->_bf--;
>  else
>  pParent->_bf++;
>  // 更新后检测双亲的平衡因子
>  if (0 == pParent->_bf)
>        {    
>             break;
>        }
>  else if (1 == pParent->_bf || -1 == pParent->_bf)
>  {
>               // 插入前双亲的平衡因子是0，插入后双亲的平衡因为为1 或者 -1 ，说明以双亲
> 为根的二叉树
>               // 的高度增加了一层，因此需要继续向上调整
>  pCur = pParent;
>  pParent = pCur->_pParent;
>  }
>  else
>  {
>  // 双亲的平衡因子为正负2，违反了AVL树的平衡性，需要对以pParent
>  // 为根的树进行旋转处理
>               if(2 == pParent->_bf)
>              {
>                   // ...
>              }
>               else
>              {
>                   // ...
>              }
>  }
>  }
>  return true;
> }
>      
> ```

### Ⅱ. Ⅲ AVL树的旋转

> 如果在一棵原本是平衡的AVL树中插入一个新节点，可能造成不平衡，此时必须调整树的结构， 使之平衡化。根据节点插入位置的不同，AVL树的旋转分为四种：
>
> ①新节点插入较高**左子树的左侧**---左左：右单旋
>
> ![左左_右单旋](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/w49inSey6C2ZsMG-1694598419802-3.gif)
>
> ```cpp
> /*
> 上图在插入前，AVL树是平衡的，新节点插入到8的左子树(注意：此处不是左孩子)中，8左
> 子树增加
> 了一层，导致以34为根的二叉树不平衡，要让34平衡，只能将34左子树的高度减少一层，右子
> 树增加一层，
> 即将左子树往上提，这样34转下来，因为34比8大，只能将其放在8的右子树，而如果8有
> 右子树，右子树根的值一定大于8，小于34，只能将其放在34的左子树，旋转完成后，更新节点
> 的平衡因子即可。在旋转过程中，有以下几种情况需要考虑：
>   1. 8节点的右孩子可能存在，也可能不存在
>   2. 34可能是根节点，也可能是子树
>      如果是根节点，旋转完成后，要更新根节点
>      如果是子树，可能是某个节点的左子树，也可能是右子树
> 
> 同学们再此处可举一些详细的例子进行画图，考虑各种情况，加深旋转的理解
> */
> void _RotateR(PNode pParent)
> {
>     // pSubL: pParent的左孩子
>     // pSubLR: pParent左孩子的右孩子，注意：该
>  PNode pSubL = pParent->_pLeft;
>  PNode pSubLR = pSubL->_pRight;
>     // 旋转完成之后，8的右孩子作为双亲的左孩子
>  pParent->_pLeft = pSubLR;
>     // 如果8的左孩子的右孩子存在，更新亲双亲
>  if(pSubLR)
>  pSubLR->_pParent = pParent;
>     // 34 作为 8的右孩子
>   pSubL->_pRight = pParent;
> 
>     // 因为34可能是棵子树，因此在更新其双亲前必须先保存34的双亲
>  PNode pPParent = pParent->_pParent;
> 
>     // 更新34的双亲
>  pParent->_pParent = pSubL;
> 
>     // 更新8的双亲
>  pSubL->_pParent = pPParent;
>     // 如果34是根节点，根新指向根节点的指针
>  if(NULL == pPParent)
>  {
>  _pRoot = pSubL;
>  pSubL->_pParent = NULL;
>  }
>  else
>  {
>          // 如果34是子树，可能是其双亲的左子树，也可能是右子树
>  if(pPParent->_pLeft == pParent)
>  pPParent->_pLeft = pSubL;
>  else
>  pPParent->_pRight = pSubL;
>  }
>     // 根据调整后的结构更新部分节点的平衡因子
>  pParent->_bf = pSubL->_bf = 0;
> }
> ```
>
> ②新节点插入**较高右子树的右侧**---右右：左单旋
>
> ![AVL树左单旋](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/ZFElCaKrGO59SNV.gif)
>
> 实现方法和右单旋极其类似
>
> ```cpp
> void RotateL(node* parent)
> 		{
> 			node* subR = parent->_right;
> 			node* subRL = subR->_left;
> 
> 
> 			parent->_right = subRL;
> 			if (subRL)
> 				subRL->_parent = parent;
> 
> 			node* ppnode = parent->_parent;
> 
> 			subR->_left = parent;
> 			parent->_parent = subR;
> 
> 			if (ppnode == nullptr)
> 			{
> 				_root = subR;
> 				_root->_parent = nullptr;
> 			}
> 			else
> 			{
> 				if (ppnode->_left == parent)
> 				{
> 					ppnode->_left = subR;
> 				}
> 				else
> 				{
> 					ppnode->_right = subR;
> 				}
> 				subR->_parent = ppnode;
> 			}
> 
> 			parent->_bf = subR->_bf = 0;
> 		}
> ```
>
> ③新节点插入**较高左子树的右侧**---左右：**先左单旋再右单旋**
>
> ![AVL树左单旋+右单旋](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/AVL%E6%A0%91%E5%B7%A6%E5%8D%95%E6%97%8B+%E5%8F%B3%E5%8D%95%E6%97%8B.gif)
> 将双旋变成单旋后再旋转，即：先对30进行左单旋，然后再对90进行右单旋，旋转完成后再 考虑平衡因子的更新。
>
> ```cpp
> 	void RotateLR(node* parent)
> 		{
> 			node* subL = parent->_left;
> 			node* subLR = subL->_right;
> 			int bf = subLR->_bf;
> 
> 			RotateL(parent->_left);
> 			RotateR(parent);
> 			//以下更新节点的平衡因子的情况需要通过一个一个画图去分析
> 			if (bf == 1)
> 			{
> 				parent->_bf = 0;
> 				subLR->_bf = 0;
> 				subL->_bf = -1;
> 			}
> 			else if (bf == -1)
> 			{
> 				parent->_bf = 1;
> 				subLR->_bf = 0;
> 				subL->_bf = 0;
> 			}
> 			else if (bf == 0)
> 			{
> 				parent->_bf = 0;
> 				subLR->_bf = 0;
> 				subL->_bf = 0;
> 			}
> 			else
> 			{
> 				assert(false);
> 			}
> 		}
> ```
>
> ④新节点插入较高右子树的左侧---右左：先右单旋再左单旋
>
> ![AVL树右单旋+左单旋](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/63g5eKVAI8YkFXS.gif)
>
> ```cpp
> 	void RotateRL(node* parent)
> 	{
> 			node* subR = parent->_right;
> 			node* subRL = subR->_left;
> 			int bf = subRL->_bf;
> 
> 			RotateR(parent->_right);
> 			RotateL(parent);
> 			//以下更新节点的平衡因子的情况需要通过一个一个画图去分析
> 			if (bf == 1)
> 			{
> 				parent->_bf = -1;
> 				subRL->_bf = 0;
> 				subR->_bf = 0;
> 			}
> 			else if (bf == -1)
> 			{
> 				parent->_bf = 0;
> 				subRL->_bf = 0;
> 				subR->_bf = 1;
> 			}
> 			else if (bf == 0)
> 			{
> 				parent->_bf = 0;
> 				subRL->_bf = 0;
> 				subR->_bf = 0;
> 			}
> 			else
> 			{
> 				assert(false);
> 			}
> 		}
> ```

## Ⅲ. AVL树的验证

> AVL树是在二叉搜索树的基础上加入了平衡性的限制，因此要验证AVL树，可以分两步：
>
> ①验证其为二叉搜索树
>
> 如果**中序遍历**可得到一个有序的序列，就说明为二叉搜索树
>
> ②验证其为平衡树
>
> - 每个节点子树高度差的绝对值不超过1(注意节点中如果没有平衡因子)
> - 节点的平衡因子是否计算正确
>
> ```cpp
> int _Height(PNode pRoot);
> bool _IsBalanceTree(PNode pRoot)
> {
>  // 空树也是AVL树
>  if (nullptr == pRoot) return true;
>     
>  // 计算pRoot节点的平衡因子：即pRoot左右子树的高度差
>  int leftHeight = _Height(pRoot->_pLeft);
>  int rightHeight = _Height(pRoot->_pRight);
>  int diff = rightHeight - leftHeight;
>  // 如果计算出的平衡因子与pRoot的平衡因子不相等，或者
>  // pRoot平衡因子的绝对值超过1，则一定不是AVL树
>  if (diff != pRoot->_bf || (diff > 1 || diff < -1))
>  return false;
>  // pRoot的左和右如果都是AVL树，则该树一定是AVL树
>  return _IsBalanceTree(pRoot->_pLeft) && _IsBalanceTree(pRoot-
> >_pRight);
>  }
> 
> ```
>
> ③验证用例
>
> 请同学们结合上述代码按照以下的数据次序，自己动手画AVL树的创建过程，验证代码 是否有漏洞。
>
> - 常规场景1
>
>   {16, 3, 7, 11, 9, 26, 18, 14, 15}
>
> - 特殊场景2
>
>   {4, 2, 6, 1, 3, 5, 15, 7, 16, 14}

## Ⅳ.AVL树的性能

> AVL树是一棵绝对平衡的二叉搜索树，其要求每个节点的左右子树高度差的绝对值都不超过1，这 样可以保证查询时高效的时间复杂度，即$log_2 (N)$。但是如果要对AVL树做一些结构修改的操 作，性能非常低下，比如：插入时要维护其绝对平衡，旋转的次数比较多，更差的是在删除时， 有可能一直要让旋转持续到根的位置。因此：如果需要一种查询高效且有序的数据结构，而且数 据的个数为静态的(即不会改变)，可以考虑AVL树，但一个结构经常修改，就不太适合。

[什么是AVL树][https://zhuanlan.zhihu.com/p/56066942]