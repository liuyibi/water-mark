// Watermark.cpp : �������̨Ӧ�ó������ڵ㡣
/*
*
* Copyright (c) 2014, Optimal of CAS
* All rights reserved
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
#include "Watermark.h"

using namespace std;
using namespace cv;


int isood(int n)//�ж���������ż��
{
	//ʵ����֤�����ֶ����ԣ�ʱ���࣬��ͼƬ�����й�
	return abs(n%2);//���෽��

	/*//λ������
	unsigned char b=1;
	unsigned char a=n;
	unsigned char d = b & a;
	return int(d);*/

}


WatermarkApplication::WatermarkApplication(const int height, const int width, const int imagesz, char* filename)
{
	this->Height = height;
	this->Width = width; 
	this->Imagesz = imagesz;
	this->Filename = filename;
}


//����ͼ�񴰿ڣ�����ĳ�Ա������ֵ����
void WatermarkApplication::setImageAndWinName( const Mat& _image, const string& _winName  )
{
	if( _image.empty() || _winName.empty() )
		return;
	image = &_image;//ͼ��ָ��image���������ͼ������ֱ��Ϊԭ���󣬲���������
	image->copyTo( Tempres );
	image->copyTo( TestShowres );
	winName = &_winName;

}


//������ɫ��Ϣ��ϵ
double WatermarkApplication::compColorInfo( double R, double G, double B )
{
	R = R - isood( int(R) );
	G = G - isood( int(G) );
	B = B - isood( int(B) );

	double Temp = abs( R + G - B );//��ɫ�Ͳ����ֵ
	double ColorInfos = pow( Temp,2 ); //��ƽ��

	return ColorInfos;
}


//������������
double RoundEx(const double& dInput)
{
	double dIn = dInput;
	double dOut;
	dOut = double(int(dIn + 0.5));
	return dOut;

}


//����λ����Ϣ
double WatermarkApplication::compPositionInfo( double x, double y )
{
	//double PositionInfo = abs( x - y );//λ���ݺ�������Բ�,������У�����ƽ�Ʋ��ܿ˷�ԭͼ����ճ����
	//�򵥵�ƽ�ƺ�ƽ�������кܴ���ʳ���ĳ���������ż����

	//ע�⣺λ����Ϣ�������׼��������ֵͨ��������λ����Ϣ����ϵû���κη������ż����
	double PositionInfo =  sqrt( 1000 + (x-y) + pow( pow(x-y,2) + 53 ,2 ) + pow( (x-y + 7),2 ) + pow(( x - y),3 ) + pow( x+13 ,2 ) + pow( y+29,3 ) ) ;

	return RoundEx(PositionInfo);
}


//��ͼ���ˮӡ
void WatermarkApplication::watermarkImage()
{
	for(int i=0;i<Height;i++)
	{
		for(int j=0;j<Width;j++)
		{	
			//��ȡ��ɫӳ����Ϣ
			double B = Tempres.at<Vec3b>( i,j )(0);
			double G = Tempres.at<Vec3b>( i,j )(1);
			double R = Tempres.at<Vec3b>( i,j )(2);
			double ColorInfos = compColorInfo( R, G, B );
			//��ȡ����ӳ����Ϣ
			double PositionInfo = compPositionInfo( double(i), double(j) );


			double HashCode = ColorInfos + PositionInfo;//���ͨ�����һλ���������

			//���һλ���������,�õ���ֵ�������һλ
			//Bͨ��
			int XOR_Btag = isood( int(HashCode) );
			B = B - isood( int(B) ) + double(XOR_Btag);
			Tempres.at<Vec3b>( i,j )(0) = int(B);

			//Gͨ��
			int XOR_Gtag = isood( int(HashCode) );
			G = G - isood( int(G) ) + double(XOR_Gtag);
			Tempres.at<Vec3b>( i,j )(1) = int(G);

			//Rͨ��
			int XOR_Rtag = isood( int(HashCode) );
			R = R - isood( int(R) ) + double(XOR_Rtag);
			Tempres.at<Vec3b>( i,j )(2) = int(R);

		}
	}
}


//������ˮӡͼƬ
int WatermarkApplication::saveImage()
{
	char * TrimapName = "WatermarkedImage.bmp";//.bmp������ѹ��
	IplImage Trimap = Tempres;
	IplImage * TrimapPoint = &Trimap;
	const int params[2]={CV_IMWRITE_JPEG_QUALITY,100};
	int save_tag = cvSaveImage(TrimapName,TrimapPoint,params);
	//imwrite(TrimapName,Tempres);

	return save_tag;
}


//��⺬��ˮӡͼƬ�Ƿ��޸Ĺ�
int WatermarkApplication::testWatermark()
{
	int revised_tag = 0;

	for(int i=0;i<Height;i++)
	{
		for(int j=0;j<Width;j++)
		{	
			//��ȡ��ɫӳ����Ϣ
			double B = Tempres.at<Vec3b>( i,j )(0);
			double G = Tempres.at<Vec3b>( i,j )(1);
			double R = Tempres.at<Vec3b>( i,j )(2);
			double ColorInfos = compColorInfo( R, G, B );

			//��ȡ����ӳ����Ϣ
			double PositionInfo = compPositionInfo( double(i), double(j) );
			double HashCode = ColorInfos + PositionInfo;//���ͨ�����һλ���������

			//���һλ���������,����Ƿ��б仯
			//Bͨ��
			int XOR_Btag = ( isood( int(HashCode) ) ^ isood( int(B) ) );
			//cout << XOR_Btag << endl;

			//Gͨ��
			int XOR_Gtag = ( isood( int(HashCode) ) ^ isood( int(G) ) );


			//Rͨ��
			int XOR_Rtag = ( isood( int(HashCode) ) ^ isood( int(R) ) );

			//����б仯�����Ϊ1��û�б仯�����Ϊ0��
			if( XOR_Btag || XOR_Gtag || XOR_Rtag )
			{
				//�жϱ߽�
				if( 0<i && i< Height-1 && 0<j && j< Width-1 )
				{
					//��⵽��Ŀ��������ź�ɫ������8���򶼱����ʹ������
					for( int local_i=i-1; local_i<i+2; local_i++ )
					{
						for( int local_j=j-1; local_j<j+2; local_j++ )
						{
							TestShowres.at<Vec3b>( local_i,local_j )(0) = 0;
							TestShowres.at<Vec3b>( local_i,local_j )(1) = 0;
							TestShowres.at<Vec3b>( local_i,local_j )(2) = 255;
						}
					}
				}
				revised_tag = 1;
			}

		}
	}

	Tempres = TestShowres;

	//��ʾ�ָ���ͼ��
	imshow( *winName, Tempres );//��ʾͼ�� *winName����ʾ��������ǩ; Tempres:Ҫ��ʾ�Ķ���

	return revised_tag;
}


//��ʾͼƬ
void WatermarkApplication::showImage()
{
	if( image->empty() || winName->empty() )
		return;

	image->copyTo( Tempres );//��ͼƬ���󿽱���Tempres

	//��ʾ�����ͼ��
	imshow( *winName, Tempres );//��ʾͼ�� *winName����ʾ��������ǩ; Tempres:Ҫ��ʾ�Ķ���

}
