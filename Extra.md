# 扩展功能

> 扩展需求文档

## Google Test (ok)

1. 使用 Google Test 进行测试

2. 测试 CRUD 相关函数

3. 额外自定义 2 - 3 个测试

## Agenda Storage IO以及Agenda Service的日志功能 (ok)

1. 编写一个单例的LOG类，在Agenda服务运行的时候记录StorageIO和Service的操作以供错误追踪。

2. 要求把操作的时间用户记录下，并写入文件方便查阅。

3. 高级要求：限定log文件的大小，如每10MB会分割log文件。(ok)

## 中断信号捕获与处理 (ok)

1. 捕获从键盘输入的信号如“CTRL + C”程序终止信号，让Agenda就收到终止信号后会先把记录写入文件保持好数据在终止程序而不是直接终止。

## 异常处理以及错误信息返回 (ok)

1. 在阶段二中创建会议出错是直接返回“error”，用户并不知道是为什么创建失败。

2. 在UI层采用try catch机制捕获从AgendaServer中抛出的异常，再处理反馈给用户。