##数字水印检测##


2014-12-05

检测水印函数设计，综合考虑了时间、空间复杂度和检测定位鲁棒性。

f函数算法步奏：
1.	|R+G-B|^2 (考虑颜色信息关系，非线性的映射，平方比开方速度快)；
2.	|P(x)-P(y)| (考虑坐标位置信息)；
3.	1.+2.的最低位分别赋予R, G, B通道的最低位；
4.	检测水印的时候，重复1.，2.，3.步骤，得到的最低位与图片最低位做异或运算，检测是否发生改变。

实验证明：
//double PositionInfo = abs( x - y );//位置纵横坐标绝对差,这个不行，线性平移不能克服原图复制粘贴；简单的平移和平方都会有很大概率出现某个方向的奇偶规律


//注意：位置信息函数设计准则：让坐标值通过函数后，位置信息坐标系没有任何方向的奇偶规律，因为，赋予颜色通道最低位的值，只有0或者1，两种，所以，如果出现奇偶规律，那么平移等简单操作将会失效。因此该函数不唯一，可以任意拼凑，但要考虑时间复杂度。
例如，位置信息设计为： double PositionInfo =  sqrt( 1000 + (x-y) + pow( pow(x-y,2) + 53 ,2 ) + pow( (x-y + 7),2 ) + pow(( x - y),3 ) + pow( x+13 ,2 ) + pow( y+29,3 ) ) ;

因此，f函数算法步骤修改为：
	|R+G-B|^2 (考虑颜色信息关系，非线性的映射，平方比开方速度快)；
	double PositionInfo =  sqrt( 1000 + (x-y) + pow( pow(x-y,2) + 53 ,2 ) + pow( (x-y + 7),2 ) + pow(( x - y),3 ) + pow( x+13 ,2 ) + pow( y+29,3 ) ) ; (考虑坐标位置信息)；
	1.+2.的最低位分别赋予R, G, B通道的最低位；
	检测水印的时候，重复1.，2.，3.步骤，得到的最低位与图片最低位做异或运算，检测是否发生改变。
通过这种颜色信息非线性、位置信息较高阶非线性的函数设计，可以对图像的多种篡改都有很好的适应性和鲁棒性，比如：描点、划线、异图粘贴、同图复制粘贴等操作，都有很强的检测性能。
如200万像素的图片，加水印操作在普通PC机约需要2-3秒。



注：每个颜色通道的像素值只考虑前7位，把最后一位作为一个验证码，进行检测；因为生成水印的像素值与原图有差异，所以在检测水印时读取的像素值不是原图的颜色值了，所以检测是会有问题的，因此在R,G,B计算的时候一律去除末尾的值，把末尾考虑为校验码，用来检测最后一位就可以了。



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
