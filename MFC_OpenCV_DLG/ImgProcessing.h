#pragma once

#include <opencv2\opencv.hpp>
#include "cv.h"
#include "highgui.h"
#include "afxwin.h"



using namespace cv;
using namespace std;

typedef struct vector
{
	int x;
	int y;
}MotionVect, *pMotionVect;


class CImgProcessing
{
private:
	CvRNG m_rng;
public:
	unsigned int Edge_Mode;

	CImgProcessing();
	/**
	*	function:		IplImage2hBitmap
	*	description:	将IplImage格式转换成HBITMAP格式
	*	parameter:		IplImage* pImg:IplImage图像指针
	*	return:			HBITMAP:位图图像指针
	**/
	HBITMAP IplImage2hBitmap(IplImage* pImg);

	/**
	*	function:		IplImage2hBitmap
	*	description:	将IplImage格式转换成指定尺寸HBITMAP格式
	*	parameter:		IplImage* pImg:IplImage图像指针
	nWidth, nHeight:输出位图宽度高度
	*	return:			HBITMAP:位图图像指针
	**/
	HBITMAP IplImage2hBitmap(IplImage* pImg, int nWidth, int nHeight);

	/**
	*	function:		FillBitmapInfo
	*	description:	将IplImage格式转换成指定尺寸BITMAPINFO格式
	*	parameter:		IplImage* pImg:IplImage图像指针
	width, height:图像宽度高度
	bpp:图像位深度
	*	return:			BITMAPINFO*:位图图像指针
	**/
	BITMAPINFO* FillBitmapInfo(int width, int height, int bpp);

	void BGR2YUV(IplImage *pImgBGR, IplImage *pImgY, IplImage *pImgU, IplImage *pImgV);
	void YUV2BGR(IplImage *pImgBGR, IplImage *pImgY, IplImage *pImgU, IplImage *pImgV);

	/**
	*	function:		AddGuassianNoise
	*	description:	添加高斯噪声
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	fNoisePSNR:加噪后图像的峰值信噪比
	*	return:			bool值:是否处理成功
	**/
	bool AddGuassianNoise(IplImage *pInputImg, IplImage *pOutputImg, float fNoisePSNR);

	/**
	*	function:		AddPepperSaltNoise
	*	description:	添加椒盐噪声
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	fNoisePSNR:加噪后图像的峰值信噪比
	*	return:			bool值:是否处理成功
	**/
	bool AddPepperSaltNoise(IplImage *pInputImg, IplImage *pOutputImg, float fNoisePSNR);


	/**
	*	function:		GrayReversion
	*	description:	灰度反转函数
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	*	return:			bool值:是否处理成功
	**/
	bool GrayReversion(IplImage *pInputImg, IplImage *pOutputImg);

	/**
	*	function:		LogCorrection
	*	description:	图像对数变换函数 s = c*log(1 + r)
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	fConst:对数变换常数
	*	return:			bool值:是否处理成功
	**/
	bool LogCorrection(IplImage *pInputImg, IplImage *pOutputImg, float fConst);

	/**
	*	function:		RevLogCorrection
	*	description:	图像反对数变换函数 r = c*log(1 + s)  s = exp(r/c) - 1
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	fConst:对数变换常数
	*	return:			bool值:是否处理成功
	**/
	bool RevLogCorrection(IplImage *pInputImg, IplImage *pOutputImg, float fConst);

	/**
	*	function:		LogCorrection_Table
	*	description:	(查表)图像对数变换函数 s = c*log(1 + r)
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	fConst:对数变换常数
	*	return:			bool值:是否处理成功
	**/
	bool LogCorrection_Table(IplImage *pInputImg, IplImage *pOutputImg, float fConst);

	/**
	*	function:		RevLogCorrection_Table
	*	description:	(查表)图像反对数变换函数 r = c*log(1 + s)  s = exp(r/c) - 1
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	fConst:对数变换常数
	*	return:			bool值:是否处理成功
	**/
	bool RevLogCorrection_Table(IplImage *pInputImg, IplImage *pOutputImg, float fConst);
	/**
	*	function:		GammaCorrection
	*	description:	冥次变换函数
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	fGamma:冥次变换参数Gamme
	*	return:			bool值:是否处理成功
	**/
	bool GammaCorrection(IplImage *pInputImg, IplImage *pOutputImg, float fGamma);

	/**
	*	function:		GammaCorrection_Table
	*	description:	(查表)冥次变换函数
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	fGamma:冥次变换参数Gamme
	*	return:			bool值:是否处理成功
	**/
	bool GammaCorrection_Table(IplImage *pInputImg, IplImage *pOutputImg, float fGamma);

	/**
	*	function:		GetHistogram
	*	description:	统计灰度直方图
	*	parameter:		pInputImg:需要统计直方图图像指针
	pHist:存放直方图数据指针
	pMax:直方图最大值
	*	return:			bool值:是否处理成功
	**/
	bool GetHistogram(IplImage *pInputImg, float *pHist, float *pMax);

	/**
	*	function:		EqualizeHist
	*	description:	直方图均衡化
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	*	return:			bool值:是否处理成功
	**/
	bool EqualizeHist(IplImage *pInputImg, IplImage *pOutputImg);

	/**
	*	function:		Smooth_Blur
	*	description:	均值滤波
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	nSize:滤波模板大小
	*	return:			bool值:是否处理成功
	**/
	bool Smooth_Blur(IplImage *pInputImg, IplImage *pOutputImg, int nSize);

	/**
	*	function:		Smooth_Blur_Table
	*	description:	积分图均值滤波
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	nSize:滤波模板大小
	*	return:			bool值:是否处理成功
	**/
	bool Smooth_Blur_Table(IplImage *pInputImg, IplImage *pOutputImg, int nSize);

	/**
	*	function:		Median_Blur_Sort
	*	description:	中值滤波(基于排序)
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	nSize:滤波模板大小
	pSortFunc:指向本类中排序函数的指针
	*	return:			bool值:是否处理成功
	**/
	bool Median_Blur_Sort(IplImage *pInputImg, IplImage *pOutputImg, int nSize, void(CImgProcessing::*pSortFunc)(uchar*, unsigned int));

	/**
	*	function:		Median_Blur_Hist
	*	description:	中值滤波(基于直方图)
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	nSize:滤波模板大小
	*	return:			bool值:是否处理成功
	**/
	bool Median_Blur_Hist(IplImage *pInputImg, IplImage *pOutputImg, int nSize);

	/**
	*	function:		Smooth_Gaussian
	*	description:	高斯滤波
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	nSize:滤波模板大小
	fSigma:高斯分布参数
	*	return:			bool值:是否处理成功
	**/
	bool Smooth_Gaussian(IplImage *pInputImg, IplImage *pOutputImg, int nSize, float fSigma);

	/**
	*	function:		Smooth_Gaussian
	*	description:	可分离高斯滤波(将二位高斯滤波拆成一维，更快)
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	nSize:滤波模板大小
	fSigma:高斯分布参数
	*	return:			bool值:是否处理成功
	**/
	bool Smooth_Gaussian_SepFilter(IplImage *pInputImg, IplImage *pOutputImg, int nSize, float fSigma);

	/**
	*	function:		BilateralFilter
	*	description:	双边滤波
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	nSize:滤波模板大小
	fSigmaColor:双边滤波颜色Sigma
	fSigmaSpace:双边滤波控件Sigma
	*	return:			bool值:是否处理成功
	**/
	bool BilateralFilter(IplImage *pInputImg, IplImage *pOutputImg, int nSize, float fSigmaColor, float fSigmaSpace);

	/**
	*	function:		Sobel
	*	description:	Sobel边缘检测
	*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
	*	return:			bool值:是否处理成功
	**/
	bool Sobel(IplImage *pInputImg, IplImage *pOutputImg);


	bool Motion_Estimation(IplImage *pInputImg_pre, IplImage *pInputImg, IplImage *pOutputImg, int nSchRange);

	/******************************排序算法(用于查询中值)*****************************************/
	void exchangeNum(uchar *num1, uchar *num2);

	void Bubble_Sort(uchar* pDataList, unsigned int nSize);

	void Selection_Sort(uchar* pDataList, unsigned int nSize);

	void Insertion_Sort(uchar* pDataList, unsigned int nSize);

	void Quick_Sort(uchar* pDataList, unsigned int nSize);

	void Shell_Sort(uchar* pDataList, unsigned int nSize);

	void RadixMSD_Sort(uchar* pDataList, unsigned int nSize);

	/*********************************获取直方图中值*****************************************************/
	uchar Get_Hist_Med(unsigned int* pnHist, unsigned int nMed);
};