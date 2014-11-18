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
//����WatermarkApplicationӦ����
//////////////////////////////////////////////////////////////////////////////////////////
class WatermarkApplication
{

public:

	//������Ҫ������������ԭʼͼ��ĸߡ������ظ���
	WatermarkApplication(const int height, const int width,const int imagesz,  char* filename);
	void setImageAndWinName( const Mat& _image, const string& _winName );
	
	double compColorInfo( double R, double G, double B );//������ɫ��Ϣ��ϵ
	double compPositionInfo( double x, double y );//����λ����Ϣ
	
	void showImage();
	void watermarkImage();//��ͼ���ˮӡ
	int saveImage();//�������ˮӡͼƬ
	int testWatermark();//���ˮӡ

private:

	const string* winName;
	int Height;
	int Width; 
	int Imagesz;
	char*  Filename;//����ͼ����
	const Mat* image;   
	Mat Tempres; //��ʱ�洢ͼ����Ϣ
	Mat TestShowres;//���ˮӡʱ��ʾ,��Ϊ�ڼӴ���ɫ��ʱ���ı�Tempres����ɫֵ��
	//���԰�Tempres����ԭʼ������������TestShowres��Ϊ��ʱ�Ľ����޸�

};

#endif
