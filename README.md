##数字水印检测##

2014-11-13
1.显示窗口可以自动调节大小；
2.应用采样方法加水印（选择在RGB的一个颜色通道上加，如中间通道；隔行或者隔列）加水印；
3.现在的程序版本不仅在选择中间通道，并且隔行隔列采样，一幅200万像素图像上时间约0.13秒；


vs2012上配置

配置OpenCV，可以参照这两个博文配置：[Opencv 完美配置攻略 2014 (Win8.1 + Opencv 2.4.8 + VS 2013)](http://my.phirobot.com/blog/2014-02-opencv_configuration_in_vs.html), [实用图像处理入门 - 1 - opencv VS2012 环境搭建](http://www.cnblogs.com/jhzhu/p/3175832.html).

配置完成如果出现找不到opencv的dll文件，可以重启一下IDE试试。

编译中的几个问题：

- VS2012 编译程序时报无法加载PDB文件错误解决方案，可以参阅[这里](http://blog.csdn.net/yihaiyiren/article/details/11493553)
- 编译事出现`error Building MFC application with /MD[d] (CRT dll version) requires MFC shared dll version. Please #define _AFXDLL or do not use /MD[d]`错误，可以参阅这里：[Visual Studio编译错误](http://buptdtt.blog.51cto.com/2369962/542663)。

关于char* filename2初始化的问题：

- 可以参阅[不能将参数从“const char *”转换为“LPCWSTR](http://blog.163.com/it_codexgigas/blog/static/2179963132013230917969)和这个[帖子](http://bbs.csdn.net/topics/310146539)