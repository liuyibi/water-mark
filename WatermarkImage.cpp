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
	cout << "�ó������ڸ�ͼ���ˮӡ�����ͼ�������Ƿ񱻴۸� \n"
		" \n �밴������Ĳ˵����в�����\n"<<
		"\n ������ݼ�: \n"
		"\t ESC - ���� \n"
		"\t l - ����ͼ��\n"
		"\t w - ���ˮӡ\n"
		"\t s - ����ͼ��\n"
		"\t t - ���ˮӡ\n" << endl;
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	help();
	cout <<" ����ѡ��һ��ˮӡ��������ͼƬ \n";

Load://����һ��ͼƬ

	string filename = " ";
	char*   filename2 = NULL;
	//filename = "..\\..\\image\\food.bmp";//����ת��˫б�������DOS�ն˻��ɵ�б��

	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: ���Ĵ�������Է���������Ҫ
			_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
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

				if( aryFilename.GetSize() > 1 )//���ѡ��ͼƬ����һ�������ܴ���goto Load;//����ѡ������ͼƬ
				{
					cout<< "ֻ��ѡ��һ��ͼƬ��" <<endl;
					goto Load;//����ѡ������ͼƬ
				}
				strFilePath = aryFilename.GetAt(0);//���ѡ���ͼƬ·��

				cout << "ͼƬ����ɹ���"<<endl;
				//ע��CString ת�� char*
				filename2 = strFilePath.GetBuffer(strFilePath.GetLength());

				//ע��CString ת�� string
				//1.string filename(strFilePath.GetBuffer());//���Ǿֲ��������ⲿ�������ã�Ӧ���õڶ���
				//2. ע�� cout �� << ������string, ��printf �Ļ����� printf("%s", str.c_str()); 
				// ������ print( "%s", str ); ��Ϊ str �Ǹ� string ��
				filename = std::string(filename2);
			}
			cout << filename <<endl;

			if ( " " == filename )
			{
				cout << "��û��ѡ���κ�ͼƬ��������һ��ͼƬ��";
				cout << endl;
				goto Load;//����ѡ������ͼƬ
			}

			Mat image = imread( filename, 1 );
			if( image.empty() )
			{
				cout << "\n Durn, couldn't read image filename " << filename << endl;
				return 1;
			}

			//ͼ����С��в���Ū��
			int height=0;
			int width=0;
			height=image.rows;
			width=image.cols;
			int imagesz= height * width;

			//ʵ����һ��Ӧ�ö���
			WatermarkApplication gcapp(height, width, imagesz, filename2);

			//��Ҫ�߼�����
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
					cout << "�����˳�..." << endl;
					goto exit_main;

				case 'l':
					cout << "����һ��ͼƬ..." << endl;
					goto Load;

				case 'w'://��ˮӡ

					clock_t nTimeStart;      //��ʱ��ʼ
					clock_t nTimeStop;       //��ʱ����
					nTimeStart = clock();    //
					cout << "ͼƬ���ˮӡ��Ϣ..." << endl;
					gcapp.watermarkImage();
					cout << "ͼƬ���ˮӡ��Ϣ����" << endl;
					nTimeStop = clock();
					cout <<"��ˮӡ��ʱ��"<<(double)(nTimeStop - nTimeStart)/CLOCKS_PER_SEC<<"��"<< endl;
					break;

				case 's'://����ͼ
					cout << "������ˮӡͼƬ..." << endl;
					save_tag = gcapp.saveImage();
					if(save_tag)
					{
						cout << "������ˮӡͼƬ�ɹ�" << endl;
					}
					else
					{
						cout << "������ˮӡͼƬʧ��" << endl;
					}
					break;

				case 't'://���ˮӡ
					cout << "���ͼƬˮӡ�Ƿ�۸�..." << endl;
					int revised_tag = gcapp.testWatermark();
					if(revised_tag)
					{
						cout << "���ͼƬˮӡ����" << endl;
						cout << "��ͼƬ �� ���޸Ĺ���" << endl;
					}
					else
					{
						cout << "���ͼƬˮӡ����" << endl;
						cout << "��ͼƬ û�� ���޸Ĺ���" << endl;
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
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: GetModuleHandle ʧ��\n"));
		nRetCode = 1;
	}
	return nRetCode;
}