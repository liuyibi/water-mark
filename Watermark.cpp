// Watermark.cpp : 定义控制台应用程序的入口点。
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


int isood(int n)//判断奇数或者偶数
{
	//实验验证正两种都可以，时间差不多，和图片内容有关
	return n%2;//求余方法

	/*//位与运算
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


//载入图像窗口，给类的成员变量赋值而已
void WatermarkApplication::setImageAndWinName( const Mat& _image, const string& _winName  )
{
	if( _image.empty() || _winName.empty() )
		return;
	image = &_image;//图像指针image引用载入的图像，引用直接为原对象，不拷贝对象
	image->copyTo( Tempres );
	winName = &_winName;

}


//给图像加水印
void WatermarkApplication::watermarkImage()
{
	for(int i=0;i<Height;i++)
	{
		for(int j=0;j<Width;j++)
		{	
			if(isood(Tempres.at<Vec3b>( i,j )(0)))
			{
				//cout<<"该处被加入水印！"<<endl;
				int fistvalue = Tempres.at<Vec3b>( i,j )(0);
				Tempres.at<Vec3b>( i,j )(0) = fistvalue-1;
			}
			if(isood(Tempres.at<Vec3b>( i,j )(1)))
			{
				//cout<<"该处被加入水印！"<<endl;
				int secondvalue = Tempres.at<Vec3b>( i,j )(1);
				Tempres.at<Vec3b>( i,j )(1) = secondvalue-1;
			}
			if(isood(Tempres.at<Vec3b>( i,j )(2)))
			{
				//cout<<"该处被加入水印！"<<endl;
				int thirdvalue = Tempres.at<Vec3b>( i,j )(2);
				Tempres.at<Vec3b>( i,j )(2) = thirdvalue-1;
			}
		}
	}
}


//保存有水印图片
int WatermarkApplication::saveImage()
{
	char * TrimapName = "WatermarkedImage.bmp";//.bmp是无损压缩
	IplImage Trimap = Tempres;
	IplImage * TrimapPoint = &Trimap;
	const int params[2]={CV_IMWRITE_JPEG_QUALITY,100};
	int save_tag = cvSaveImage(TrimapName,TrimapPoint,params);
	//imwrite(TrimapName,Tempres);

	return save_tag;
}


//检测含有水印图片是否被修改过
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
				//cout<<"该图像内容有改动，如图红色区域！"<<endl;
				//cout<<i<<" "<<j<<endl;
				Tempres.at<Vec3b>( i,j )(0) = 0;
				Tempres.at<Vec3b>( i,j )(1) = 0;
				Tempres.at<Vec3b>( i,j )(2) = 255;

				revised_tag = 1;
			}
		}
	}
	//显示分割后的图像
	imshow( *winName, Tempres );//显示图像 *winName：显示窗口名标签; Tempres:要显示的对象

	return revised_tag;
}


//显示图片
void WatermarkApplication::showImage()
{
	if( image->empty() || winName->empty() )
		return;

	image->copyTo( Tempres );//把图片矩阵拷贝给Tempres

	//显示检测后的图像
	imshow( *winName, Tempres );//显示图像 *winName：显示窗口名标签; Tempres:要显示的对象

}
