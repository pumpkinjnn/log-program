# log-program

#log模块

**需求：**
- log文件有固定数量，固定大小
- log文件时效性
- 可能被同时调用
- 时间问题：
 - 记录下一次开始写的时候，不能依靠时间
 - 提供两个接口，一个原始，一个带时间戳

**设计**
名称：`dailylog`，记录日常信息

1.根据用户输入目录地址检查目录，不能正常访问则不记录，若能成功访问，则根据提供的filename及log文件数量要求从filename.log.1书写文件到filename.log.n。

2.若中途遇到无权限文件则跳过，遇到不存在文件则创建新文件。

3.每次在文件头部记录一个access_time, 每次书写新文件时增大1。所以，退出后重新书写该目录下log文件时，则从access_time最大的文件开始书写。

4.用户使用时，使用`new_dailylog（params）`创建dailylog结构，书写时给两个参数：dailylog指针和本条log内容，使用完毕后，使用给定函数`free_dailylog(dailylog *handle)`释放dailylog结构。

**.h文件介绍**
包含三个函数：
```
/*
 dir: 记录log目录名称
 name: 记录log文件名称
 num: 记录log文件数量
 size: 一个log文件的大小上限，kb
 b_need_time: 是否需要时间戳，1代表需要，0为不需要
*/
dailylog* new_dailylog(const char *dir,const char *name,int num,int size,int b_need_time);

//用户给info，书写一条log信息
void  write_dailylog(dailylog *handle, const char *info);

//释放当前dailylog结构，代表当次书写结束
void free_dailylog(dailylog *handle);
```

#log program

**Needs：**
- log files are fixed in numbers and sizes
- log files have time restrictions
- a single files can be written at the same time
- Time Related Issue：
 - When deciding where(which file) to start when rerunning the program, time is not a reliable indicator
 - Offer two interfaces, one with time recoded, another without

**Design**
Name：`dailylog`

1.Check directory according to the input dir，if dir is not valid，then the program does not perform, nor it reports error.
  Else，it logs messages into provided filename (from filename.log.1 to filename.log.n), n provided by user.

2.If anyfile during the process have restricted permission，we skip this file.
  If anyfile during the process is not available, we create it.

3.Every time record access_time at the head of a file, when starting a new file, access_time increase by 1.

4.When using this program，use `new_dailylog（params）` to initialize the structure，
  with 2 parameters：dailylog's handle and message to be loged，
  after usage, use `free_dailylog(dailylog *handle)` to free.

**.h overview**
Includes three functions：
```
/*
 dir: directory
 name: name of the log files
 num: the maximum number of log files
 size: the maximum size of a log file, in kb
 b_need_time: if need time recoded, true for adding time
*/
dailylog* new_dailylog(const char *dir,const char *name,int num,int size,int b_need_time);

//get info(message) from user, write into file
void  write_dailylog(dailylog *handle, const char *info);

//free struct, end writing
void free_dailylog(dailylog *handle);
```
#log プログラム

**要求：**
- log ファイルはネームもサイズも規定されている
- log ファイルは時間に限る
- 一つのファイルは当時に二つのプログラムに書かれる可能性がある
- 时间問題：
 - 次に書くファイルを決めるときは、時間に頼らない
 - 時間付きと時間切りバージョン二つある

**デザイン**
名前：`dailylog`

1.ユーサーから提供したファイルの名前やサイズでファイルを作成する，権限のないときは記録しない，エラーもつくらない。

2.権限のないファイルはスキップ，存在しないファイルは作る。

3.ファイルの初めにaccess_timeを書き, 新しく書くときはプラス1。

4.使うとには，`new_dailylog（params）`でストラクチャを作る，`free_dailylog(dailylog *handle)`はdailylogストラクチャをフリーする。

**.hファイル紹介**

```
/*
 dir: ファイルのアドレス
 name: ファイルの名前
 num: ファイルの数
 size: ファイルのサイズ、KB
 b_need_time: 時間を書く設定、trueは書く
*/
dailylog* new_dailylog(const char *dir,const char *name,int num,int size,int b_need_time);

//ユーサーからメッセージをもらい、ファイルに書く
void  write_dailylog(dailylog *handle, const char *info);

//dailylogストラクチャをフリーする、書き終わり
void free_dailylog(dailylog *handle);
