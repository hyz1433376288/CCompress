# CCompress
- 工程文件夹中不包含图形界面的exe，您只能在本地的Qt打开CCompress文件夹，然后编译它。

- 项目工程名叫CCompress，的软件名字叫做Bandyz。Bandyz软件压缩/解压的本质是，Qt在后台开启yz线程(QProcess)，使用命令行进行压缩，当然您也可以直接使用命令行进行压缩。您可以在yz工程的main.cpp中看到命令行格式。

- Bandyz使用Qt5.9.0进行开发，CCompress是它的整个工程文件夹，您可以在您的Qt上进行编译运行。
- 您可以使用codeblock打开yz.cbp，yz工程包含了文件压缩和解压的核心算法，yz编译生成yz.exe供Qt调用

- 文件压缩以及解压的详细说明.doc是对文件压缩以及解压的详细介绍，看完这篇说明，你会对本工程了如指掌

- 如果对本项目有任何疑问，QQ：14333762788，或者在我的博客留言https://blog.houyz.cn/


