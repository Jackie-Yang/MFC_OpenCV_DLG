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
	*	description:	��IplImage��ʽת����HBITMAP��ʽ
	*	parameter:		IplImage* pImg:IplImageͼ��ָ��
	*	return:			HBITMAP:λͼͼ��ָ��
	**/
	HBITMAP IplImage2hBitmap(IplImage* pImg);

	/**
	*	function:		IplImage2hBitmap
	*	description:	��IplImage��ʽת����ָ���ߴ�HBITMAP��ʽ
	*	parameter:		IplImage* pImg:IplImageͼ��ָ��
	nWidth, nHeight:���λͼ��ȸ߶�
	*	return:			HBITMAP:λͼͼ��ָ��
	**/
	HBITMAP IplImage2hBitmap(IplImage* pImg, int nWidth, int nHeight);

	/**
	*	function:		FillBitmapInfo
	*	description:	��IplImage��ʽת����ָ���ߴ�BITMAPINFO��ʽ
	*	parameter:		IplImage* pImg:IplImageͼ��ָ��
	width, height:ͼ���ȸ߶�
	bpp:ͼ��λ���
	*	return:			BITMAPINFO*:λͼͼ��ָ��
	**/
	BITMAPINFO* FillBitmapInfo(int width, int height, int bpp);

	void BGR2YUV(IplImage *pImgBGR, IplImage *pImgY, IplImage *pImgU, IplImage *pImgV);
	void YUV2BGR(IplImage *pImgBGR, IplImage *pImgY, IplImage *pImgU, IplImage *pImgV);

	/**
	*	function:		AddGuassianNoise
	*	description:	��Ӹ�˹����
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	fNoisePSNR:�����ͼ��ķ�ֵ�����
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool AddGuassianNoise(IplImage *pInputImg, IplImage *pOutputImg, float fNoisePSNR);

	/**
	*	function:		AddPepperSaltNoise
	*	description:	��ӽ�������
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	fNoisePSNR:�����ͼ��ķ�ֵ�����
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool AddPepperSaltNoise(IplImage *pInputImg, IplImage *pOutputImg, float fNoisePSNR);


	/**
	*	function:		GrayReversion
	*	description:	�Ҷȷ�ת����
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool GrayReversion(IplImage *pInputImg, IplImage *pOutputImg);

	/**
	*	function:		LogCorrection
	*	description:	ͼ������任���� s = c*log(1 + r)
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	fConst:�����任����
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool LogCorrection(IplImage *pInputImg, IplImage *pOutputImg, float fConst);

	/**
	*	function:		RevLogCorrection
	*	description:	ͼ�񷴶����任���� r = c*log(1 + s)  s = exp(r/c) - 1
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	fConst:�����任����
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool RevLogCorrection(IplImage *pInputImg, IplImage *pOutputImg, float fConst);

	/**
	*	function:		LogCorrection_Table
	*	description:	(���)ͼ������任���� s = c*log(1 + r)
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	fConst:�����任����
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool LogCorrection_Table(IplImage *pInputImg, IplImage *pOutputImg, float fConst);

	/**
	*	function:		RevLogCorrection_Table
	*	description:	(���)ͼ�񷴶����任���� r = c*log(1 + s)  s = exp(r/c) - 1
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	fConst:�����任����
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool RevLogCorrection_Table(IplImage *pInputImg, IplImage *pOutputImg, float fConst);
	/**
	*	function:		GammaCorrection
	*	description:	ڤ�α任����
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	fGamma:ڤ�α任����Gamme
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool GammaCorrection(IplImage *pInputImg, IplImage *pOutputImg, float fGamma);

	/**
	*	function:		GammaCorrection_Table
	*	description:	(���)ڤ�α任����
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	fGamma:ڤ�α任����Gamme
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool GammaCorrection_Table(IplImage *pInputImg, IplImage *pOutputImg, float fGamma);

	/**
	*	function:		GetHistogram
	*	description:	ͳ�ƻҶ�ֱ��ͼ
	*	parameter:		pInputImg:��Ҫͳ��ֱ��ͼͼ��ָ��
	pHist:���ֱ��ͼ����ָ��
	pMax:ֱ��ͼ���ֵ
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool GetHistogram(IplImage *pInputImg, float *pHist, float *pMax);

	/**
	*	function:		EqualizeHist
	*	description:	ֱ��ͼ���⻯
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool EqualizeHist(IplImage *pInputImg, IplImage *pOutputImg);

	/**
	*	function:		Smooth_Blur
	*	description:	��ֵ�˲�
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	nSize:�˲�ģ���С
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool Smooth_Blur(IplImage *pInputImg, IplImage *pOutputImg, int nSize);

	/**
	*	function:		Smooth_Blur_Table
	*	description:	����ͼ��ֵ�˲�
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	nSize:�˲�ģ���С
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool Smooth_Blur_Table(IplImage *pInputImg, IplImage *pOutputImg, int nSize);

	/**
	*	function:		Median_Blur_Sort
	*	description:	��ֵ�˲�(��������)
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	nSize:�˲�ģ���С
	pSortFunc:ָ��������������ָ��
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool Median_Blur_Sort(IplImage *pInputImg, IplImage *pOutputImg, int nSize, void(CImgProcessing::*pSortFunc)(uchar*, unsigned int));

	/**
	*	function:		Median_Blur_Hist
	*	description:	��ֵ�˲�(����ֱ��ͼ)
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	nSize:�˲�ģ���С
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool Median_Blur_Hist(IplImage *pInputImg, IplImage *pOutputImg, int nSize);

	/**
	*	function:		Smooth_Gaussian
	*	description:	��˹�˲�
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	nSize:�˲�ģ���С
	fSigma:��˹�ֲ�����
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool Smooth_Gaussian(IplImage *pInputImg, IplImage *pOutputImg, int nSize, float fSigma);

	/**
	*	function:		Smooth_Gaussian
	*	description:	�ɷ����˹�˲�(����λ��˹�˲����һά������)
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	nSize:�˲�ģ���С
	fSigma:��˹�ֲ�����
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool Smooth_Gaussian_SepFilter(IplImage *pInputImg, IplImage *pOutputImg, int nSize, float fSigma);

	/**
	*	function:		BilateralFilter
	*	description:	˫���˲�
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	nSize:�˲�ģ���С
	fSigmaColor:˫���˲���ɫSigma
	fSigmaSpace:˫���˲��ؼ�Sigma
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool BilateralFilter(IplImage *pInputImg, IplImage *pOutputImg, int nSize, float fSigmaColor, float fSigmaSpace);

	/**
	*	function:		Sobel
	*	description:	Sobel��Ե���
	*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
	*	return:			boolֵ:�Ƿ���ɹ�
	**/
	bool Sobel(IplImage *pInputImg, IplImage *pOutputImg);


	bool Motion_Estimation(IplImage *pInputImg_pre, IplImage *pInputImg, IplImage *pOutputImg, int nSchRange);

	/******************************�����㷨(���ڲ�ѯ��ֵ)*****************************************/
	void exchangeNum(uchar *num1, uchar *num2);

	void Bubble_Sort(uchar* pDataList, unsigned int nSize);

	void Selection_Sort(uchar* pDataList, unsigned int nSize);

	void Insertion_Sort(uchar* pDataList, unsigned int nSize);

	void Quick_Sort(uchar* pDataList, unsigned int nSize);

	void Shell_Sort(uchar* pDataList, unsigned int nSize);

	void RadixMSD_Sort(uchar* pDataList, unsigned int nSize);

	/*********************************��ȡֱ��ͼ��ֵ*****************************************************/
	uchar Get_Hist_Med(unsigned int* pnHist, unsigned int nMed);
};