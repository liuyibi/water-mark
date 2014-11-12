/*
* Watermark.h: interface for the WatermarkApplication class.
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

#ifndef WATERMARK_H
#define WATERMARK_H

#include <iostream>
#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

//////////////////////////////////////////////////////////////////////////////////////////
//定义WatermarkApplication应用类
//////////////////////////////////////////////////////////////////////////////////////////
class WatermarkApplication
{

public:

	//传入重要参数，这里是原始图像的高、宽、像素个数
	WatermarkApplication(const int height, const int width,const int imagesz,  char* filename);
	void setImageAndWinName( const Mat& _image, const string& _winName );
	void showImage();
	void watermarkImage();//给图像加水印
	int saveImage();//保存加载水印图片
	int testWatermark();//检测水印

private:

	const string* winName;
	int Height;
	int Width; 
	int Imagesz;
	char*  Filename;//传递图像名
	const Mat* image;   
	Mat Tempres; //暂时存储图像信息

};

#endif
