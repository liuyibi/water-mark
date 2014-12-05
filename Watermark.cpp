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
	return abs(n%2);//求余方法

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
	image->copyTo( TestShowres );
	winName = &_winName;

}


//计算颜色信息关系
double WatermarkApplication::compColorInfo( double R, double G, double B )
{
	R = R - isood( int(R) );
	G = G - isood( int(G) );
	B = B - isood( int(B) );

	double Temp = abs( R + G - B );//颜色和差绝对值
	double ColorInfos = pow( Temp,2 ); //求平方

	return ColorInfos;
}


//计算四舍五入
double RoundEx(const double& dInput)
{
	double dIn = dInput;
	double dOut;
	dOut = double(int(dIn + 0.5));
	return dOut;

}


//计算位置信息
double WatermarkApplication::compPositionInfo( double x, double y )
{
	//double PositionInfo = abs( x - y );//位置纵横坐标绝对差,这个不行，线性平移不能克服原图复制粘贴；
	//简单的平移和平方都会有很大概率出现某个方向的奇偶规律

	//注意：位置信息函数设计准则：让坐标值通过函数后，位置信息坐标系没有任何方向的奇偶规律
	double PositionInfo =  sqrt( 1000 + (x-y) + pow( pow(x-y,2) + 53 ,2 ) + pow( (x-y + 7),2 ) + pow(( x - y),3 ) + pow( x+13 ,2 ) + pow( y+29,3 ) ) ;

	return RoundEx(PositionInfo);
}


//给图像加水印
void WatermarkApplication::watermarkImage()
{
	for(int i=0;i<Height;i++)
	{
		for(int j=0;j<Width;j++)
		{	
			//获取颜色映射信息
			double B = Tempres.at<Vec3b>( i,j )(0);
			double G = Tempres.at<Vec3b>( i,j )(1);
			double R = Tempres.at<Vec3b>( i,j )(2);
			double ColorInfos = compColorInfo( R, G, B );
			//获取坐标映射信息
			double PositionInfo = compPositionInfo( double(i), double(j) );


			double HashCode = ColorInfos + PositionInfo;//与各通道最后一位做异或运算

			//最后一位做异或运算,得到的值赋予最后一位
			//B通道
			int XOR_Btag = isood( int(HashCode) );
			B = B - isood( int(B) ) + double(XOR_Btag);
			Tempres.at<Vec3b>( i,j )(0) = int(B);

			//G通道
			int XOR_Gtag = isood( int(HashCode) );
			G = G - isood( int(G) ) + double(XOR_Gtag);
			Tempres.at<Vec3b>( i,j )(1) = int(G);

			//R通道
			int XOR_Rtag = isood( int(HashCode) );
			R = R - isood( int(R) ) + double(XOR_Rtag);
			Tempres.at<Vec3b>( i,j )(2) = int(R);

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
			//获取颜色映射信息
			double B = Tempres.at<Vec3b>( i,j )(0);
			double G = Tempres.at<Vec3b>( i,j )(1);
			double R = Tempres.at<Vec3b>( i,j )(2);
			double ColorInfos = compColorInfo( R, G, B );

			//获取坐标映射信息
			double PositionInfo = compPositionInfo( double(i), double(j) );
			double HashCode = ColorInfos + PositionInfo;//与各通道最后一位做异或运算

			//最后一位做异或运算,检测是否有变化
			//B通道
			int XOR_Btag = ( isood( int(HashCode) ) ^ isood( int(B) ) );
			//cout << XOR_Btag << endl;

			//G通道
			int XOR_Gtag = ( isood( int(HashCode) ) ^ isood( int(G) ) );


			//R通道
			int XOR_Rtag = ( isood( int(HashCode) ) ^ isood( int(R) ) );

			//如果有变化，异或为1；没有变化，异或为0。
			if( XOR_Btag || XOR_Gtag || XOR_Rtag )
			{
				//判断边界
				if( 0<i && i< Height-1 && 0<j && j< Width-1 )
				{
					//检测到的目标点邻域着红色，并且8邻域都标出，使得明显
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
