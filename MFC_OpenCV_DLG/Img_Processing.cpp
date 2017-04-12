#include "stdafx.h"
#include "ImgProcessing.h"

CImgProcessing::CImgProcessing()
{
	Edge_Mode = IPL_BORDER_REFLECT_101;
	m_rng = cvRNG(cvGetTickCount());
}


//void Resize(IplImage *src,IplImage *dst)
//{
//	int src_Width = src->width;
//	int src_Height = src->height;
//	int dst_Width = dst->width;
//	int dst_Height = dst->height;
//	float Width_Scale = (float)src_Width / dst_Width;
//	float Height_Scale = (float)dst_Height / dst_Height;
//	uchar *pDataOut = (uchar *)dst->imageData;
//
//
//	for (int i = 0, float y = 0.0f; i < dst_Height; i++, y += Height_Scale)
//	{
//		for (int j = 0, float x = 0.0f; j < dst_Width; j++, x += Width_Scale)
//		{
//
//		}
//		pDataOut += dst->widthStep;
//	}
//	
//}

/**
*	function:		IplImage2hBitmap
*	description:	将IplImage格式转换成HBITMAP格式
*	parameter:		IplImage* pImg:IplImage图像指针
*	return:			HBITMAP:位图图像指针
**/
HBITMAP  CImgProcessing::IplImage2hBitmap(IplImage* pImg, int nWidth, int nHeight)
{
	HBITMAP hBmp;
	IplImage* dst = cvCreateImage(cvSize(nWidth, nHeight), pImg->depth, pImg->nChannels);
	cvResize(pImg, dst);			//根据设定大小生成新的图像
	hBmp = IplImage2hBitmap(dst);	//将图像转换成hbitmap类型
	cvReleaseImage(&dst);		//清除临时图像内存
	return hBmp;
}

/**
*	function:		IplImage2hBitmap
*	description:	将IplImage格式转换成指定尺寸HBITMAP格式
*	parameter:		IplImage* pImg:IplImage图像指针
nWidth, nHeight:输出位图宽度高度
*	return:			HBITMAP:位图图像指针
**/
HBITMAP  CImgProcessing::IplImage2hBitmap(IplImage* pImg)
{
	BYTE tmp[sizeof(BITMAPINFO) + 1024];
	BITMAPINFO *bmi = (BITMAPINFO*)tmp;
	HBITMAP hBmp;
	int i;
	memset(bmi, 0, sizeof(BITMAPINFO));
	bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi->bmiHeader.biWidth = pImg->width;
	bmi->bmiHeader.biHeight = -abs(pImg->height);
	bmi->bmiHeader.biPlanes = 1;
	bmi->bmiHeader.biBitCount = pImg->nChannels * pImg->depth;
	bmi->bmiHeader.biCompression = BI_RGB;
	bmi->bmiHeader.biSizeImage = 0;//if biCompression is BI_RGB,this can be 0
	bmi->bmiHeader.biClrImportant = 0;

	switch (pImg->nChannels * pImg->depth)
	{
	case 8:
		for (i = 0; i < 256; i++){
			bmi->bmiColors[i].rgbBlue = i;
			bmi->bmiColors[i].rgbGreen = i;
			bmi->bmiColors[i].rgbRed = i;
		}
		break;
	case 32:
	case 24:
		((DWORD*)bmi->bmiColors)[0] = 0x00FF0000;
		((DWORD*)bmi->bmiColors)[1] = 0x0000FF00;
		((DWORD*)bmi->bmiColors)[2] = 0x000000FF;
		break;
	}
	hBmp = ::CreateDIBSection(NULL, bmi, DIB_RGB_COLORS, NULL, 0, 0);

	SetDIBits(NULL, hBmp, 0, pImg->height, pImg->imageData, bmi, DIB_RGB_COLORS);

	return hBmp;
}

/**
*	function:		FillBitmapInfo
*	description:	将IplImage格式转换成指定尺寸BITMAPINFO格式
*	parameter:		IplImage* pImg:IplImage图像指针
width, height:图像宽度高度
bpp:图像位深度
*	return:			BITMAPINFO*:位图图像指针
**/
BITMAPINFO* CImgProcessing::FillBitmapInfo(int width, int height, int bpp)
{
	ASSERT(width > 0 && height > 0 &&
		(bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFO* pBmi = (BITMAPINFO*) new char[(1078 - sizeof(BITMAPFILEHEADER))*sizeof(BITMAPINFO)];

	BITMAPINFOHEADER* bmih = &pBmi->bmiHeader;

	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = bpp;
	bmih->biCompression = BI_RGB;

	if (bpp == 8)
	{
		RGBQUAD* palette = pBmi->bmiColors;
		int i;
		for (i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}

	return pBmi;
}



//IplImage* AddPepperSaltNoise(IplImage* src)    //添加椒盐噪声，随机黑白点
//{
//	IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
//	cvCopy(src, dst);
//	for (int k = 0; k<(src->height*src->width*P); k++)
//	{
//		int i = rand() % src->height;
//		int j = rand() % src->width;
//		int m = rand() % 2;
//		CvScalar s = cvGet2D(src, i, j);
//		if (src->nChannels == 1)
//		{
//			if (m == 0)
//			{
//				s.val[0] = 255;
//			}
//			else
//			{
//				s.val[0] = 0;
//			}
//		}
//		else if (src->nChannels == 3)
//		{
//			if (m == 0)
//			{
//				s.val[0] = 255;
//				s.val[1] = 255;
//				s.val[2] = 255;
//			}
//			else
//			{
//				s.val[0] = 0;
//				s.val[1] = 0;
//				s.val[2] = 0;
//			}
//		}
//		cvSet2D(dst, i, j, s);
//	}
//	return dst;
//}

//double generateGaussianNoise(double mu, double sigma)
//{
//	static double V1, V2, S;
//	static int phase = 0;
//	double X;
//	double U1, U2;
//	if (phase == 0) {
//		do {
//			U1 = (double)rand() / RAND_MAX;
//			U2 = (double)rand() / RAND_MAX;
//
//			V1 = 2 * U1 - 1;
//			V2 = 2 * U2 - 1;
//			S = V1 * V1 + V2 * V2;
//		} while (S >= 1 || S == 0);
//
//		X = V1 * sqrt(-2 * log(S) / S);
//	}
//	else{
//		X = V2 * sqrt(-2 * log(S) / S);
//	}
//	phase = 1 - phase;
//	return mu + sigma*X;
//}

//double AverageRandom(double min, double max)
//{
//	int minInteger = (int)(min * 10000);
//	int maxInteger = (int)(max * 10000);
//	int randInteger = rand()*rand();
//	int diffInteger = maxInteger - minInteger;
//	int resultInteger = randInteger % diffInteger + minInteger;
//	return resultInteger / 10000.0;
//}
//
//double Normal(double x, double miu, double sigma) //概率密度函数
//{
//	return 1.0 / sqrt(2 * 3.1415926*sigma) * exp(-1 * (x - miu)*(x - miu) / (2 * sigma*sigma));
//}
//double NormalRandom(double miu,double sigma, double min, double max)//产生正态分布随机数
//{
//	double x;
//	double dScope;
//	double y;
//	do
//	{
//		x = AverageRandom(min, max);
//		y = Normal(x, miu, sigma);
//		dScope = AverageRandom(0, Normal(miu, miu, sigma));
//	} while (dScope > y);
//	return x;
//}

/**
*	function:		AddGuassianNoise
*	description:	添加高斯噪声
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
					fNoisePSNR:加噪后图像的峰值信噪比
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::AddGuassianNoise(IplImage *pInputImg, IplImage *pOutputImg, float fNoisePSNR)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//多通道图进行分离处理
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//临时存储输出结果
		IplImage *pTemp;	//指针交换中间变量

		//分离通道
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blue通道处理
		bValidB = AddGuassianNoise(pBlueChannel, pTempImg, fNoisePSNR);	//嵌套调用函数
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//输出结果在pTempImg,指针交换
		//Green通道处理
		bValidG = AddGuassianNoise(pGreenChannel, pTempImg, fNoisePSNR);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Red通道处理
		bValidR = AddGuassianNoise(pRedChannel, pTempImg, fNoisePSNR);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//合成通道
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//单通道直接处理
	bool bValid = TRUE;
	double dVarSum = 0.0;
	double dRandParm;
	//计算噪声信号标准差
	dRandParm = 255 / pow(10.0f, fNoisePSNR / 20.0f);

	IplImage* pNoise = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
	//生成均值0、标准差为噪声信号标准差的高斯分布序列
	cvRandArr(&m_rng, pNoise, CV_RAND_NORMAL, cvScalarAll(0), cvScalarAll(dRandParm));
	//加噪
	cvAdd(pInputImg, pNoise, pOutputImg);
	cvReleaseImage(&pNoise);

	return bValid;
}

/**
*	function:		AddPepperSaltNoise
*	description:	添加椒盐噪声
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
					fNoisePSNR:加噪后图像的峰值信噪比
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::AddPepperSaltNoise(IplImage *pInputImg, IplImage *pOutputImg, float fNoisePSNR)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//多通道图进行分离处理
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//临时存储输出结果
		IplImage *pTemp;	//指针交换中间变量

		//分离通道
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blue通道处理
		bValidB = AddPepperSaltNoise(pBlueChannel, pTempImg, fNoisePSNR);	//嵌套调用函数
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//输出结果在pTempImg,指针交换
		//Green通道处理
		bValidG = AddPepperSaltNoise(pGreenChannel, pTempImg, fNoisePSNR);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Red通道处理
		bValidR = AddPepperSaltNoise(pRedChannel, pTempImg, fNoisePSNR);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//合成通道
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//单通道直接处理
	bool bValid = TRUE;
	double dVarSum = 0.0;

	double dNoiseValSum;
	//计算方差乘以总像素个数得到方差总值
	dNoiseValSum = 255 * 255 / pow(10.0f, fNoisePSNR / 10.0f) * pInputImg->width * pInputImg->height;
	int nNoiseX, nNoiseY, nNoiseVal, nIndex;
	uchar *pInput = (uchar *)pInputImg->imageData;
	uchar *pOutput = (uchar *)pOutputImg->imageData;

	cvCopy(pInputImg, pOutputImg);

	//一直产生随机的椒盐噪声，直到达到设定值
	while (dNoiseValSum > 0)
	{
		nNoiseX = cvRandInt(&m_rng) % pOutputImg->width;
		nNoiseY = cvRandInt(&m_rng) % pOutputImg->height;
		nNoiseVal = cvRandInt(&m_rng) % 2;
		nIndex = nNoiseX + pOutputImg->widthStep * nNoiseY;
		if (nNoiseVal)
		{
			*(pOutput + nIndex) = (uchar)255;
		}
		else
		{
			*(pOutput + nIndex) = (uchar)0;
		}
		dNoiseValSum -= ((double)*(pOutput + nIndex) - (double)*(pInput + nIndex)) * ((double)*(pOutput + nIndex) - (double)*(pInput + nIndex));
	}


	return bValid;
}


/**
*	function:		GrayReversion
*	description:	灰度反转函数
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::GrayReversion(IplImage *pInputImg, IplImage *pOutputImg)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;

	bool bValid = TRUE;

	//优化前，逐个取反
	/*for (int i = 0; i < pInputImg->imageSize; i++)
	{
		*(pOutputImg->imageData + i) = char(255 - (uchar)*(pInputImg->imageData + i));
	}*/
	
	//优化后,4个字节一起取反
	int nImageSize_4Byte = pInputImg->imageSize / 4;
	int *pDataIn = (int *)pInputImg->imageData;
	int *pDataOut = (int *)pOutputImg->imageData;

	for (int i = 0; i < nImageSize_4Byte; i++)
	{
		*(pDataOut + i) = ~*(pDataIn + i);
	}


	return bValid;
}


/**
*	function:		LogCorrection
*	description:	图像对数变换函数 s = c*log(1 + r)
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
fConst:对数变换常数
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::LogCorrection(IplImage *pInputImg, IplImage *pOutputImg, float fConst)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;

	bool bValid = TRUE;
	float fVal = 0.0f;


	for (int i = 0; i < pInputImg->imageSize; i++)
	{
		fVal = (uchar)*(pInputImg->imageData + i);
		fVal = log(fVal + 1.0f) * fConst;
		if (fVal > 255.0f) fVal = 255.0f;
		*(pOutputImg->imageData + i) = char(fVal);
	}
	return bValid;
}

/**
*	function:		RevLogCorrection
*	description:	图像反对数变换函数 r = c*log(1 + s)  s = exp(r/c) - 1
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
fConst:对数变换常数
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::RevLogCorrection(IplImage *pInputImg, IplImage *pOutputImg, float fConst)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;

	bool bValid = TRUE;
	float fVal = 0.0f;


	for (int i = 0; i < pInputImg->imageSize; i++)
	{
		fVal = (uchar)*(pInputImg->imageData + i);
		fVal = exp(fVal / fConst) - 1;
		if (fVal > 255.0f) fVal = 255.0f;
		*(pOutputImg->imageData + i) = char(fVal);
	}
	return bValid;
}

/**
*	function:		LogCorrection_Table
*	description:	(查表)图像对数变换函数 s = c*log(1 + r)
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
fConst:对数变换常数
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::LogCorrection_Table(IplImage *pInputImg, IplImage *pOutputImg, float fConst)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;

	bool bValid = TRUE;
	float fVal = 0.0f;
	char achTable[256];
	uchar *pInputData = (uchar*)pInputImg->imageData;
	char *pOutputData = (char*)pOutputImg->imageData;

	for (int i = 0; i < 256; i++)
	{
		fVal = log(float(i) + 1.0f) * fConst;
		if (fVal > 255.0f) fVal = 255.0f;
		*(achTable + i) = char(fVal);
	}


	for (int i = 0; i < pInputImg->imageSize; i++, pInputData++, pOutputData++)
	{
		*pOutputData = *(achTable + *pInputData);
	}
	return bValid;
}

/**
*	function:		RevLogCorrection_Table
*	description:	(查表)图像反对数变换函数 r = c*log(1 + s)  s = exp(r/c) - 1
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
fConst:对数变换常数
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::RevLogCorrection_Table(IplImage *pInputImg, IplImage *pOutputImg, float fConst)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;

	bool bValid = TRUE;
	float fVal = 0.0f;
	char achTable[256];
	uchar *pInputData = (uchar*)pInputImg->imageData;
	char *pOutputData = (char*)pOutputImg->imageData;

	for (int i = 0; i < 256; i++)
	{
		fVal = exp(float(i) / fConst) - 1;
		if (fVal > 255.0f) fVal = 255.0f;
		*(achTable + i) = char(fVal);
		//fVal = log(float(i) + 1.0f) * fConst;
		//if (fVal > 255.0f) fVal = 255.0f;
		//*(achTable + (int)fVal) = char((float)i);
	}


	for (int i = 0; i < pInputImg->imageSize; i++, pInputData++, pOutputData++)
	{
		*pOutputData = *(achTable + *pInputData);
	}
	return bValid;
}

/**
*	function:		GammaCorrection
*	description:	冥次变换函数
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
fGamma:冥次变换参数Gamme
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::GammaCorrection(IplImage *pInputImg, IplImage *pOutputImg, float fGamma)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;

	bool bValid = TRUE;
	float fVal = 0.0f, fScale = 255.0f;


	for (int i = 0; i < pInputImg->imageSize; i++)
	{
		fVal = (uchar)*(pInputImg->imageData + i);
		fVal /= fScale;		//映射值到0~1,变换后Gamma>1把输入的窄带暗值映射到宽带输出亮值
		fVal = pow(fVal, fGamma);		//Gamma<1则把输入高值映射为宽带
		fVal *= fScale;					//映射回0~255
		if (fVal > 255.0f) fVal = 255.0f;	//判断溢出
		*(pOutputImg->imageData + i) = char(fVal);
	}
	return bValid;
}

/**
*	function:		GammaCorrection_Table
*	description:	(查表)冥次变换函数
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
fGamma:冥次变换参数Gamme
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::GammaCorrection_Table(IplImage *pInputImg, IplImage *pOutputImg, float fGamma)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;

	bool bValid = TRUE;
	float fVal = 0.0f, fScale = 255.0f;

	char achTable[256];
	uchar *pInputData = (uchar*)pInputImg->imageData;
	char *pOutputData = (char*)pOutputImg->imageData;

	for (int i = 0; i < 256; i++)
	{
		fVal = float(i) / fScale;		//映射值到0~1,变换后Gamma>1把输入的窄带暗值映射到宽带输出亮值
		fVal = pow(fVal, fGamma);		//Gamma<1则把输入高值映射为宽带
		fVal *= fScale;					//映射回0~255
		if (fVal > 255.0f) fVal = 255.0f;
		*(achTable + i) = char(fVal);
	}

	for (int i = 0; i < pInputImg->imageSize; i++, pInputData++, pOutputData++)
	{
		*pOutputData = *(achTable + *pInputData);
	}
	return bValid;
}


/**
*	function:		GetHistogram
*	description:	统计灰度直方图
*	parameter:		pInputImg:需要统计直方图图像指针
pHist:存放直方图数据指针
pMax:直方图最大值
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::GetHistogram(IplImage *pInputImg, float *pHist, float *pMax)
{
	if (NULL == pInputImg || NULL == pHist || pInputImg->nChannels != 1)return FALSE;
	bool bValid = TRUE;
	memset(pHist, 0, sizeof(float) * 256);
	int width_offset = pInputImg->widthStep - pInputImg->width;
	ULONG size = pInputImg->width * pInputImg->height;
	uchar *pData = (uchar*)pInputImg->imageData;
	*pMax = 0;

	for (int i = 0; i < pInputImg->height; i++)
	{
		for (int j = 0; j < pInputImg->width; j++)
		{
			(*(pHist + *(pData++)))++;
		}
		pData += width_offset;
	}

	for (int i = 0; i < 256; i++)
	{
		*(pHist + i) /= size;
		if (*(pHist + i) > *pMax)
		{
			*pMax = *(pHist + i);
		}
	}
	return bValid;
}

/**
*	function:		BGR2YUV
*	description:	RGB转YUV并分离
*	parameter:		pImgBGR:输入RGB图像指针
					pImgY,pImgU,pImgV:输出的YUV图像指针
*	return:			bool值:是否处理成功
**/
void CImgProcessing::BGR2YUV(IplImage *pImgBGR, IplImage *pImgY, IplImage *pImgU, IplImage *pImgV)
{
	int i,j;
	int DataSize = pImgBGR->imageSize / 3;
	uchar* pData = (uchar*)pImgBGR->imageData;
	//float R, G, B;
	long R, G, B, Y;
	for (i = 0, j = 0; i < DataSize; i++)
	{
		/*B = (float)*(pData + j++);
		G = (float)*(pData + j++);
		R = (float)*(pData + j++);*/
		B = (long)*(pData + j++);
		G = (long)*(pData + j++);
		R = (long)*(pData + j++);
		//*(pImgY->imageData + i) = char(0.114f * B
		//						+ 0.587f * G
		//						+ 0.299f * R);
		//*(pImgU->imageData + i) = char(128
		//						+ 0.5f * B
		//						- 0.3313f * G
		//						- 0.1687f * R);
		//*(pImgV->imageData + i) = char(128
		//						- 0.0813f * B
		//						- 0.4187f * G
		//						+ 0.5f * R);
		Y = (B * 1868 + G * 9617 + R * 4899 + 8192) >> 14;
		*(pImgY->imageData + i) = char(Y);
		*(pImgU->imageData + i) = char((((B - Y) * 9241 + 8192) >> 14) + 128);
		*(pImgV->imageData + i) = char((((R - Y) * 11682 + 8192) >> 14) + 128);
	}
}

/**
*	function:		YUV2BGR
*	description:	YUV转RGB
*	parameter:		pImgBGR:输出RGB图像指针
					pImgY,pImgU,pImgV:输入的YUV图像指针
*	return:			bool值:是否处理成功
**/
void CImgProcessing::YUV2BGR(IplImage *pImgBGR, IplImage *pImgY, IplImage *pImgU, IplImage *pImgV)
{
	int i, j;
	int DataSize = pImgBGR->imageSize / 3;
	uchar* pDataY = (uchar*)pImgY->imageData;
	uchar* pDataU = (uchar*)pImgU->imageData;
	uchar* pDataV = (uchar*)pImgV->imageData;

	float Y, U, V;
	int R, G, B;
	for (i = 0, j = 0; i < DataSize; i++)
	{
		Y = float(*(pDataY + i));
		U = float(*(pDataU + i));
		V = float(*(pDataV + i));
		B = int(Y + 1.779f * (U - 128));
		G = int(Y - 0.3455f * (U - 128) - 0.7169f * (V - 128));
		R = int(Y + 1.4075f * (V - 128));
		if (B > 255)B = 255;
		if (B < 0)B = 0;
		if (G > 255)G = 255;
		if (G < 0)G = 0;
		if (R > 255)R = 255;
		if (R < 0)R = 0;
		*(pImgBGR->imageData + j++) = (char)B;
		*(pImgBGR->imageData + j++) = (char)G;
		*(pImgBGR->imageData + j++) = (char)R;
	}
}


/**
*	function:		EqualizeHist
*	description:	直方图均衡化
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::EqualizeHist(IplImage *pInputImg, IplImage *pOutputImg)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//多通道图进行分离处理
	if (pInputImg->nChannels == 3)
	{
		bool bValid = TRUE;
		IplImage;
		IplImage *pChannelY = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pChannelU = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pChannelV = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pResult = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//临时存储输出结果

		//分离通道
		BGR2YUV(pInputImg, pChannelY, pChannelU, pChannelV);
		bValid = EqualizeHist(pChannelY, pResult);
		//合成通道
		YUV2BGR(pOutputImg, pResult, pChannelU, pChannelV);

		cvReleaseImage(&pChannelY);
		cvReleaseImage(&pChannelU);
		cvReleaseImage(&pChannelV);
		cvReleaseImage(&pResult);
		return bValid;
	}
	else if (pInputImg->nChannels != 1)
	{
		return false;
	}
	bool bValid = TRUE;
	float TempHist[256] = { 0 };
	ULONG size = pInputImg->width * pInputImg->height;
	uchar *pInputData = (uchar*)pInputImg->imageData;
	char *pOutputData = pOutputImg->imageData;
	//统计直方图
	for (int i = 0; i < pInputImg->height; i++)
	{
		for (int j = 0; j < pInputImg->width; j++)
		{
			(*(TempHist + *(pInputData + j)))++;
		}
		pInputData += pInputImg->widthStep;
	}

	//灰度级映射,概率积分(累加)
	float fGrayMap = 255.0f / size;	//映射系数,除以size归一化,乘255映射到0~255
	*(TempHist) *= fGrayMap;		//灰度级映射,第一个不用积分
	for (int i = 1; i < 256; i++)
	{
		*(TempHist + i) *= fGrayMap;	//灰度级映射
		*(TempHist + i) += *(TempHist + i - 1);	//累加
	}

	//根据积分重新分布灰度级，以积分值作为灰度级(取积分值最接近的灰度级)
	pInputData = (uchar*)pInputImg->imageData;
	for (int i = 0; i < pInputImg->height; i++)
	{
		for (int j = 0; j < pInputImg->width; j++)
		{	//将灰度级改为当前像素灰度级的积分值
			*(pOutputData + j) = char(int(*(TempHist + *(pInputData + j)) + 0.5));
		}
		pInputData += pInputImg->widthStep;
		pOutputData += pOutputImg->widthStep;
	}
	return bValid;
}

/**
*	function:		Smooth_Blur
*	description:	均值滤波
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
nSize:滤波模板大小
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::Smooth_Blur(IplImage *pInputImg, IplImage *pOutputImg, int nSize)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//多通道图进行分离处理
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//临时存储输出结果
		IplImage *pTemp;	//指针交换中间变量

		//分离通道
		cvSplit(pInputImg,pBlueChannel,pGreenChannel,pRedChannel,NULL);

		//Blue通道处理
		bValidB = Smooth_Blur(pBlueChannel, pTempImg, nSize);		//嵌套调用函数
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//输出结果在pTempImg,指针交换
		//Green通道处理
		bValidG = Smooth_Blur(pGreenChannel, pTempImg, nSize);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Red通道处理
		bValidR = Smooth_Blur(pRedChannel, pTempImg, nSize);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//合成通道
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//单通道直接处理
	bool bValid = TRUE;
	unsigned long nSum = 0;	//像素和
	int nOffSet = nSize / 2;
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;
	unsigned int nArea = nSize * nSize;

	//边界拓展
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);

	char *pDataOut = pOutputImg->imageData; // 指向当前输出的数据

	int x, y;
	for (int j = 0; j < nHeight; j++)	//图像行定位
	{
		for (int i = 0; i < nWidth; i++)	//图像列定位
		{
			for (int m = -nOffSet; m <= nOffSet; m++)	//模板行定位
			{
				for (int n = -nOffSet; n <= nOffSet; n++)	//模板列定位
				{
					x = i + nOffSet + n;
					y = j + nOffSet + m;
					nSum += (uchar)*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x);
				}
			}
			*(pDataOut + i) = char(nSum / nArea);
			nSum = 0;
		}
		pDataOut += pOutputImg->widthStep;
	}

	cvReleaseImage(&pEdgeImage);

	return bValid;
}

/**
*	function:		Smooth_Blur_Table
*	description:	积分图均值滤波
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
nSize:滤波模板大小
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::Smooth_Blur_Table(IplImage *pInputImg, IplImage *pOutputImg, int nSize)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//多通道图进行分离处理
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//临时存储输出结果
		IplImage *pTemp;	//指针交换中间变量

		//分离通道
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blue通道处理
		bValidB = Smooth_Blur_Table(pBlueChannel, pTempImg, nSize);			//嵌套调用函数
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//输出结果在pTempImg,指针交换
		//Green通道处理
		bValidG = Smooth_Blur_Table(pGreenChannel, pTempImg, nSize);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Red通道处理
		bValidR = Smooth_Blur_Table(pRedChannel, pTempImg, nSize);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//合成通道
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//单通道直接处理
	bool bValid = TRUE;
	unsigned long nSum = 0;	//像素和
	int nOffSet = nSize / 2;
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;
	unsigned int nArea = nSize * nSize;

	//边界拓展
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);

	ULONG *pTable = new ULONG[(nWidth + n2OffSet) * (nHeight + n2OffSet)];	//积分图

	*(pTable) = *(pEdgeImage->imageData);	//积分图第一个元素

	//生成积分图
	for (int i = 1; i < nEdgeWidth; i++)	//积分图首行首列
	{	//首行
		*(pTable + i) = *(pTable + i - 1) + (uchar)*(pEdgeImage->imageData + i);
	}
	for (int j = 1; j < nEdgeHeight; j++)
	{	//首列
		*(pTable + j * nEdgeWidth) = *(pTable + (j - 1) * nEdgeWidth) + (uchar)*(pEdgeImage->imageData + j * pEdgeImage->widthStep);
	}

	for (int j = 1; j < nEdgeHeight; j++)		//积分图其余元素
	{
		for (int i = 1; i < nEdgeWidth; i++)
		{
			*(pTable + j * nEdgeWidth + i) = (uchar)*(pEdgeImage->imageData + j * pEdgeImage->widthStep + i) +
				*(pTable + (j - 1) * nEdgeWidth + i) -
				*(pTable + (j - 1) * nEdgeWidth + (i - 1)) +
				*(pTable + j * nEdgeWidth + i - 1);
		}
	}


	//处理图像
	//首元素
	*(pOutputImg->imageData) = char(*(pTable + n2OffSet * nEdgeWidth + n2OffSet) / nArea);
	//首行输出
	for (int i = 1; i < nWidth; i++)
	{
		nSum = *(pTable + n2OffSet * nEdgeWidth + i + n2OffSet) -
			*(pTable + n2OffSet * nEdgeWidth + i - 1);
		*(pOutputImg->imageData + i) = char(nSum / nArea);
	}


	for (int j = 1; j < nHeight; j++)
	{
		//首列输出
		nSum = *(pTable + (j + n2OffSet) * nEdgeWidth + n2OffSet) -
			*(pTable + (j - 1) * nEdgeWidth + n2OffSet);
		*(pOutputImg->imageData + j * pOutputImg->widthStep) = char(nSum / nArea);
		//其余元素输出
		for (int i = 1; i < nWidth ; i++)
		{
			nSum = *(pTable + (j + n2OffSet) * nEdgeWidth + i + n2OffSet) -
				*(pTable + (j + n2OffSet) * nEdgeWidth + i - 1) +	//i - 1 = x - nOffset - 1
				*(pTable + (j - 1) * nEdgeWidth + i - 1) -			//j - 1 = y - nOffset - 1
				*(pTable + (j - 1) * nEdgeWidth + i + n2OffSet);
			*(pOutputImg->imageData + j * pOutputImg->widthStep + i) = char(nSum / nArea);
		}
	}

	delete[] pTable;		//释放积分图
	cvReleaseImage(&pEdgeImage);
	return bValid;

}


/**
*	function:		Median_Blur_Sort
*	description:	中值滤波(基于排序)
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
nSize:滤波模板大小
pSortFunc:指向本类中排序函数的指针
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::Median_Blur_Sort(IplImage *pInputImg, IplImage *pOutputImg, int nSize, void (CImgProcessing::*pSortFunc)(uchar*, unsigned int))
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//多通道图进行分离处理
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//临时存储输出结果
		IplImage *pTemp;	//指针交换中间变量

		//分离通道
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blue通道处理
		bValidB = Median_Blur_Sort(pBlueChannel, pTempImg, nSize, pSortFunc);		//嵌套调用函数
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//输出结果在pTempImg,指针交换
		//Green通道处理
		bValidG = Median_Blur_Sort(pGreenChannel, pTempImg, nSize, pSortFunc);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Red通道处理
		bValidR = Median_Blur_Sort(pRedChannel, pTempImg, nSize, pSortFunc);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//合成通道
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//单通道直接处理
	bool bValid = TRUE;
	int nOffSet = nSize / 2;
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;
	unsigned int nArea = nSize * nSize;
	int nMedian = nArea / 2;

	//边界拓展
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);

	uchar *pchKernel = new uchar[nArea];	//存放模板数据进行排序
	uchar *pKernel_Index = pchKernel;		//数组索引
	char *pDataOut = pOutputImg->imageData; // 指向当前输出的数据

	int x, y;
	for (int j = 0; j < nHeight; j++)	//图像行定位
	{
		for (int i = 0; i < nWidth; i++)	//图像列定位
		{
			pKernel_Index = pchKernel;
			for (int m = -nOffSet; m <= nOffSet; m++)	//模板行定位
			{
				for (int n = -nOffSet; n <= nOffSet; n++)	//模板列定位
				{
					y = j + nOffSet + m;
					x = i + nOffSet + n;
					*(pKernel_Index++) = (uchar)*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x);
				}
			}
			(this->*pSortFunc)(pchKernel, nArea);
			*(pDataOut + i) = char(*(pchKernel + nMedian));	//取中值
		}
		pDataOut += pOutputImg->widthStep;
	}

	delete[] pchKernel;
	cvReleaseImage(&pEdgeImage);
	return bValid;
}



/**
*	function:		Median_Blur_Hist
*	description:	中值滤波(基于直方图)
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
nSize:滤波模板大小
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::Median_Blur_Hist(IplImage *pInputImg, IplImage *pOutputImg, int nSize)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//多通道图进行分离处理
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//临时存储输出结果
		IplImage *pTemp;	//指针交换中间变量

		//分离通道
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blue通道处理
		bValidB = Median_Blur_Hist(pBlueChannel, pTempImg, nSize);		//嵌套调用函数
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//输出结果在pTempImg,指针交换
		//Green通道处理
		bValidG = Median_Blur_Hist(pGreenChannel, pTempImg, nSize);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Red通道处理
		bValidR = Median_Blur_Hist(pRedChannel, pTempImg, nSize);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//合成通道
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//单通道直接处理
	bool bValid = TRUE;
	int nOffSet = nSize / 2;
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;
	unsigned int nArea = nSize * nSize;
	unsigned int nMedian = nArea / 2;

	//边界拓展
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);

	int x, y;
	char *pDataOut = pOutputImg->imageData; // 指向当前输出的数据


	unsigned int **pnHist = new unsigned int*[nWidth];	//为每一列分配一个直方图数据块
	if (pnHist == NULL)
	{
		return FALSE;
	}


	//初始化首个直方图(0列，0行)
	*(pnHist) = new unsigned int[256];	//分配直方图数据
	memset(*(pnHist), 0, sizeof(unsigned int) * 256);
	for (int m = -nOffSet; m <= nOffSet; m++)	//模板行定位
	{
		for (int n = -nOffSet; n <= nOffSet; n++)	//模板列定位
		{
			y = m + nOffSet;
			x = n + nOffSet;
			//相应直方图的相应灰度级计数
			(*(*(pnHist) + uchar(*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x))))++;
		}
	}
	*(pDataOut) = char(Get_Hist_Med(*(pnHist),nMedian));	//取中值

	//初始化首列剩余直方图
	for (int i = 1; i < nWidth; i++)	//图像列定位
	{
		*(pnHist + i) = new unsigned int[256];	//分配直方图数据
		memcpy(*(pnHist + i), *(pnHist + i - 1), sizeof(unsigned int) * 256);	//拷贝左边直方图数据
		for (int m = -nOffSet; m <= nOffSet; m++)	//模板行定位
		{
			y = m + nOffSet;
			//减去左边区域的左列
			x = i - 1; //i - 1 - nOffSet + nOffSet
			(*(*(pnHist + i) + (uchar)*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x)))--;

			//加上当前区域的右列
			x = i + nOffSet + nOffSet;
			(*(*(pnHist + i) + (uchar)*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x)))++;
		}
		*(pDataOut + i) = char(Get_Hist_Med(*(pnHist + i), nMedian));	//取中值
	}

	pDataOut += pOutputImg->widthStep;

	
	for (int j = 1; j < nHeight; j++)	//图像行定位
	{
		for (int i = 0; i < nWidth; i++)	//图像列定位
		{

			for (int n = -nOffSet; n <= nOffSet; n++)	//模板列定位
			{
				
				x = i + n + nOffSet;
				//减去上一区域上列
				y = j - 1; //j - 1 - nOffSet + nOffSet
				(*(*(pnHist + i) + (uchar)*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x)))--;

				//加上当前区域下列
				y = j + nOffSet + nOffSet;
				(*(*(pnHist + i) + (uchar)*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x)))++;
			}
			*(pDataOut + i) = char(Get_Hist_Med(*(pnHist + i), nMedian));	//取中值
		}
		pDataOut += pOutputImg->widthStep;
	}


	for (int i = 0; i < nWidth; i++)
	{
		delete[] * (pnHist + i);		//释放直方图数据
	}

	delete[] pnHist;		//释放直方图指针数据
	cvReleaseImage(&pEdgeImage);

	return bValid;
}





/**
*	function:		Smooth_Gaussian
*	description:	高斯滤波
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
nSize:滤波模板大小
fSigma:高斯分布参数
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::Smooth_Gaussian(IplImage *pInputImg, IplImage *pOutputImg, int nSize,float fSigma)
{ 
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//多通道图进行分离处理
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//临时存储输出结果
		IplImage *pTemp;	//指针交换中间变量

		//分离通道
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blue通道处理
		bValidB = Smooth_Gaussian(pBlueChannel, pTempImg, nSize, fSigma);	//嵌套调用函数
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//输出结果在pTempImg,指针交换
		//Green通道处理
		bValidG = Smooth_Gaussian(pGreenChannel, pTempImg, nSize, fSigma);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Red通道处理
		bValidR = Smooth_Gaussian(pRedChannel, pTempImg, nSize, fSigma);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//合成通道
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//单通道直接处理
	bool bValid = TRUE;

	int nOffSet = nSize / 2;                //定义滤波窗口中心的索引  
	int nArea = nSize * nSize;
	float *pfKernel = new float[nArea];
	float  fSum = 0.0f;                       //求和，进行归一化，计算权值    
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;

	//提前计算好数据，减少计算量
	float fSigma_2pow = pow(fSigma, 2.0f);	//Sigma平方
	float f_2PiSigma_2pow = 2.0f * 3.1415926f * fSigma_2pow;	//2*Pi*Sigma平方
	float fGaussianCoeff = -0.5f / fSigma_2pow;	//高斯系数
	float fResult = 0.0f;	//临时存放结果

	//边界拓展
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);

	//生成二维的高斯滤波系数  
	////////////////////二维高斯函数公式/////////////////////////////
	//          1                  (i - offset)^2+(j - offset)^2   //
	//   ---------------- exp(-1 * ------------------------------) //
	//   2*pi*Sigma*Sigma                2*Sigma*Sigma             //
	/////////////////////////////////////////////////////////////////	
	unsigned int nIndex;

	nIndex = 0;
	for (int y = -nOffSet; y <= nOffSet; y++)
	{
		for (int x = -nOffSet; x <= nOffSet; x++)
		{
			fResult = exp(fGaussianCoeff * (x * x + y * y)) / f_2PiSigma_2pow;
			*(pfKernel + nIndex++) = fResult;
			fSum += fResult;	//计算高斯系数和，用于计算权值
		}
	}
	//进行归一化,计算权值
	for (int i = 0; i < nArea; i++)
	{
		*(pfKernel + i) /= fSum;
	}

	char *pDataOut = pOutputImg->imageData; // 指向当前输出的数据
	int x, y;	//行、列定位值
	fResult = 0;
	for (int j = 0; j < nHeight; j++)	//图像行定位
	{
		for (int i = 0; i < nWidth; i++)	//图像列定位
		{
			for (int m = -nOffSet; m <= nOffSet; m++)	//模板行定位
			{
				for (int n = -nOffSet; n <= nOffSet; n++)	//模板列定位
				{
					y = j + nOffSet + m;
					x = i + nOffSet + n;
					fResult += *(pfKernel + nOffSet + n + (nOffSet + m) * nSize) * 
						(uchar)*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x);
				}
			}
			//前面已经作了归一化，可直接输出结果
			*(pDataOut + i) = char(fResult);
			fResult = 0;
		}
		pDataOut += pOutputImg->widthStep;
	}
	delete[] pfKernel;
	cvReleaseImage(&pEdgeImage);
	return bValid;
}








/**
*	function:		BilateralFilter
*	description:	双边滤波
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
nSize:滤波模板大小
fSigmaColor:双边滤波颜色Sigma
fSigmaSpace:双边滤波控件Sigma
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::BilateralFilter(IplImage *pInputImg, IplImage *pOutputImg, int nSize, float fSigmaColor, float fSigmaSpace)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//多通道图进行分离处理
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//临时存储输出结果
		IplImage *pTemp;	//指针交换中间变量

		//分离通道
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blue通道处理
		bValidB = BilateralFilter(pBlueChannel, pTempImg, nSize, fSigmaColor, fSigmaSpace);			//嵌套调用函数
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//输出结果在pTempImg,指针交换
		//Green通道处理
		bValidG = BilateralFilter(pGreenChannel, pTempImg, nSize, fSigmaColor, fSigmaSpace);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Red通道处理
		bValidR = BilateralFilter(pRedChannel, pTempImg, nSize, fSigmaColor, fSigmaSpace);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//合成通道
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//单通道直接处理
	bool bValid = TRUE;

	int nOffSet = nSize / 2;                //定义滤波窗口中心的索引  
	int nArea = nSize * nSize;
	float  fSum = 0.0f;                       //求和，进行归一化，计算权值    
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;

	//边界拓展
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);


	float* fDistDiff = new float[nArea]; //计算距离中间点的几何距离  
	float* fColorDiff = new float[nArea]; //定义中心点到当前点的灰度差  

	//提前计算好数据，减少计算量
	float fGaussianCoeff_Space = -0.5f / pow(fSigmaSpace, 2.0f);
	float fGaussianCoeff_Color = -0.5f / pow(fSigmaColor, 2.0f);


	unsigned int nIndex;

	nIndex = 0;
	for (int y = -nOffSet; y <= nOffSet; y++)
	{
		for (int x = -nOffSet; x <= nOffSet; x++)
		{
			*(fDistDiff + nIndex++) = exp(fGaussianCoeff_Space * (x * x + y * y));   //C参数  
		}
	}
	
	
	//以下求解灰度值的差  
	uchar *pDataOut = (uchar*)(pOutputImg->imageData); // 指向当前输出的数据
	int x, y;
	for (int j = 0; j < nHeight; j++)
	{
		for (int i = 0; i < nWidth; i++)
		{
			float fColor = (float)uchar(*(pEdgeImage->imageData + (j + nOffSet) * pEdgeImage->widthStep + i + nOffSet));  //当前点的灰度值   
			float fData = 0.0f;
			fSum = 0.0f;                      //用于进行归一化   

			nIndex = 0;
			for (int m = -nOffSet; m <= nOffSet; m++)	//模板行定位
			{
				for (int n = -nOffSet; n <= nOffSet; n++)	//模板列定位
				{
					y = j + nOffSet + m;
					x = i + nOffSet + n;

					float fColor1 = (float)uchar(*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x));
					float fColorDis = fColor - fColor1;
					*(fColorDiff + nIndex) = exp(fGaussianCoeff_Color * fColorDis * fColorDis); //S参数  

					//if (m || n)
					{
						float fColorDiffxDistDiff = *(fColorDiff + nIndex) * *(fDistDiff + nIndex);
						fData += fColor1 * fColorDiffxDistDiff; //C和S参数综合  
						fSum += fColorDiffxDistDiff;    //加权系数求和，进行归一化  
					}

					nIndex++;
				}
			}
			fData /= fSum;
			*(pDataOut + i) = (char)fData;
		}
		pDataOut += pOutputImg->widthStep;
	}
	delete[]fDistDiff;
	delete[]fColorDiff;
	cvReleaseImage(&pEdgeImage);
	return bValid;
}






/**
*	function:		Smooth_Gaussian
*	description:	可分离高斯滤波(将二位高斯滤波拆成一维，更快)
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
nSize:滤波模板大小
fSigma:高斯分布参数
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::Smooth_Gaussian_SepFilter(IplImage *pInputImg, IplImage *pOutputImg, int nSize, float fSigma)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//多通道图进行分离处理
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//临时存储输出结果
		IplImage *pTemp;	//指针交换中间变量

		//分离通道
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blue通道处理
		bValidB = Smooth_Gaussian_SepFilter(pBlueChannel, pTempImg, nSize, fSigma);			//嵌套调用函数
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//输出结果在pTempImg,指针交换
		//Green通道处理
		bValidG = Smooth_Gaussian_SepFilter(pGreenChannel, pTempImg, nSize, fSigma);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Red通道处理
		bValidR = Smooth_Gaussian_SepFilter(pRedChannel, pTempImg, nSize, fSigma);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//合成通道
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//单通道直接处理
	bool bValid = TRUE;
	int nWidth = pInputImg->width;		//图像宽度
	int nHeight = pInputImg->height;	//图像高度
	int nOffSet = (nSize) / 2;                //定义滤波窗口中心的索引 
	float *pfKernel = new float[nSize];		 //分配内存给滤波模板
	float fSum = 0.0f;		//求滤波系数和，计算权值
		
	//临时变量
	//提前计算好数据，减少计算量
	float fSigma_2pow = pow(fSigma, 2.0f);	//Sigma平方
	float f_Sqrt2PiSigma = sqrt(2.0f * 3.1415926f) * fSigma;	//2*Pi*Sigma平方
	float fGaussianCoeff = -0.5f / fSigma_2pow;	//高斯系数
	float fResult = 0.0f;	//临时存放结果
	IplImage *pTemp_Img = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, pInputImg->nChannels);
	uchar *pDataIn;	//指向当前处理的数据
	char *pDataOut;	//指向当前输出的数据

	//边界拓展
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);
	

	int nIndex;
	nIndex = 0;
	//生成高斯数据  
	for (int nDis = -nOffSet; nDis <= nOffSet; nDis++)
	{
		fResult = exp(fGaussianCoeff * nDis * nDis) / f_Sqrt2PiSigma;
		*(pfKernel + nIndex++) = fResult;
		fSum += fResult;	//计算高斯系数和，用于计算权值
	}
	//归一化,计算权值
	for (nIndex = 0; nIndex < nSize; nIndex++)
	{
		*(pfKernel + nIndex) /= fSum;
	}



	//可分离高斯滤波
	int x, y;	//模板运算取的行、列
	//x方向滤波
	fResult = 0.0f;
	pDataIn = (uchar*)pEdgeImage->imageData + pEdgeImage->widthStep * nOffSet;
	pDataOut = pTemp_Img->imageData;
	for (int j = 0; j < nHeight; j++)	//图像行定位
	{
		for (int i = 0; i < nWidth; i++)	//图像列定位
		{
			for (int n = -nOffSet; n <= nOffSet; n++)	//模板列定位
			{
				x = i + nOffSet + n;
				fResult += *(pfKernel + nOffSet + n) * uchar(*(pDataIn + x));
			}
			*(pDataOut + i) = char(fResult);
			fResult = 0.0f;
		}
		pDataIn += pEdgeImage->widthStep;
		pDataOut += pTemp_Img->widthStep;
	}

	
	//y方向滤波
	cvCopyMakeBorder(pTemp_Img, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);
	for (int i = 0; i < nWidth; i++)	//图像列定位
	{
		pDataIn = (uchar*)(pEdgeImage->imageData + i + nOffSet);	//先定位当前列
		pDataOut = (pOutputImg->imageData + i);
		for (int j = 0; j < nHeight; j++)	//图像行定位
		{
			for (int n = -nOffSet; n <= nOffSet; n++)	//模板列定位
			{
				y = j + nOffSet + n;
				fResult += *(pfKernel + nOffSet + n) * uchar(*(pDataIn + y * pEdgeImage->widthStep));
			}
			*(pDataOut) = char(fResult);
			pDataOut += pOutputImg->widthStep;	//跳到下一行
			fResult = 0.0f;
		}
	}

	cvReleaseImage(&pTemp_Img);		//释放临时图像
	cvReleaseImage(&pEdgeImage);	//释放拓展边界图像
	delete[] pfKernel;				//删除模板数据
	return bValid;

}


/**
*	function:		Sobel
*	description:	Sobel边缘检测
*	parameter:		pInputImg,pOutputImg:输入、输出图像指针
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::Sobel(IplImage *pInputImg, IplImage *pOutputImg)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//多通道图进行分离处理
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//临时存储输出结果
		IplImage *pTemp;	//指针交换中间变量

		//分离通道
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blue通道处理
		bValidB = Sobel(pBlueChannel, pTempImg);			//嵌套调用函数
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//输出结果在pTempImg,指针交换
		//Green通道处理
		bValidG = Sobel(pGreenChannel, pTempImg);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Red通道处理
		bValidR = Sobel(pRedChannel, pTempImg);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//合成通道
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//单通道直接处理
	bool bValid = TRUE;
	long nSum_X = 0;	//x方向求导像素和
	long nSum_Y = 0;	//y方向求导像素和
	long nSum = 0;		//2个方向求导像素和
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;
	int i, j;
	//系数和窗口较小,不用模板，优化速度
	//int pSobelX[3][3] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	//int pSobelY[3][3] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

	//边界拓展
	int nOffSet = 1;
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);


	uchar *pDataIn = (uchar*)(pEdgeImage->imageData + pEdgeImage->widthStep); // 指向当前处理的数据
	char *pDataOut = pOutputImg->imageData; // 指向当前输出的数据

	int x;
	for (j = 0; j < nHeight; j++)	//图像行定位
	{
		//图像X、Y方向求导
		for (i = 0; i < nWidth; i++)	//图像列定位,x方向可以忽略首列与最后一列
		{
			x = i + nOffSet;
			//X方向求导，算出竖直边缘
			nSum_X = (uchar)*(pDataIn + x - pEdgeImage->widthStep + 1)		//1 * 右上
				+ (uchar)*(pDataIn + x + pEdgeImage->widthStep + 1)		//1	* 右下
				+ (uchar)*(pDataIn + x + 1) * 2							//2 * 右中	
				- uchar(*(pDataIn + x - pEdgeImage->widthStep - 1))		//-1 * 左上
				- uchar(*(pDataIn + x + pEdgeImage->widthStep - 1))		//-1 * 左下
				- uchar(*(pDataIn + x - 1)) * 2;							//-2 * 左中

			//Y方向求导，算出水平边缘
			nSum_Y = uchar(*(pDataIn + pEdgeImage->widthStep + x - 1))		//1 * 下左
				+ uchar(*(pDataIn + pEdgeImage->widthStep + x + 1))		//1 * 下右
				+ uchar(*(pDataIn + pEdgeImage->widthStep + x)) * 2		//2 * 下中
				- uchar(*(pDataIn - pEdgeImage->widthStep + x - 1))		//-1 * 上左
				- uchar(*(pDataIn - pEdgeImage->widthStep + x + 1))		//-1 * 上右
				- uchar(*(pDataIn - pEdgeImage->widthStep + x)) * 2;		//-2 * 上中

			//两方向导数结合,也可 sqrt(abs(nSum_X) * abs(nSum_X) + abs(nSum_Y) * abs(nSum_Y))
			//但abs(nSum_X) + abs(nSum_Y)能求出近似值，且更快
			nSum = abs(nSum_X) + abs(nSum_Y);
			if (nSum > 255)
			{
				nSum = 255;
			}
			*(pDataOut + i) = char(nSum);
		}

		pDataIn += pEdgeImage->widthStep;
		pDataOut += pOutputImg->widthStep;
	}
	cvReleaseImage(&pEdgeImage);
	return bValid;
}

double CalMAD(IplImage *pImage_pre, IplImage *pImage, int X, int Y, int difX, int difY, int nBlockSize)
{
	double result = 0.0;
	int i, j;
	int nBlockPix = nBlockSize * nBlockSize;
	uchar *pImgData_pre = (uchar *)pImage_pre->imageData;
	uchar *pImgData = (uchar *)pImage->imageData;

	X *= nBlockSize;
	Y *= nBlockSize;
	difX += X;
	difY += Y;

	for (j = 0; j < nBlockSize; j++)
	{
		for (i = 0; i < nBlockSize; i++)
		{
			if (X + i >= pImage->width || difX + i >= pImage->width || Y + j >= pImage->height || difY + j >= pImage->height || difX + i < 0 || difY + j < 0)
			{
				nBlockPix--;
				continue;
			}
			result += abs(
							(int)*(pImgData_pre + X + i + pImage->widthStep * (Y + j)) -
							(int)*(pImgData + difX + i + pImage->widthStep * (difY + j))
							);
		}
	}

	if (!nBlockPix)
	{
		result = 255.0;
	}
	else
	{
		result /= nBlockPix;
	}
	return result;
}

void CopyBlock(IplImage *pImageSrc, IplImage *pImageDst, int x, int y, MotionVect Vector, int nBlockSize)
{
	x *= nBlockSize;
	y *= nBlockSize;
	int X = x + Vector.x;
	int Y = y + Vector.y;
	int j;
	int BlockSize = nBlockSize;
	if (pImageSrc->width - x < BlockSize)
	{
		BlockSize = pImageSrc->width - x;
	}

	if (X < 0)
	{
		x -= X;
		BlockSize += X;
		X = 0;
	}

	if (pImageDst->width - X < BlockSize)
	{
		BlockSize = pImageDst->width - X;
	}

	if (BlockSize <= 0)
	{
		return;
	}

	uchar *pDataSrc = (uchar*)(pImageSrc->imageData + pImageSrc->widthStep * y); // 指向当前处理的数据
	char *pDataDst = pImageDst->imageData + pImageDst->widthStep * Y; // 指向当前输出的数据

	for (j = 0; j < nBlockSize; j++)
	{
		if (y + j >= pImageSrc->height || Y + j >= pImageDst->height)
		{
			break;
		}
		if (Y + j >= 0)
		{
			memcpy(pDataDst + X, pDataSrc + x, BlockSize);
		}
		pDataSrc += pImageSrc->widthStep;
		pDataDst += pImageDst->widthStep;
	}
}

/**
*	function:		Motion_Estimation
*	description:	运动估计
*	parameter:		pInputImg_pre,pInputImg:上一帧图像、当前帧图像
					pOutputImg：利用运动矢量通过上一帧得出当前帧预测结果
					nSchRange:块搜索范围
*	return:			bool值:是否处理成功
**/
bool CImgProcessing::Motion_Estimation(IplImage *pInputImg_pre, IplImage *pInputImg, IplImage *pOutputImg, int nSchRange)
{
	if (NULL == pInputImg_pre || NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//多通道图进行分离处理
	if (pInputImg->nChannels != 1)
	{
		bool bValidR = false, bValidG = false, bValidB = false;
		IplImage *pBlueChannel_pre = cvCreateImage(cvGetSize(pInputImg_pre), pInputImg->depth, 1);
		IplImage *pGreenChannel_pre = cvCreateImage(cvGetSize(pInputImg_pre), pInputImg->depth, 1);
		IplImage *pRedChannel_pre = cvCreateImage(cvGetSize(pInputImg_pre), pInputImg->depth, 1);
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//临时存储输出结果
		IplImage *pTemp;	//指针交换中间变量

		//分离通道
		cvSplit(pInputImg_pre, pBlueChannel_pre, pGreenChannel_pre, pRedChannel_pre, NULL);
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blue通道处理
		//memset(pTempImg->imageData, 0, pTempImg->imageSize);
		bValidB = Motion_Estimation(pBlueChannel_pre, pBlueChannel, pTempImg, nSchRange);			//嵌套调用函数
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//输出结果在pTempImg,指针交换
		//Green通道处理
		//memset(pTempImg->imageData, 0, pTempImg->imageSize);
		bValidG = Motion_Estimation(pGreenChannel_pre, pGreenChannel, pTempImg, nSchRange);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Red通道处理
		//memset(pTempImg->imageData, 0, pTempImg->imageSize);
		bValidR = Motion_Estimation(pRedChannel_pre, pRedChannel, pTempImg, nSchRange);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//合成通道
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel_pre);
		cvReleaseImage(&pGreenChannel_pre);
		cvReleaseImage(&pRedChannel_pre);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//单通道直接处理
	bool bValid = TRUE;

	int nBlockSize = nSchRange / 2;
	int nBlockWidthX = pInputImg->width % nBlockSize ? pInputImg->width / nBlockSize + 1 : pInputImg->width / nBlockSize;
	int nBlockHeightY = pInputImg->height % nBlockSize ? pInputImg->height / nBlockSize + 1 : pInputImg->height / nBlockSize;
	int x, y, i ,j;
	double miniMAD, MAD;
	//pMotionVect pVector = new MotionVect[nBlockWidthX * nBlockHeightY];
	MotionVect Vector;

	cvCopyImage(pInputImg_pre, pOutputImg);
	//memset(pOutputImg->imageData, 0, pOutputImg->imageSize);
	//MotionVect test = {30,30};
	//CopyBlock(pInputImg, pOutputImg, 0, 0, test, 900);

	for (y = 0; y < nBlockHeightY; y++)
	{
		for (x = 0; x < nBlockWidthX; x++)
		{
			miniMAD = CalMAD(pInputImg_pre, pInputImg, x, y, 0, 0, nBlockSize);
			Vector.x = 0;
			Vector.y = 0;
			for (j = -nBlockSize; j < nBlockSize; j++)
			{
				for (i = -nBlockSize; i < nBlockSize; i++)
				{
					if (i || j)
					{
						//计算与前一帧图像最接近的块
						MAD = CalMAD(pInputImg_pre, pInputImg, x, y, i, j, nBlockSize);
						if (MAD < miniMAD)
						{
							miniMAD = MAD;
							//(pVector + x + y * nBlockWidthX)->x = i;
							//(pVector + x + y * nBlockWidthX)->y = j;
							Vector.x = i;
							Vector.y = j;
						}
					}
				}
			}
			if (Vector.x || Vector.y)
			{
				CopyBlock(pInputImg_pre, pOutputImg, x, y, Vector, nBlockSize);
			}
		}
	}
	
	//delete[] pVector;
	return bValid;
}



void CImgProcessing::exchangeNum(uchar *num1, uchar *num2)
{
	uchar temp;
	temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}

void CImgProcessing::Bubble_Sort(uchar* pDataList, unsigned int nSize)
{
	bool flag;
	for (unsigned int i = 0; i < nSize - 1; i++)
	{
		flag = TRUE;
		for (unsigned int j = 0; j < nSize - 1 - i; j++)
		{
			if (*(pDataList + j) > *(pDataList + j + 1))
			{
				exchangeNum((pDataList + j), (pDataList + j + 1));
				flag = FALSE;
			}
		}
		if (flag)	//flag为TRUE则已经按顺序排好，无须再排
		{
			break;
		}
	}
}

void CImgProcessing::Selection_Sort(uchar* pDataList, unsigned int nSize)
{
	unsigned int min_index;
	//选择最小的数放在最前
	for (unsigned int i = 0; i < nSize - 1; i++)
	{
		min_index = i;
		for (unsigned int j = i + 1; j < nSize; j++)
		{
			if (*(pDataList + min_index) > *(pDataList + j))
			{
				min_index = j;
			}
		}
		exchangeNum((pDataList + i), (pDataList + min_index));
	}
}

void CImgProcessing::Insertion_Sort(uchar* pDataList, unsigned int nSize)
{
	uchar nCompare;	//待插入的数，与前面的数逐个比较
	unsigned int nData_Index;		//待插入的数的索引
	unsigned int nInser_Index;		//插入点索引

	for (nData_Index = 1; nData_Index < nSize; nData_Index++)
	{
		nCompare = *(pDataList + nData_Index);
		nInser_Index = nData_Index;

		//将前面所有比待插入的数大的数移到后面
		while (nInser_Index > 0 && *(pDataList + nInser_Index - 1) > nCompare)
		{
			*(pDataList + nInser_Index) = *(pDataList + nInser_Index - 1);
			nInser_Index--;
		}
		//找到小于等于该数的位置，插入
		*(pDataList + nInser_Index) = nCompare;
	}
}

void CImgProcessing::Quick_Sort(uchar* pDataList, unsigned int nSize)
{
	bool cross = FALSE;
	unsigned int end = nSize - 1;	//数组末索引
	unsigned int left = 1, right = end;
	if (nSize != 1)								//衹有一個節點時不需排序 
	{
		while (1)
		{
			while (*(pDataList + left) <= *(pDataList))		//從左至右搜尋比基準值大的數 
			{
				if (left == right)				//與右指標交叉，記錄下來 
				{
					cross = TRUE;
					break;
				}
				left++;
			}
			while (*(pDataList + right) > *(pDataList))		//從右至左搜尋小於等於基準值的數 
			{
				if (left == right)
				{
					cross = TRUE;
				}
				right--;
			}
			if (cross)
			{
				break;							//若發生了交叉，此時右指標及其左邊的值都小於基準值，右邊大於基準值， 
			}									//此時應跳出回圈，將右指標指向的值與基準值互換 
			exchangeNum((pDataList + left), (pDataList + right));			//若沒交叉，左指標與右指標指向的值互換 
		}
		exchangeNum(pDataList, (pDataList + right));
		if (0 != right)						//此時右指標指向的值在正確位置（左邊的值都比它小，右邊比它大） 
		{										//遞回再次對左右兩邊進行quick sort 
			Quick_Sort(pDataList, right);
		}
		if (right != end)
		{
			Quick_Sort((pDataList + right + 1), end - right);
		}
	}
}

void CImgProcessing::Shell_Sort(uchar* pDataList, unsigned int nSize)
{
	//分组,每组2个进行插入排序，再分成每组4个...类推
	unsigned int incr = nSize / 2;
	unsigned int i;
	int j;
	while (incr > 0)
	{
		for (i = incr; i < nSize; i++)	//类似插入排序可从第1个开始进行插入,这里从每组第1个开始
		{
			j = i - incr;	//选取该组上一个进行比较
			while (j >= 0)
			{
				if (*(pDataList + j) > *(pDataList + j + incr))
				{
					exchangeNum((pDataList + j), (pDataList + j + incr));
					j -= incr;
				}
				else
				{
					break;		//每组每次都只排一个数
				}
			}
		}		//最后将数插入的该组合适的位置，原理同插入排序
		incr /= 2;
	}
}

void CImgProcessing::RadixMSD_Sort(uchar* pDataList, unsigned int nSize)
{
	unsigned int radix = 0;
	unsigned int nData_Index;		//寻找基数相同的数据的索引
	unsigned int nInser_Index = 0;		//插入点索引
	while (nInser_Index < nSize)
	{
		for (nData_Index = nInser_Index; nData_Index < nSize; nData_Index++)
		{
			if (radix == *(pDataList + nData_Index) / 100)
			{
				exchangeNum(pDataList + nInser_Index, pDataList + nData_Index);
				nInser_Index++;
			}
		}
		radix++;
	}
	Insertion_Sort(pDataList, nSize);
}

uchar CImgProcessing::Get_Hist_Med(unsigned int* pnHist, unsigned int nMed)
{
	unsigned long nSum = 0;
	for (unsigned int i = 0; i < 256; i++)
	{
		nSum += *(pnHist + i);
		if (nSum > nMed)
		{
			return uchar(i);
		}
	}
	return uchar(0);
}