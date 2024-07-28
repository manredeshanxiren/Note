# Git基础操作

## 1Git的安装

- CentOS环境

这里使用`yum`来安装,执行如下的命令即可:

```bash
sudo yum install git -y
```

安装完成之后执行如下的命令就可以查看是否安装成功,如果安装成功了就会执行这个命令就会显示安装的git的版本：

```bash
git --version
```

- Ubuntu环境

这里使用的是`apt-get`来安装：

```bash
sudo apt-get install -y
```

同样的查看是否安装成功的方式和上面的命令一样。

```bash
git --version
```

- Win：

可以直接去Git的官网进行下载：[Git - 安装 Git (git-scm.com)](https://git-scm.com/book/zh/v2/起步-安装-Git)

官方也有对应的安装教程文档，点击进去就可以看到

## 2.Git基本操作

### 2.1本地仓库的创建

- 什么是本地仓库

本地仓库（Local Repository）指的是用户在自己的计算机上创建和管理的代码仓库。这个仓库包含了项目的所有文件、提交历史、分支、标签等信息。

- 创建Git本地仓库，在向创建本地仓库的目录下执行即可：

```bash
git init
```

执行完成后会显示如下，可能不同的系统显示不一样，但是意思都是一样：

```bash
Initialized empty Git repository in /home/mi/gitcode/.git/
```

其中这个.git文件是用来跟踪和管理我们对应的仓库的，大致的目录结构是这样，我们不能去手动更改其中的一些东西，否则会导致仓库损坏：

```bash
[mi@lavm-5wklnbmaja gitcode]$ tree .git/
.git/
├── branches
├── config
├── description
├── HEAD
├── hooks
│   ├── applypatch-msg.sample
│   ├── commit-msg.sample
│   ├── post-update.sample
│   ├── pre-applypatch.sample
│   ├── pre-commit.sample
│   ├── prepare-commit-msg.sample
│   ├── pre-push.sample
│   ├── pre-rebase.sample
│   └── update.sample
├── info
│   └── exclude
├── objects
│   ├── info
│   └── pack
└── refs
    ├── heads
    └── tags

9 directories, 13 files

```

### 2.2配置本地仓库

- **设置用户名和邮箱**：

  每个提交都会包含用户名和邮箱，可以通过以下命令配置全局（对于当前用户的所有仓库）或局部（仅对当前仓库）信息。

- **全局配置**：

```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

- **局部配置**：

```bash
git config user.name "Your Name"
git config user.email "your.email@example.com"
```

- **配置默认编辑器**：
  默认情况下，Git 使用系统的默认文本编辑器，但你可以指定一个自己喜欢的编辑器。

```bash
git config --global core.editor "code --wait"  # 使用 Visual Studio Code
```

- **查看配置**：
  你可以查看当前的配置，通过以下命令查看所有全局和局部配置。

```bash
git config --list
```

- 取消设置某个配置，取消全局配置需要加上对应的`--global`选项，例如这里取消用户名，这里就是全局取消，非全局的可以不适用`--global`选项：

```bash
git config --global --unset user.name
```

## 3.认识一些Git的基本概念

<div id = "image1">这里先上图：<div>

![0d93755c6e5e7a7b3f96f4bbb5170c59-17220679614041](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/0d93755c6e5e7a7b3f96f4bbb5170c59-17220679614041.png)


这个图展示了Git的工作流程和数据流向，包括远程仓库、版本库、暂存区和工作区之间的交互。以下是对各个部分的详细介绍：

1. **远程仓库（Remote）**：
   - 这是存储在服务器上的中央版本库，团队成员可以通过远程仓库共享代码。
   - 常用操作：`fetch/clone` 和 `push`。
   - `fetch/clone`：从远程仓库拉取数据到本地版本库。
   - `push`：将本地版本库的更新推送到远程仓库。

2. **版本库（Repository）**：
   - 本地版本库，存储代码的历史版本信息，是Git的核心。
   - 常用操作：`pull` 和 `commit`。
   - `pull`：从远程仓库获取最新的更改并合并到本地版本库。
   - `commit`：将工作区或暂存区的更改保存到本地版本库。

3. **暂存区（Index / Stage）**：
   - 暂存区用于暂时存储你准备提交到版本库的文件快照。
   - 常用操作：`add` 和 `commit`。
   - `add`：将工作区的更改添加到暂存区。
   - `commit`：将暂存区的更改提交到本地版本库。

4. **工作区（Workspace）**：
   - 工作区是你实际进行开发和修改文件的地方。
   - 常用操作：`checkout` 和 `add`。
   - `checkout`：从版本库获取文件到工作区，或在工作区切换分支。
   - `add`：将工作区的文件修改添加到暂存区，准备提交。

### 3.1操作流程：

1. **从远程仓库克隆代码**：
   - 使用 `git clone` 命令，将远程仓库的代码复制到本地，创建本地版本库。

2. **在工作区进行修改**：
   - 在工作区（workspace）中进行代码修改、添加新文件或删除文件。

3. **将修改添加到暂存区**：
   - 使用 `git add` 命令，将工作区的修改添加到暂存区（stage）。

4. **提交修改到本地版本库**：
   - 使用 `git commit` 命令，将暂存区的内容提交到本地版本库（repository）。

5. **推送提交到远程仓库**：
   - 使用 `git push` 命令，将本地版本库的提交推送到远程仓库（remote）。

6. **从远程仓库拉取更新**：
   - 使用 `git fetch` 或 `git pull` 命令，从远程仓库获取最新的更改并更新本地版本库。

那么Git是如何基于此实现版本管理的呢？

## 4.一些使用场景

### 4.1添加文件场景一

我们这里创建三个文件并添加到暂存区之后commit到版本库：

```bash
[mi@lavm-5wklnbmaja gitcode]$ touch file1 file2 file3
[mi@lavm-5wklnbmaja gitcode]$ la
.  ..  file1  file2  file3  .git  ReadMe
[mi@lavm-5wklnbmaja gitcode]$ git add file*
[mi@lavm-5wklnbmaja gitcode]$ git commit -m "添加file1-3"
[master 58f5185] 添加file1-3
 3 files changed, 0 insertions(+), 0 deletions(-)
 create mode 100644 file1
 create mode 100644 file2
 create mode 100644 file3
```

`git log`命令，用于打印出我们的提交记录：

​	如下我们最新的提交是添加了三个文件，上一次是测试提交；

​    其中第一行对应的`commit`后面跟的是`commit id`是一个md5值；

​	第二行和第三行是对应的提交者的信息，和提交的日期，最后一行是我们`commit`的时候的描述信息。

```bash
[mi@lavm-5wklnbmaja gitcode]$ git log
commit 58f5185f2050dccbcaff6b8861eea844288105cd
Author: mqy <mqingyang@163.com>
Date:   Sun Jul 21 23:15:47 2024 +0800

    添加file1-3

commit 99d14bd9e71e882ac90c60249a4b9b13709d7900
Author: mqy <mqingyang@163.com>
Date:   Sun Jul 21 22:39:25 2024 +0800

    测试
```

同时`git log`也可以加一些参数例如让信息在一行显示：

```bash
[mi@lavm-5wklnbmaja gitcode]$ git log --pretty=oneline
58f5185f2050dccbcaff6b8861eea844288105cd 添加file1-3
99d14bd9e71e882ac90c60249a4b9b13709d7900 测试
```

### 4.2查看git文件

首先我们查看以下git文件下都有什么：

用tree命令来查看一下：

```bash
[mi@lavm-5wklnbmaja gitcode]$ tree .git/
.git/
├── branches
├── COMMIT_EDITMSG
├── config
├── description
├── HEAD
├── hooks
│   ├── applypatch-msg.sample
│   ├── commit-msg.sample
│   ├── post-update.sample
│   ├── pre-applypatch.sample
│   ├── pre-commit.sample
│   ├── prepare-commit-msg.sample
│   ├── pre-push.sample
│   ├── pre-rebase.sample
│   └── update.sample
├── index
├── info
│   └── exclude
├── logs
│   ├── HEAD
│   └── refs
│       └── heads
│           └── master
├── objects
│   ├── 58
│   │   └── f5185f2050dccbcaff6b8861eea844288105cd
│   ├── 6a
│   │   └── a86de6831a0d231d154dcc15e4e86d5502d358
│   ├── 99
│   │   └── d14bd9e71e882ac90c60249a4b9b13709d7900
│   ├── 9f
│   │   └── 4d96d5b00d98959ea9960f069585ce42b1349a
│   ├── dc
│   │   └── b8a6f4408f11ab8afe025b20a7315a2732446b
│   ├── e6
│   │   └── 9de29bb2d1d6434b8b29ae775ad8c2e48c5391
│   ├── info
│   └── pack
└── refs
    ├── heads
    │   └── master
    └── tags

18 directories, 24 files
```

**index：**

> 其中`index`就是我们在执行`add`命令的时候，git帮我们将文件提交到暂存区形成的索引。

**HEAD：**

> `HEAD`是一个指针，它指向的是当前所在的仓库分支，也可以理解为指向当前最新的`commit id`。
>
> 我们这里先找到对应的`HEAD`指针存储在哪，发现它存储在`.git`下的这个路径`refs/heads/master`，然后我们在查看以下它的内容，发现对应的内容和最新提交的`commit id`是一致的。
>
> ```bash
> [mi@lavm-5wklnbmaja gitcode]$ git log
> commit 58f5185f2050dccbcaff6b8861eea844288105cd
> Author: mqy <mqingyang@163.com>
> Date:   Sun Jul 21 23:15:47 2024 +0800
> 
>  添加file1-3
> 
> commit 99d14bd9e71e882ac90c60249a4b9b13709d7900
> Author: mqy <mqingyang@163.com>
> Date:   Sun Jul 21 22:39:25 2024 +0800
> 
>  测试
> [mi@lavm-5wklnbmaja gitcode]$ cat .git/HEAD
> ref: refs/heads/master
> [mi@lavm-5wklnbmaja gitcode]$ cat .git/refs/heads/master 
> 58f5185f2050dccbcaff6b8861eea844288105cd
> 
> ```

**objects：**

> 首先，其实不管是暂存区还是对应的分支，其实内部都是维护了对应的索引而不是文件/对象。那么其实对应的对象都是存储在object下的。其中object和分支可以理解为[图(点击可跳转到对应的图)](#image1)上的版本库。
>
> 这里将`commit id`分成了两段，前2位为文件夹名后面的表示对象名。
>
> ```
> ├── objects
> │   ├── 58
> │   │   └── f5185f2050dccbcaff6b8861eea844288105cd
> │   ├── 6a
> │   │   └── a86de6831a0d231d154dcc15e4e86d5502d358
> │   ├── 99
> │   │   └── d14bd9e71e882ac90c60249a4b9b13709d7900
> │   ├── 9f
> │   │   └── 4d96d5b00d98959ea9960f069585ce42b1349a
> │   ├── dc
> │   │   └── b8a6f4408f11ab8afe025b20a7315a2732446b
> │   ├── e6
> │   │   └── 9de29bb2d1d6434b8b29ae775ad8c2e48c5391
> │   ├── info
> │   └── pack
> ```
>
> 那么我们查看一下master(用HEAD指针)下有啥吧，用这个命令，这个命令可以查看任何`commit id`的信息,也不一定是`HEAD`：
>
> ```bash
> git cat-file -p HEAD
> ```
>
> 运行查看：
>
> ```bash
> [mi@lavm-5wklnbmaja gitcode]$ git cat-file -p 58f5185f2050dccbcaff6b8861eea844288105cd
> tree 6aa86de6831a0d231d154dcc15e4e86d5502d358
> parent 99d14bd9e71e882ac90c60249a4b9b13709d7900
> author mqy <mqingyang@163.com> 1721574947 +0800
> committer mqy <mqingyang@163.com> 1721574947 +0800
> 
> 添加file1-3
> 
> ```
>
> 参数解释：
>
> parent：是本次提交的上一次提交的`commit id`
>
> tree：首先我们可以次啊用同样的命令来看看它内部是什么，好像是我们文件和`commit id`的映射？
>
> ```bash
> [mi@lavm-5wklnbmaja gitcode]$ git cat-file -p 6aa86de6831a0d231d154dcc15e4e86d5502d358
> 100644 blob 9f4d96d5b00d98959ea9960f069585ce42b1349a	ReadMe
> 100644 blob e69de29bb2d1d6434b8b29ae775ad8c2e48c5391	file1
> 100644 blob e69de29bb2d1d6434b8b29ae775ad8c2e48c5391	file2
> 100644 blob e69de29bb2d1d6434b8b29ae775ad8c2e48c5391	file3
> ```
>
> 我们查看以下第一个`commit id`存储了什么吧，原来是我们的文件内容，其实这里就是git帮我们把每一个版本的文件修改内容都维护起来了：
>
> ```bash
> [mi@lavm-5wklnbmaja gitcode]$ git cat-file -p 9f4d96d5b00d98959ea9960f069585ce42b1349a
> Hello Git
> ```

### 4.3修改文件

**Git追踪管理的其实是修改，而不是文件。**

我们尝试修改一下已经提交了的`ReadMe`文件，向其中多写入一行字符，然后再`git`仓库目录下执行`git status`：

```bash
[mi@lavm-5wklnbmaja gitcode]$ git status
# On branch master
# Changes not staged for commit:
#   (use "git add <file>..." to update what will be committed)
#   (use "git checkout -- <file>..." to discard changes in working directory)
#
#	modified:   ReadMe
#
no changes added to commit (use "git add" and/or "git commit -a")
```

其中我们可以看到第一行是没有暂存区的改变需要提交，接下来就是ReadMe文件被修改的信息，但是`git status`是看不到对应的文件被修改的内容，只能获取到哪些文件被修改了。

那么Git也提供了一个命令用于查看暂存区和工作区之间的差异，可以看到文件的具体内容哪些被修改了：`git diff`

```bash
[mi@lavm-5wklnbmaja gitcode]$ git diff
diff --git a/ReadMe b/ReadMe   //比较两个版本的ReadMe文件
index 9f4d96d..c52a7d0 100644  //显示两个版本的文件索引（哈希值）和文件权限。
--- a/ReadMe                   //旧版本文件的内容开始
+++ b/ReadMe                   //新版本文件的内容开始
@@ -1 +1,2 @@          //显示差异的上下文，旧版本文件的第1行，新版本文件从第1行开始，有两行内容
 Hello Git             //旧版本文件的第1行内容
+Hello Tx              //新版本文件新增加的第2行内容
[mi@lavm-5wklnbmaja gitcode]$ git diff ReadMe 
diff --git a/ReadMe b/ReadMe
index 9f4d96d..c52a7d0 100644
--- a/ReadMe
+++ b/ReadMe
@@ -1 +1,2 @@
 Hello Git
+Hello Tx
```

我们将`ReadMe`提交到暂存区之后再次执行一下`git status`

```bash
[mi@lavm-5wklnbmaja gitcode]$ git add ReadMe 
[mi@lavm-5wklnbmaja gitcode]$ git status
# On branch master
# Changes to be committed:
#   (use "git reset HEAD <file>..." to unstage)
#
#	modified:   ReadMe
#
[mi@lavm-5wklnbmaja gitcode]$ git commit -m "modify ReadMe"
[master db2ce21] modify ReadMe
 1 file changed, 1 insertion(+)
[mi@lavm-5wklnbmaja gitcode]$ git status
# On branch master
nothing to commit, working directory clean
```

### 4.4Git版本回退

`git reset` 命令的三个参数对比对工作区、暂存区（索引）以及版本库（HEAD）的影响的表格：

| 参数             | 工作区               | 暂存区（索引）       | 版本库（HEAD）                                 |
| ---------------- | -------------------- | -------------------- | ---------------------------------------------- |
| `--soft`         | 无影响               | 无影响               | 移动HEAD指针到指定提交                         |
| `--mixed` (默认) | 无影响               | 移除暂存区中的更改   | 移动HEAD指针到指定提交                         |
| `--hard`         | 丢弃所有未提交的更改 | 丢弃所有未提交的更改 | 移动HEAD指针到指定提交，并丢弃所有未提交的更改 |

**详细解释**

- **`--soft`**: 当使用 `--soft` 参数时，`git reset` 只会移动HEAD指针到指定的提交，但不会改变暂存区和工作区的状态。这意味着所有的更改仍然保留在暂存区中，你可以重新提交或修改。

- **`--mixed` (默认)**: 默认情况下，`git reset` 使用 `--mixed` 参数。它会移动HEAD指针到指定的提交，并且更新暂存区以匹配该提交的状态，但不会改变工作区。这通常用于取消暂存文件，即将文件从暂存区移回工作区。

- **`--hard`**: 使用 `--hard` 参数时，`git reset` 会移动HEAD指针到指定的提交，并且重置暂存区和工作区到该提交的状态。这将丢弃所有未提交的更改，因此在使用这个选项之前需要非常小心。

请注意，使用 `--hard` 参数时，所有未提交的更改都会被丢弃，且无法恢复。因此，在执行可能影响工作区和暂存区的 `git reset` 操作之前，确保你已经保存了所有重要的更改。

我们尝试使用一下：

```bash
[mi@lavm-5wklnbmaja gitcode]$ git log --pretty=oneline   //首先这里打印日志
db2ce217bd898c59522f748672436dd7e17541d2 modify ReadMe
58f5185f2050dccbcaff6b8861eea844288105cd 添加file1-3
99d14bd9e71e882ac90c60249a4b9b13709d7900 测试
[mi@lavm-5wklnbmaja gitcode]$ git reset --hard    99d14bd9e71e882ac90c60249a4b9b13709d7900                 //--hard回退到第一次的commit
HEAD is now at 99d14bd 测试
[mi@lavm-5wklnbmaja gitcode]$ ls
ReadMe
[mi@lavm-5wklnbmaja gitcode]$ cat ReadMe 
Hello Git
[mi@lavm-5wklnbmaja gitcode]$ git reset --hard           db2ce217bd898c59522f748672436dd7e17541d2                //--hard回退到历史最新的commit
HEAD is now at db2ce21 modify ReadMe
[mi@lavm-5wklnbmaja gitcode]$ ls
file1  file2  file3  ReadMe
[mi@lavm-5wklnbmaja gitcode]$ git log --pretty=oneline  //我们可以查看到日志也恢复了
db2ce217bd898c59522f748672436dd7e17541d2 modify ReadMe
58f5185f2050dccbcaff6b8861eea844288105cd 添加file1-3
99d14bd9e71e882ac90c60249a4b9b13709d7900 测试
[mi@lavm-5wklnbmaja gitcode]$ cat ReadMe                //文件内容也恢复了
Hello Git
Hello Tx
```

这里假设我们丢失了上面的log结果，也就是说我们找不到所有的`commit id`了，这时候我们还有一个命令`git reflog`：我们发现`reflog`还是为我们保存了每一次的`commit id`

```bash
[mi@lavm-5wklnbmaja gitcode]$ git reflog
99d14bd HEAD@{0}: reset: moving to 99d14bd9e71e882ac90c60249a4b9b13709d7900
db2ce21 HEAD@{1}: reset: moving to db2ce217bd898c59522f748672436dd7e17541d2
99d14bd HEAD@{2}: reset: moving to 99d14bd9e71e882ac90c60249a4b9b13709d7900
db2ce21 HEAD@{3}: commit: modify ReadMe
58f5185 HEAD@{4}: commit: 添加file1-3
99d14bd HEAD@{5}: commit (initial): 测试
[mi@lavm-5wklnbmaja gitcode]$ git log --pretty=oneline
99d14bd9e71e882ac90c60249a4b9b13709d7900 测试
```

### 4.5git撤销修改

| 工作区 | 暂存区 | 版本库 | 解决方式                                                     |
| ------ | ------ | ------ | ------------------------------------------------------------ |
| 有修改 | 无修改 | 无修改 | `git checkout -- [filename]` ：执行的效果就是将工作区中的修改撤销掉 |
| 有修改 | 有修改 | 无修改 | `git reset HEAD + git checkout -- [filename] `:执行的效果先将暂存区中的修改撤销到和当前版本库一致，然后再将工作区的修改撤销 |
| 有修改 | 有修改 | 有修改 | 前提条件：暂未`push`(提交到远程仓库)，`git reset --hard HEAD^`:执行之后的效果就是将三个区域的修改同步为上一次版本库的版本 |

补充：

> 对于`git reset HEAD`命令：
>
> ```bash
> git reset HEAD  //回退到当前版本
> git reset HEAD^ //回退到上一次版本
> git reset HEAD^^ //回退到上上一次版本
> ...
> ```

- 删除文件

git的`git rm fileName`命令：这里我们可以看到使用这个命令git直接帮我们把变动直接更新到了暂存区，我们只需要执行以下`commit`命令就可以更新到版本库了；当然也可以手动删除，但是需要先执行`add`再执行`commit`才可以更新到暂存区和版本库。

```bash
[mi@lavm-5wklnbmaja gitcode]$ ls
file1  file2  file3  ReadMe
[mi@lavm-5wklnbmaja gitcode]$ git rm file3 
rm 'file3'
[mi@lavm-5wklnbmaja gitcode]$ ls
file1  file2  ReadMe
[mi@lavm-5wklnbmaja gitcode]$ git status
# On branch master
# Changes to be committed:
#   (use "git reset HEAD <file>..." to unstage)
#
#	deleted:    file3
#
[mi@lavm-5wklnbmaja gitcode]$ git commit -m "delete file3"
[master feb6306] delete file3
 1 file changed, 0 insertions(+), 0 deletions(-)
 delete mode 100644 file3
[mi@lavm-5wklnbmaja gitcode]$ git status
# On branch master
nothing to commit, working directory clean
```

## 5.分支管理

### 5.1查看分支

`git branch`:可以看到这里我们只有一个分支master，前面的`*`代表的就是HEAD

```bash
[mi@lavm-5wklnbmaja gitcode]$ git branch
* master
```

### 5.2创建本地分支

`git branch 分支名  `：例如这里我们创建一个叫做dev的分支

```bash
[mi@lavm-5wklnbmaja gitcode]$ git branch dev
[mi@lavm-5wklnbmaja gitcode]$ git branch
  dev
* master
```

### 5.3切换本地分支

`git checkout 分支名` ：注意这里和之前的撤销操作不一样撤销对应的是:

 `git checkout --`,这里的执行效果就是将本地分支从`master`切换到`dev`

```bash
[mi@lavm-5wklnbmaja gitcode]$ git checkout dev
Switched to branch 'dev'
[mi@lavm-5wklnbmaja gitcode]$ git branch
* dev
  master
```

- 具体场景：

我们这里在dev分支中对`Readme`文件做一些修改，然后到master分支去看看

```bash
[mi@lavm-5wklnbmaja gitcode]$ ls
file1  file2  ReadMe
[mi@lavm-5wklnbmaja gitcode]$ vim ReadMe   //对ReadMe文件进行修改
[mi@lavm-5wklnbmaja gitcode]$ cat ReadMe   //查看ReadMe文件内容
Hello Git
Hello Tx
change from dev
[mi@lavm-5wklnbmaja gitcode]$ git add ReadMe    //将ReadMe文件的修改提交到暂存区
[mi@lavm-5wklnbmaja gitcode]$ git commit -m "md ReadMe" //将修改提交到版本库
[dev 31a9f0a] md ReadMe
 1 file changed, 1 insertion(+)
[mi@lavm-5wklnbmaja gitcode]$ git status
# On branch dev
nothing to commit, working directory clean
[mi@lavm-5wklnbmaja gitcode]$ cat ReadMe  
Hello Git
Hello Tx
change from dev

--------------------------------------------- //接下来切换到master分支看看

[mi@lavm-5wklnbmaja gitcode]$ git checkout master   //切换分支
Switched to branch 'master'
[mi@lavm-5wklnbmaja gitcode]$ ls
file1  file2  ReadMe
[mi@lavm-5wklnbmaja gitcode]$ cat ReadMe            //查看master分支下的ReadMe内容
Hello Git                                           //我们发现是不一样
Hello Tx
```

### 5.4合并分支

`git merge 分支名` : 将对应命令中的分支合并到当前所在的分支，例如这里我们想将dev的change合并到master：

```bash
[mi@lavm-5wklnbmaja gitcode]$ git checkout master     //首先我们切换到master
Already on 'master'
[mi@lavm-5wklnbmaja gitcode]$ git merge dev           //执行合并分支的命令
Updating feb6306..31a9f0a
Fast-forward                                        //这里的Fast-forward是快进模式的意思
 ReadMe | 1 +
 1 file changed, 1 insertion(+)
[mi@lavm-5wklnbmaja gitcode]$ cat ReadMe       //查看一下master分支的ReadMe文件的内容
Hello Git
Hello Tx
change from dev

```

Fast-forward：快进模式，因为这里直接将master当前最新的`commit id`直接切换为了dev的最新`commit id`,所以整个合并的速度非常的快，我们可以查看一下这两个分支对应的`commit id`，发现是一模一样的：

```bash
[mi@lavm-5wklnbmaja gitcode]$ cat .git/refs/heads/master
31a9f0a550e020137f5cb9bb9d9d56d2751b569d
[mi@lavm-5wklnbmaja gitcode]$ cat .git/refs/heads/dev
31a9f0a550e020137f5cb9bb9d9d56d2751b569d
```

### 5.5删除分支

`git branch -d 分支名`：注意删除对应的分支，不能是当前分支

```bash
[mi@lavm-5wklnbmaja gitcode]$ git checkout dev
Switched to branch 'dev'
[mi@lavm-5wklnbmaja gitcode]$ git branch -d dev      //当前在dev分支，尝试删除dev分支
error: Cannot delete the branch 'dev' which you are currently on.
[mi@lavm-5wklnbmaja gitcode]$ git checkout master   //切换到master分支
Switched to branch 'master'
[mi@lavm-5wklnbmaja gitcode]$ git branch -d dev     //删除dev分支
Deleted branch dev (was 31a9f0a).
[mi@lavm-5wklnbmaja gitcode]$ git branch            //查看本地仓库的分支
* master
```

`git branch -D 分支名` ：代表强制删除分支,上面的`-d`选项只能删除我们没有进行提交的分支，我们需要删除提交过的分支的时候就不能采用上面的命令了。

```bash
[mi@lavm-5wklnbmaja gitcode]$ git checkout -b dev1   //创建并切换到分支dev1
Switched to a new branch 'dev1'
[mi@lavm-5wklnbmaja gitcode]$ ls
file1  file2  ReadMe
[mi@lavm-5wklnbmaja gitcode]$ vi ReadMe             //修改ReadMe
[mi@lavm-5wklnbmaja gitcode]$ git add ReadMe        //提交修改
[mi@lavm-5wklnbmaja gitcode]$ git commit -m "mod ReadMe"
[dev1 1ab15ef] mod ReadMe
 1 file changed, 1 insertion(+), 1 deletion(-)
[mi@lavm-5wklnbmaja gitcode]$ git checkout master
Switched to branch 'master'
[mi@lavm-5wklnbmaja gitcode]$ git branch -d dev1     //尝试删除dev1分支，发现失败
error: The branch 'dev1' is not fully merged.
If you are sure you want to delete it, run 'git branch -D dev1'.
[mi@lavm-5wklnbmaja gitcode]$ git branch -D dev1    //确认一定要删除使用-D选项
Deleted branch dev1 (was 1ab15ef).
```

### 5.6合并冲突

#### 5.6.1冲突的产生

场景：这里我们创建了一个新的分支`dev1`，在`dev1`和`master`中对ReadMe中的同一行进行了修改；

```bash
[mi@lavm-5wklnbmaja gitcode]$ git checkout -b dev1        //创建并切换到新的分支
Switched to a new branch 'dev1'
[mi@lavm-5wklnbmaja gitcode]$ git branch                
* dev1
  master
[mi@lavm-5wklnbmaja gitcode]$ ls
file1  file2  ReadMe
[mi@lavm-5wklnbmaja gitcode]$ vi ReadMe                  //修改ReadMe中的文件
[mi@lavm-5wklnbmaja gitcode]$ git add ReadMe 
[mi@lavm-5wklnbmaja gitcode]$ git commit -m "change ReadMe from dev1"  //提交到版本库
[dev1 534b774] change ReadMe from dev1
 1 file changed, 1 insertion(+), 1 deletion(-)
 [mi@lavm-5wklnbmaja gitcode]$ git checkout master      //切换到master分支
Switched to branch 'master'
[mi@lavm-5wklnbmaja gitcode]$ ls              
file1  file2  ReadMe
[mi@lavm-5wklnbmaja gitcode]$ vim ReadMe               //修改同样的行
[mi@lavm-5wklnbmaja gitcode]$ git add ReadMe 
[mi@lavm-5wklnbmaja gitcode]$ git commit -m "change ReadMe from master"
[master 3508c78] change ReadMe from master
 1 file changed, 1 insertion(+), 1 deletion(-)
```

#### 5.6.2解决冲突并合并

- 尝试合并：这里尝试合并之后，git提示我们有合并冲突，并告诉我们在哪个文件，我们只需要打开文件，就可以看到git帮我们把冲突的内容很清晰的划分出来了。

```bash
[mi@lavm-5wklnbmaja gitcode]$ git merge dev1         //这里在master分支尝试合并dev1
Auto-merging ReadMe                                  //这里提示了冲突在ReadMe文件
CONFLICT (content): Merge conflict in ReadMe         //提示我们解决冲突之后需要重新提交
Automatic merge failed; fix conflicts and then commit the result.
[mi@lavm-5wklnbmaja gitcode]$ cat ReadMe            //我们查看ReadMe文件
Hello Git                                           
Hello Tx
<<<<<<< HEAD
change from master                                  //这一行代表在master分支的修改
=======
change from dev1                                    //这一行代表在dev1分支的修改
>>>>>>> dev1                                        
 
```

- 处理冲突：我们只需要将想保留下来的修改留下来，剩下的部分删除就好了，比如这里我们想留下在master的修改，就删除掉dev1中对应的修改

```bash
  1 Hello Git
  2 Hello Tx                                                               
  3 change from master
```

这里我们提交之后再进行合并操作：我们发现合并成功了，这就是一次冲突的处理

```bash
[mi@lavm-5wklnbmaja gitcode]$ git add ReadMe 
[mi@lavm-5wklnbmaja gitcode]$ git commit -m "fix conflict from ReadMe"
[master 234dc10] fix conflict from ReadMe
[mi@lavm-5wklnbmaja gitcode]$ git merge dev1
Already up-to-date.
```

我们发现这里的merge的模式好像不是fast-forward模式：因为之前我们的合并如果是fast-forward模式的话，合并成功之后会有fast-forward的提示，那么其实这里git默认给我们采用了no-ff，也就是非fast-forward模式，这两种模式有什么区别呢？我们可以看到在no-ff模式可以看到这个修改是合并进来的还是commit进来的，比fast-forward模式要清晰一些。

我们也可以在合并的时候告诉git我们要选择no-ff模式，`git merge -no-ff 分支名 `

```bash
//这是no-ff模式
[mi@lavm-5wklnbmaja gitcode]$ git log --graph --abbrev-commit
*   commit 234dc10
|\  Merge: 3508c78 534b774
| | Author: mqy <mqingyang@163.com>
| | Date:   Sun Jul 28 18:24:04 2024 +0800
| | 
| |     fix conflict from ReadMe
| |   
| * commit 534b774
| | Author: mqy <mqingyang@163.com>
| | Date:   Sun Jul 28 18:04:29 2024 +0800
| | 
| |     change ReadMe from dev1
| |   
* | commit 3508c78
|/  Author: mqy <mqingyang@163.com>
|   Date:   Sun Jul 28 18:06:50 2024 +0800
|   
|       change ReadMe from master
|  
* commit 31a9f0a
| Author: mqy <mqingyang@163.com>
| Date:   Sat Jul 27 16:19:42 2024 +0800
| 
|     md ReadMe
|  
* commit feb6306 
----------------------------------------------------
//fast-forward模式
[mi@lavm-5wklnbmaja gitcode]$ git log --graph --abbrev-commit
* commit 749daa6
| Author: mqy <mqingyang@163.com>
| Date:   Sun Jul 28 18:48:03 2024 +0800
| 
|     mod from dev3
|    
*   commit 234dc10
|\  Merge: 3508c78 534b774
| | Author: mqy <mqingyang@163.com>
| | Date:   Sun Jul 28 18:24:04 2024 +0800
| | 
| |     fix conflict from ReadMe
| |   
| * commit 534b774
| | Author: mqy <mqingyang@163.com>
| | Date:   Sun Jul 28 18:04:29 2024 +0800
| | 
| |     change ReadMe from dev1
| |   
* | commit 3508c78
|/  Author: mqy <mqingyang@163.com>
|   Date:   Sun Jul 28 18:06:50 2024 +0800
|   
|       change ReadMe from master
|  
* commit 31a9f0a
```

### 5.7bug分支

这里假设一个场景：

我们基于`master`分支出了一个`dev1`，但是这时我们不知道`master`是有`bug`的，我们在`dev1`中进行开发，开发到一半时，发现`master`中是存在`bug`我们要去紧急处理一下，但是这时候我们当前的分支又没有开发完成，不想让修改影响到其他分支，我们就可以用`git stash`将未提交的修改存储起来，然后再去`master`分支拉出一个`bugfix`分支来进行`bug`的修复，修复完成之后我们将`bugfix`分支合并到`master`然后再回到`dev1`继续进行开发，则需要将使用git stash中暂存的修改先恢复出来，然后进行开发，**当我们开发完成的时候，可以先将master分支合并到dev1，然后再将dev1合并到master**(这样做的好处是如果有冲突或者造成新的bug我们可以在dev1分支上解决好，而不是造成master分支的bug).

这里我们来着重演示一下`git stash`命令的使用：将工作区的修改存入`stash`

```bash
[mi@lavm-5wklnbmaja gitcode]$ git checkout -b dev1     //创建并且切换到dev1分支
Switched to a new branch 'dev1'
[mi@lavm-5wklnbmaja gitcode]$ vim ReadMe               //修改ReadMe之中的内容
[mi@lavm-5wklnbmaja gitcode]$ cat ReadMe 
Hello Git
Hillo Tx
abc
xyz
[mi@lavm-5wklnbmaja gitcode]$ clear
[mi@lavm-5wklnbmaja gitcode]$ git stash         
//然后将工作区当前的修改stash之后，再查看一下ReadMe的内容
Saved working directory and index state WIP on dev1: 749daa6 mod from dev3
HEAD is now at 749daa6 mod from dev3
[mi@lavm-5wklnbmaja gitcode]$ cat ReadMe 
Hello Git
Hillo Tx
abc
```

如何将stash中的修改恢复到工作区，有以下几种方法：

- 使用 `git stash pop` ：

> 这将恢复最近一次暂存的更改，并从暂存列表中删除它。
>
> 示例：执行 `git stash pop` 命令后，暂存的更改将被应用到工作区，如果有冲突，需要手动解决。

- 使用 `git stash apply` ：

> 恢复最近一次暂存的更改，但不会从暂存列表中删除。
>
> 例如，如果您想先查看恢复的效果，确保没有问题后再决定是否从暂存列表中删除，可以先使用 `git stash apply` 。

- 如果您暂存了多次更改，可以通过指定索引来恢复特定的暂存：

> `git stash apply stash@{0}` 恢复第一个暂存。
>
> 假设您暂存了三次更改，索引分别为 `stash@{0}` 、 `stash@{1}` 、 `stash@{2}` ，您可以根据需要选择恢复特定的一次暂存。

- 查看暂存列表：

> 使用 `git stash list` 可以查看所有暂存的列表，包括它们的描述信息，以便确定要恢复的具体暂存。

## 6.git远程操作

### 6.1创建远程仓库

有很多主流的远程仓库网站，这里以GitHub为例：

**在 GitHub 上创建远程仓库**：

1. 登录您的 GitHub 账号。
2. 点击页面右上角的“+”号，选择“New repository”（新建仓库）。
3. 为仓库输入名称，例如“my_project”。
4. （可选）填写仓库的描述，让其他人更容易理解仓库的用途。
5. 选择仓库的可见性，例如“Public”（公开）或“Private”（私有）。
6. 选择是否添加 README 文件、.gitignore 文件等初始化选项。
7. 点击“Create repository”（创建仓库）按钮。

创建完成后，您将获得一个远程仓库的 URL，例如 `https://github.com/your_username/my_project.git` ，您可以使用这个 URL 来将本地仓库与远程仓库关联。

更详细也可以参考这个视频：[01-创建远程仓库_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1iq4y1D7KZ/?spm_id_from=333.337.search-card.all.click&vd_source=73d5c26cec29c302c35a5e417bb32842)

### 6.2克隆远程仓库

- https

这里使用的是`git clone [url] `命令：注意这里不能在本地仓库的目录下克隆

```bash
[mi@lavm-5wklnbmaja ~]$ git clone https://gitee.com/slow-heating-shaanxi-people/remote-gitcode.git
Cloning into 'remote-gitcode'...
remote: Enumerating objects: 4, done.
remote: Counting objects: 100% (4/4), done.
remote: Compressing objects: 100% (4/4), done.
remote: Total 4 (delta 0), reused 0 (delta 0), pack-reused 0
Unpacking objects: 100% (4/4), done.
[mi@lavm-5wklnbmaja ~]$ ls
cloud-backup  gitcode  install.sh  learn  other_lib  remote-gitcode
```

- SSH：

  1. 生成 SSH 密钥对（如果还没有）：    

     - 在本地终端中运行 `ssh-keygen -t rsa -C "your_email@example.com"` ，然后按照提示操作。

     -  将公钥添加到您的代码托管平台（如 GitHub、GitLab 等）的 SSH 密钥设置中。 完成准备工作后，您可以按照以下步骤通过 SSH 方式克隆远程仓库： 假设您已经在代码托管平台上创建了远程仓库，并获取到了 SSH 克隆 URL，例如：`git@github.com:username/repository.git` 在本地终端中，运行以下命令：

        ``` git clone git@github.com:username/repository.git ``` 其中 `username` 是您的用户名，`repository` 是远程仓库的名称。 运行命令后，Git 会将远程仓库克隆到本地的当前目录下，并创建一个与仓库名称相同的文件夹。 例如，如果远程仓库名为 `my_project` ，则会在当前目录创建一个 `my_project` 文件夹，其中包含了远程仓库的所有内容。 

 exp：输入完对应的`ssh-keygen`命令之后一直回车就好，我们就会在.ssh目录下看到两个文件`id_rsa` 和 `id_rsa.pub`，第一个是私钥，第二个是公钥，我们只需要把公钥也就是 `id_rsa.pub`中的内容配置在对应的远端仓库，我们就可以利用ssh来进行克隆仓库啦！

```
[mi@lavm-5wklnbmaja ~]$ ssh-keygen -t rsa -C "1448554339@qq.com"
Generating public/private rsa key pair.
Enter file in which to save the key (/home/mi/.ssh/id_rsa): 
Enter passphrase (empty for no passphrase): 
Enter same passphrase again: 
Your identification has been saved in /home/mi/.ssh/id_rsa.
Your public key has been saved in /home/mi/.ssh/id_rsa.pub.
The key fingerprint is:
SHA256:xEWp8V3cHcZjkib+4pWslWivDya7E9r8UbB/u0Vp95E 1448554339@qq.com
The key's randomart image is:
+---[RSA 2048]----+
|         .o. .o+o|
|       ..... +++o|
|        o+.oo.o .|
|       .. ..+   o|
|        S  .+.oEo|
|         . +oB..+|
|        +.++*. .o|
|       . ++oo.. o|
|         o+oo. o.|
+----[SHA256]-----+
[mi@lavm-5wklnbmaja ~]$ cd .ssh/
[mi@lavm-5wklnbmaja .ssh]$ ls
id_rsa  id_rsa.pub  known_hosts
```

省略配置的过程，详细可参看这个视频：[02-生成公钥-添加公钥_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1y64y1e78q/?spm_id_from=333.337.search-card.all.click&vd_source=73d5c26cec29c302c35a5e417bb32842)，我们配置好之后就可以进行克隆仓库了：

```bash
[mi@lavm-5wklnbmaja ~]$ git clone git@gitee.com:slow-heating-shaanxi-people/remote-gitcode.git
Cloning into 'remote-gitcode'...
remote: Enumerating objects: 4, done.
remote: Counting objects: 100% (4/4), done.
remote: Compressing objects: 100% (4/4), done.
remote: Total 4 (delta 0), reused 0 (delta 0), pack-reused 0
Receiving objects: 100% (4/4), done.
[mi@lavm-5wklnbmaja ~]$ ls
cloud-backup  install.sh  learn  other_lib  remote-gitcode
```

### 6.3向远程仓库进行推送

`git push`命令：例如这里我们创建一个file文件，然后将其推送到远端的仓库，push这个命令可以理解为两个分支之间的交互，也就是将本地的一个分支提交到远端分支。

```bash
[mi@lavm-5wklnbmaja remote-gitcode]$ vim file.txt
[mi@lavm-5wklnbmaja remote-gitcode]$ git add file.txt 
[mi@lavm-5wklnbmaja remote-gitcode]$ git commit -m "add file.txt"
[master 88528a7] add file.txt
 1 file changed, 2 insertions(+)
 create mode 100644 file.txt
[mi@lavm-5wklnbmaja remote-gitcode]$ git push origin master   //origin是远端仓库，master是本地仓库
Counting objects: 4, done.
Delta compression using up to 2 threads.
Compressing objects: 100% (2/2), done.
Writing objects: 100% (3/3), 326 bytes | 0 bytes/s, done.
Total 3 (delta 0), reused 0 (delta 0)
remote: Powered by GITEE.COM [1.1.5]
remote: Set trace flag 584eae8f
To git@gitee.com:slow-heating-shaanxi-people/remote-gitcode.git
   0227f21..88528a7  master -> master
```

### 6.4拉去远程仓库到本地仓库

`git pull`命令：假设这里远端仓库现在有新的修改，我们就可以使用这个命令，将远端的修改拉取到我们本地。

 `git pull origin master:master`：其中`origin master`代表要拉取的远程的master分支，冒号后面代表要拉取到本地的分支，当本地分支名和远程分支名一致的时候可以省略掉本地分支名

 `git pull origin master`，这里pull做了两件事①将远程分支拉取到本地②合并到本地的master分支。

```bash
hello Git[mi@lavm-5wklnbmaja remote-gitcode]$ git pull origin master:master
remote: Enumerating objects: 5, done.
remote: Counting objects: 100% (5/5), done.
remote: Compressing objects: 100% (2/2), done.
remote: Total 3 (delta 1), reused 0 (delta 0), pack-reused 0
Unpacking objects: 100% (3/3), done.
From gitee.com:slow-heating-shaanxi-people/remote-gitcode
   0352d19..3ea527f  master     -> master
Warning: fetch updated the current branch head.
Warning: fast-forwarding your working tree from
Warning: commit 0352d19802fcfde0d92aa376bd09faf4eabecb02.
Already up-to-date.
[mi@lavm-5wklnbmaja remote-gitcode]$ cat file.txt 
hello remote

hello Git
```

### 6.5.gitignore文件

`.gitignore`文件是用于指定Git在版本控制中应忽略哪些文件或目录的配置文件。通过将不需要跟踪的文件或目录列入`.gitignore`文件中，可以保持代码库的整洁，并且避免提交一些临时文件、编译生成文件或敏感信息。

- .gitignore文件的语法

`.gitignore`文件的语法非常简单，由一行行规则组成。每条规则可以是文件名、目录名或带有通配符的模式。以下是语法规则的详细说明：

1. **空行和注释**：
   - 空行会被忽略，用于增加可读性。
   - 以`#`开头的行是注释。

   ```plaintext
   # 这是一个注释
   ```

2. **匹配文件或目录**：
   - 直接写文件名或目录名即可匹配对应的文件或目录。

   ```plaintext
   /log.txt         # 忽略根目录下的log.txt文件
   /temp/           # 忽略根目录下的temp目录及其所有内容
   ```

3. **通配符**：
   - `*` 匹配零个或多个字符。
   - `?` 匹配单个字符。
   - `[]` 匹配指定范围内的单个字符。

   ```plaintext
   *.log            # 忽略所有以.log结尾的文件
   temp?.txt        # 忽略所有以temp加一个字符并以.txt结尾的文件
   temp[0-9].txt    # 忽略所有以temp加数字0到9并以.txt结尾的文件
   ```

4. **目录匹配**：
   - 以斜杠`/`结尾的模式匹配目录及其所有内容。

   ```plaintext
   /build/          # 忽略根目录下的build目录及其所有内容
   ```

5. **否定模式**：
   - 以感叹号`!`开头的模式表示例外，即即使之前的规则匹配，也不忽略该文件或目录。

   ```plaintext
   *.log            # 忽略所有以.log结尾的文件
   !important.log   # 但不忽略important.log文件
   ```

6. **递归匹配**：
   - 可以使用`**`表示递归匹配任意数量的子目录。

   ```plaintext
   logs/**/debug.log  # 忽略任何子目录下的debug.log文件
   ```

### 示例

```plaintext
# 忽略所有 .log 文件
*.log

# 忽略所有 .tmp 文件
*.tmp

# 忽略 node_modules 目录
node_modules/

# 忽略 build 目录及其所有内容
/build/

# 忽略所有以 temp 开头并以 .txt 结尾的文件
temp*.txt

# 忽略 logs 目录中的所有 .log 文件，但保留 logs 目录中的 important.log 文件
logs/*.log
!logs/important.log
```

这些规则可以根据项目的具体需求进行调整，以忽略那些不需要跟踪的文件或目录。

`git check-ignore -v 文件名` ： 用于检测为什么当前文件被ignore了

## 7.标签管理

### 7.1标签有什么用？

因为我们每一次`commit`的时候都会对应一个`commit id`，但是对应的`commit id`是一串数字，不方便我们阅读和查看，所有有了标签的概念，你可以命名一个标签对应的一个`commit id`。类似于ip和域名之类的。

### 7.2如何创建标签

比如这里我们使用git tag命令直接创建一个标签：这里的含义是给最新的commit id创建一个标签叫v1.0

```bash
[mi@lavm-5wklnbmaja remote-gitcode]$ git tag v1.0
[mi@lavm-5wklnbmaja remote-gitcode]$ git tag
v1.0
```

若是我们想向特定的`commit id`创建tag

```bash
[mi@lavm-5wklnbmaja remote-gitcode]$ git log --pretty=oneline --abbrev-commit//查看当前分支的commit id
3ea527f update file.txt.
0352d19 update file.txt.
88528a7 add file.txt
0227f21 Initial commit
[mi@lavm-5wklnbmaja remote-gitcode]$ git tag v0.9 -m "测试版本" 3ea527f
//为commit id 3ea527f添加一个叫v0.9的标签，并给上注释信息“测试版本”
[mi@lavm-5wklnbmaja remote-gitcode]$ git tag
v0.9
v1.0
```

我们也可以用标签名来查看一些详细信息：

`git show 标签名`：这里可以看到对应的日期，以及注释信息，commit id等等

```bash
[mi@lavm-5wklnbmaja remote-gitcode]$ git show v0.9
tag v0.9
Tagger: slow-heating-shaanxi-people <1448554339@qq.com>
Date:   Sun Jul 28 23:12:59 2024 +0800

测试版本

commit 3ea527f253bb17e70708bc1aea065e1cb6d88e98
Author: 慢热的陕西人 <1448554339@qq.com>
Date:   Sun Jul 28 14:42:35 2024 +0000

    update file.txt.
    
    Signed-off-by: 慢热的陕西人 <1448554339@qq.com>

diff --git a/file.txt b/file.txt
index b33c40a..aeb858b 100644
--- a/file.txt
+++ b/file.txt
@@ -1,3 +1,3 @@
 hello remote
 
-hello Git
\ No newline at end of file
+hello Git
```

### 7.3删除标签

删除标签：

`git tag -d 标签名`

```bash
[mi@lavm-5wklnbmaja remote-gitcode]$ git tag -d v1.0
Deleted tag 'v1.0' (was 3ea527f)
```

### 7.4将标签推送到远程仓库

`git push origin 标签名`  ：这里将v0.9这个标签推送到远端仓库

```bash
[mi@lavm-5wklnbmaja remote-gitcode]$ git push origin v0.9
Counting objects: 1, done.
Writing objects: 100% (1/1), 188 bytes | 0 bytes/s, done.
Total 1 (delta 0), reused 0 (delta 0)
remote: Powered by GITEE.COM [1.1.5]
remote: Set trace flag 0342c69a
To git@gitee.com:slow-heating-shaanxi-people/remote-gitcode.git
 * [new tag]         v0.9 -> v0.9
```

`git push origin --tags` ：将所有的本地标签都推送到远端

```bash
[mi@lavm-5wklnbmaja remote-gitcode]$ git push origin --tags
Total 0 (delta 0), reused 0 (delta 0)
remote: Powered by GITEE.COM [1.1.5]
remote: Set trace flag ba4b4aa2
To git@gitee.com:slow-heating-shaanxi-people/remote-gitcode.git
 * [new tag]         v0.8 -> v0.8
```

**删除远端仓库的标签：**

`git push origin : 要删除的标签名` ：例如这里我们在本地删除了v0.8的标签之后将这个操作更新到远程仓库。

```bash
[mi@lavm-5wklnbmaja remote-gitcode]$ git tag -d v0.8
Deleted tag 'v0.8' (was 88528a7)
[mi@lavm-5wklnbmaja remote-gitcode]$ git push origin :v0.8 
remote: Powered by GITEE.COM [1.1.5]
remote: Set trace flag bc4e3a91
To git@gitee.com:slow-heating-shaanxi-people/remote-gitcode.git
 - [deleted]         v0.8
```

---

关于学习git命令推荐一个网站，里面有引导和图形化显示对新手非常的友好[Learn Git Branching (gitee.com)](https://help.gitee.com/learn-Git-Branching/?locale=zh_CN)
