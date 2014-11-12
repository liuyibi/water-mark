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
	return n%2;//���෽��

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
	winName = &_winName;

}


//��ͼ���ˮӡ
void WatermarkApplication::watermarkImage()
{
	for(int i=0;i<Height;i++)
	{
		for(int j=0;j<Width;j++)
		{	
			if(isood(Tempres.at<Vec3b>( i,j )(0)))
			{
				//cout<<"�ô�������ˮӡ��"<<endl;
				int fistvalue = Tempres.at<Vec3b>( i,j )(0);
				Tempres.at<Vec3b>( i,j )(0) = fistvalue-1;
			}
			if(isood(Tempres.at<Vec3b>( i,j )(1)))
			{
				//cout<<"�ô�������ˮӡ��"<<endl;
				int secondvalue = Tempres.at<Vec3b>( i,j )(1);
				Tempres.at<Vec3b>( i,j )(1) = secondvalue-1;
			}
			if(isood(Tempres.at<Vec3b>( i,j )(2)))
			{
				//cout<<"�ô�������ˮӡ��"<<endl;
				int thirdvalue = Tempres.at<Vec3b>( i,j )(2);
				Tempres.at<Vec3b>( i,j )(2) = thirdvalue-1;
			}
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
			if(isood(Tempres.at<Vec3b>( i,j )(0))||isood(Tempres.at<Vec3b>( i,j )(1))||
				isood(Tempres.at<Vec3b>( i,j )(2)))
			{
				//cout<<isood(Tempres.at<Vec3b>( i,j )(0))<<endl;
				//cout<<"��ͼ�������иĶ�����ͼ��ɫ����"<<endl;
				//cout<<i<<" "<<j<<endl;
				Tempres.at<Vec3b>( i,j )(0) = 0;
				Tempres.at<Vec3b>( i,j )(1) = 0;
				Tempres.at<Vec3b>( i,j )(2) = 255;

				revised_tag = 1;
			}
		}
	}
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
