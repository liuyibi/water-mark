/*
* WatermarkImage.cpp: implementation main.
*
* Copyright (c) 2014, Optimal of CAS
* All rights reserved
*
* Filename:WatermarkImage.cpp
*
*
* Latest version:1.1
* Kang Liu
* Date:2014-11-12
*
*/

#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <afxdlgs.h>
#include "Watermark.h"
#include <vector>
#include <string>

void help()
{
	cout << "该程序用于给图像加水印并检测图像内容是否被篡改 \n"
		" \n 请按照下面的菜单进行操作：\n"<<
		"\n 操作快捷键: \n"
		"\t ESC - 结束 \n"
		"\t l - 载入图像\n"
		"\t w - 添加水印\n"
		"\t s - 保存图像\n"
		"\t t - 检测水印\n" << endl;
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	help();
	cout <<" 请您选择一张水印处理或检测的图片 \n";

Load://载入一幅图片

	string filename = " ";
	char*   filename2 = NULL;
	//filename = "..\\..\\image\\food.bmp";//这种转义双斜杠输出到DOS终端会变成单斜杠

	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT,
				NULL);

			CString strFilePath;
			if(dlg.DoModal() == IDOK)
			{
				CArray<CString, CString> aryFilename;
				POSITION posFile=dlg.GetStartPosition();
				while(posFile!=NULL)
				{
					aryFilename.Add(dlg.GetNextPathName(posFile));
				}

				if( aryFilename.GetSize() > 1 )//如果选择图片大于一幅，则不能处理，goto Load;//重新选择载入图片
				{
					cout<< "只能选择一幅图片！" <<endl;
					goto Load;//重新选择载入图片
				}
				strFilePath = aryFilename.GetAt(0);//获得选择的图片路径

				cout << "图片载入成功！"<<endl;
				//注：CString 转换 char*
				filename2 = strFilePath.GetBuffer(strFilePath.GetLength());

				//注：CString 转换 string
				//1.string filename(strFilePath.GetBuffer());//这是局部变量，外部不能引用，应该用第二种
				//2. 注： cout 的 << 重载了string, 若printf 的话必须 printf("%s", str.c_str()); 
				// 而不可 print( "%s", str ); 因为 str 是个 string 类
				filename = std::string(filename2);
			}
			cout << filename <<endl;

			if ( " " == filename )
			{
				cout << "您没有选择任何图片，请载入一张图片！";
				cout << endl;
				goto Load;//重新选择载入图片
			}

			Mat image = imread( filename, 1 );
			if( image.empty() )
			{
				cout << "\n Durn, couldn't read image filename " << filename << endl;
				return 1;
			}

			//图像的行、列不能弄错
			int height=0;
			int width=0;
			height=image.rows;
			width=image.cols;
			int imagesz= height * width;

			//实例化一个应用对象
			WatermarkApplication gcapp(height, width, imagesz, filename2);

			//主要逻辑步骤
			const string winName = "Image";
			cvNamedWindow( winName.c_str(), CV_WINDOW_AUTOSIZE );
			gcapp.setImageAndWinName( image, winName );
			gcapp.showImage();
			int save_tag = 0;

			for(;;)
			{
				int c = cvWaitKey(0);
				switch( (char) c )
				{
				case '\x1b':
					cout << "结束退出..." << endl;
					goto exit_main;

				case 'l':
					cout << "加载一幅图片..." << endl;
					goto Load;

				case 'w'://加水印

					clock_t nTimeStart;      //计时开始
					clock_t nTimeStop;       //计时结束
					nTimeStart = clock();    //
					cout << "图片添加水印信息..." << endl;
					gcapp.watermarkImage();
					cout << "图片添加水印信息结束" << endl;
					nTimeStop = clock();
					cout <<"加水印耗时："<<(double)(nTimeStop - nTimeStart)/CLOCKS_PER_SEC<<"秒"<< endl;
					break;

				case 's'://保存图
					cout << "保存有水印图片..." << endl;
					save_tag = gcapp.saveImage();
					if(save_tag)
					{
						cout << "保存有水印图片成功" << endl;
					}
					else
					{
						cout << "保存有水印图片失败" << endl;
					}
					break;

				case 't'://检测水印
					cout << "检测图片水印是否篡改..." << endl;
					int revised_tag = gcapp.testWatermark();
					if(revised_tag)
					{
						cout << "检测图片水印结束" << endl;
						cout << "该图片 有 被修改过！" << endl;
					}
					else
					{
						cout << "检测图片水印结束" << endl;
						cout << "该图片 没有 被修改过！" << endl;
					}
					break;
				}
			}

exit_main:
			cvDestroyWindow( winName.c_str() );
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}
	return nRetCode;
}