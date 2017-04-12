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
*	description:	��IplImage��ʽת����HBITMAP��ʽ
*	parameter:		IplImage* pImg:IplImageͼ��ָ��
*	return:			HBITMAP:λͼͼ��ָ��
**/
HBITMAP  CImgProcessing::IplImage2hBitmap(IplImage* pImg, int nWidth, int nHeight)
{
	HBITMAP hBmp;
	IplImage* dst = cvCreateImage(cvSize(nWidth, nHeight), pImg->depth, pImg->nChannels);
	cvResize(pImg, dst);			//�����趨��С�����µ�ͼ��
	hBmp = IplImage2hBitmap(dst);	//��ͼ��ת����hbitmap����
	cvReleaseImage(&dst);		//�����ʱͼ���ڴ�
	return hBmp;
}

/**
*	function:		IplImage2hBitmap
*	description:	��IplImage��ʽת����ָ���ߴ�HBITMAP��ʽ
*	parameter:		IplImage* pImg:IplImageͼ��ָ��
nWidth, nHeight:���λͼ��ȸ߶�
*	return:			HBITMAP:λͼͼ��ָ��
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
*	description:	��IplImage��ʽת����ָ���ߴ�BITMAPINFO��ʽ
*	parameter:		IplImage* pImg:IplImageͼ��ָ��
width, height:ͼ���ȸ߶�
bpp:ͼ��λ���
*	return:			BITMAPINFO*:λͼͼ��ָ��
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



//IplImage* AddPepperSaltNoise(IplImage* src)    //��ӽ�������������ڰ׵�
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
//double Normal(double x, double miu, double sigma) //�����ܶȺ���
//{
//	return 1.0 / sqrt(2 * 3.1415926*sigma) * exp(-1 * (x - miu)*(x - miu) / (2 * sigma*sigma));
//}
//double NormalRandom(double miu,double sigma, double min, double max)//������̬�ֲ������
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
*	description:	��Ӹ�˹����
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
					fNoisePSNR:�����ͼ��ķ�ֵ�����
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::AddGuassianNoise(IplImage *pInputImg, IplImage *pOutputImg, float fNoisePSNR)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//��ͨ��ͼ���з��봦��
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//��ʱ�洢������
		IplImage *pTemp;	//ָ�뽻���м����

		//����ͨ��
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blueͨ������
		bValidB = AddGuassianNoise(pBlueChannel, pTempImg, fNoisePSNR);	//Ƕ�׵��ú���
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//��������pTempImg,ָ�뽻��
		//Greenͨ������
		bValidG = AddGuassianNoise(pGreenChannel, pTempImg, fNoisePSNR);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Redͨ������
		bValidR = AddGuassianNoise(pRedChannel, pTempImg, fNoisePSNR);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//�ϳ�ͨ��
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//��ͨ��ֱ�Ӵ���
	bool bValid = TRUE;
	double dVarSum = 0.0;
	double dRandParm;
	//���������źű�׼��
	dRandParm = 255 / pow(10.0f, fNoisePSNR / 20.0f);

	IplImage* pNoise = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
	//���ɾ�ֵ0����׼��Ϊ�����źű�׼��ĸ�˹�ֲ�����
	cvRandArr(&m_rng, pNoise, CV_RAND_NORMAL, cvScalarAll(0), cvScalarAll(dRandParm));
	//����
	cvAdd(pInputImg, pNoise, pOutputImg);
	cvReleaseImage(&pNoise);

	return bValid;
}

/**
*	function:		AddPepperSaltNoise
*	description:	��ӽ�������
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
					fNoisePSNR:�����ͼ��ķ�ֵ�����
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::AddPepperSaltNoise(IplImage *pInputImg, IplImage *pOutputImg, float fNoisePSNR)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//��ͨ��ͼ���з��봦��
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//��ʱ�洢������
		IplImage *pTemp;	//ָ�뽻���м����

		//����ͨ��
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blueͨ������
		bValidB = AddPepperSaltNoise(pBlueChannel, pTempImg, fNoisePSNR);	//Ƕ�׵��ú���
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//��������pTempImg,ָ�뽻��
		//Greenͨ������
		bValidG = AddPepperSaltNoise(pGreenChannel, pTempImg, fNoisePSNR);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Redͨ������
		bValidR = AddPepperSaltNoise(pRedChannel, pTempImg, fNoisePSNR);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//�ϳ�ͨ��
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//��ͨ��ֱ�Ӵ���
	bool bValid = TRUE;
	double dVarSum = 0.0;

	double dNoiseValSum;
	//���㷽����������ظ����õ�������ֵ
	dNoiseValSum = 255 * 255 / pow(10.0f, fNoisePSNR / 10.0f) * pInputImg->width * pInputImg->height;
	int nNoiseX, nNoiseY, nNoiseVal, nIndex;
	uchar *pInput = (uchar *)pInputImg->imageData;
	uchar *pOutput = (uchar *)pOutputImg->imageData;

	cvCopy(pInputImg, pOutputImg);

	//һֱ��������Ľ���������ֱ���ﵽ�趨ֵ
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
*	description:	�Ҷȷ�ת����
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::GrayReversion(IplImage *pInputImg, IplImage *pOutputImg)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;

	bool bValid = TRUE;

	//�Ż�ǰ�����ȡ��
	/*for (int i = 0; i < pInputImg->imageSize; i++)
	{
		*(pOutputImg->imageData + i) = char(255 - (uchar)*(pInputImg->imageData + i));
	}*/
	
	//�Ż���,4���ֽ�һ��ȡ��
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
*	description:	ͼ������任���� s = c*log(1 + r)
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
fConst:�����任����
*	return:			boolֵ:�Ƿ���ɹ�
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
*	description:	ͼ�񷴶����任���� r = c*log(1 + s)  s = exp(r/c) - 1
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
fConst:�����任����
*	return:			boolֵ:�Ƿ���ɹ�
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
*	description:	(���)ͼ������任���� s = c*log(1 + r)
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
fConst:�����任����
*	return:			boolֵ:�Ƿ���ɹ�
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
*	description:	(���)ͼ�񷴶����任���� r = c*log(1 + s)  s = exp(r/c) - 1
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
fConst:�����任����
*	return:			boolֵ:�Ƿ���ɹ�
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
*	description:	ڤ�α任����
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
fGamma:ڤ�α任����Gamme
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::GammaCorrection(IplImage *pInputImg, IplImage *pOutputImg, float fGamma)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;

	bool bValid = TRUE;
	float fVal = 0.0f, fScale = 255.0f;


	for (int i = 0; i < pInputImg->imageSize; i++)
	{
		fVal = (uchar)*(pInputImg->imageData + i);
		fVal /= fScale;		//ӳ��ֵ��0~1,�任��Gamma>1�������խ����ֵӳ�䵽��������ֵ
		fVal = pow(fVal, fGamma);		//Gamma<1��������ֵӳ��Ϊ���
		fVal *= fScale;					//ӳ���0~255
		if (fVal > 255.0f) fVal = 255.0f;	//�ж����
		*(pOutputImg->imageData + i) = char(fVal);
	}
	return bValid;
}

/**
*	function:		GammaCorrection_Table
*	description:	(���)ڤ�α任����
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
fGamma:ڤ�α任����Gamme
*	return:			boolֵ:�Ƿ���ɹ�
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
		fVal = float(i) / fScale;		//ӳ��ֵ��0~1,�任��Gamma>1�������խ����ֵӳ�䵽��������ֵ
		fVal = pow(fVal, fGamma);		//Gamma<1��������ֵӳ��Ϊ���
		fVal *= fScale;					//ӳ���0~255
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
*	description:	ͳ�ƻҶ�ֱ��ͼ
*	parameter:		pInputImg:��Ҫͳ��ֱ��ͼͼ��ָ��
pHist:���ֱ��ͼ����ָ��
pMax:ֱ��ͼ���ֵ
*	return:			boolֵ:�Ƿ���ɹ�
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
*	description:	RGBתYUV������
*	parameter:		pImgBGR:����RGBͼ��ָ��
					pImgY,pImgU,pImgV:�����YUVͼ��ָ��
*	return:			boolֵ:�Ƿ���ɹ�
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
*	description:	YUVתRGB
*	parameter:		pImgBGR:���RGBͼ��ָ��
					pImgY,pImgU,pImgV:�����YUVͼ��ָ��
*	return:			boolֵ:�Ƿ���ɹ�
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
*	description:	ֱ��ͼ���⻯
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::EqualizeHist(IplImage *pInputImg, IplImage *pOutputImg)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//��ͨ��ͼ���з��봦��
	if (pInputImg->nChannels == 3)
	{
		bool bValid = TRUE;
		IplImage;
		IplImage *pChannelY = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pChannelU = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pChannelV = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pResult = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//��ʱ�洢������

		//����ͨ��
		BGR2YUV(pInputImg, pChannelY, pChannelU, pChannelV);
		bValid = EqualizeHist(pChannelY, pResult);
		//�ϳ�ͨ��
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
	//ͳ��ֱ��ͼ
	for (int i = 0; i < pInputImg->height; i++)
	{
		for (int j = 0; j < pInputImg->width; j++)
		{
			(*(TempHist + *(pInputData + j)))++;
		}
		pInputData += pInputImg->widthStep;
	}

	//�Ҷȼ�ӳ��,���ʻ���(�ۼ�)
	float fGrayMap = 255.0f / size;	//ӳ��ϵ��,����size��һ��,��255ӳ�䵽0~255
	*(TempHist) *= fGrayMap;		//�Ҷȼ�ӳ��,��һ�����û���
	for (int i = 1; i < 256; i++)
	{
		*(TempHist + i) *= fGrayMap;	//�Ҷȼ�ӳ��
		*(TempHist + i) += *(TempHist + i - 1);	//�ۼ�
	}

	//���ݻ������·ֲ��Ҷȼ����Ի���ֵ��Ϊ�Ҷȼ�(ȡ����ֵ��ӽ��ĻҶȼ�)
	pInputData = (uchar*)pInputImg->imageData;
	for (int i = 0; i < pInputImg->height; i++)
	{
		for (int j = 0; j < pInputImg->width; j++)
		{	//���Ҷȼ���Ϊ��ǰ���ػҶȼ��Ļ���ֵ
			*(pOutputData + j) = char(int(*(TempHist + *(pInputData + j)) + 0.5));
		}
		pInputData += pInputImg->widthStep;
		pOutputData += pOutputImg->widthStep;
	}
	return bValid;
}

/**
*	function:		Smooth_Blur
*	description:	��ֵ�˲�
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
nSize:�˲�ģ���С
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::Smooth_Blur(IplImage *pInputImg, IplImage *pOutputImg, int nSize)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//��ͨ��ͼ���з��봦��
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//��ʱ�洢������
		IplImage *pTemp;	//ָ�뽻���м����

		//����ͨ��
		cvSplit(pInputImg,pBlueChannel,pGreenChannel,pRedChannel,NULL);

		//Blueͨ������
		bValidB = Smooth_Blur(pBlueChannel, pTempImg, nSize);		//Ƕ�׵��ú���
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//��������pTempImg,ָ�뽻��
		//Greenͨ������
		bValidG = Smooth_Blur(pGreenChannel, pTempImg, nSize);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Redͨ������
		bValidR = Smooth_Blur(pRedChannel, pTempImg, nSize);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//�ϳ�ͨ��
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//��ͨ��ֱ�Ӵ���
	bool bValid = TRUE;
	unsigned long nSum = 0;	//���غ�
	int nOffSet = nSize / 2;
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;
	unsigned int nArea = nSize * nSize;

	//�߽���չ
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);

	char *pDataOut = pOutputImg->imageData; // ָ��ǰ���������

	int x, y;
	for (int j = 0; j < nHeight; j++)	//ͼ���ж�λ
	{
		for (int i = 0; i < nWidth; i++)	//ͼ���ж�λ
		{
			for (int m = -nOffSet; m <= nOffSet; m++)	//ģ���ж�λ
			{
				for (int n = -nOffSet; n <= nOffSet; n++)	//ģ���ж�λ
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
*	description:	����ͼ��ֵ�˲�
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
nSize:�˲�ģ���С
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::Smooth_Blur_Table(IplImage *pInputImg, IplImage *pOutputImg, int nSize)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//��ͨ��ͼ���з��봦��
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//��ʱ�洢������
		IplImage *pTemp;	//ָ�뽻���м����

		//����ͨ��
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blueͨ������
		bValidB = Smooth_Blur_Table(pBlueChannel, pTempImg, nSize);			//Ƕ�׵��ú���
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//��������pTempImg,ָ�뽻��
		//Greenͨ������
		bValidG = Smooth_Blur_Table(pGreenChannel, pTempImg, nSize);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Redͨ������
		bValidR = Smooth_Blur_Table(pRedChannel, pTempImg, nSize);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//�ϳ�ͨ��
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//��ͨ��ֱ�Ӵ���
	bool bValid = TRUE;
	unsigned long nSum = 0;	//���غ�
	int nOffSet = nSize / 2;
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;
	unsigned int nArea = nSize * nSize;

	//�߽���չ
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);

	ULONG *pTable = new ULONG[(nWidth + n2OffSet) * (nHeight + n2OffSet)];	//����ͼ

	*(pTable) = *(pEdgeImage->imageData);	//����ͼ��һ��Ԫ��

	//���ɻ���ͼ
	for (int i = 1; i < nEdgeWidth; i++)	//����ͼ��������
	{	//����
		*(pTable + i) = *(pTable + i - 1) + (uchar)*(pEdgeImage->imageData + i);
	}
	for (int j = 1; j < nEdgeHeight; j++)
	{	//����
		*(pTable + j * nEdgeWidth) = *(pTable + (j - 1) * nEdgeWidth) + (uchar)*(pEdgeImage->imageData + j * pEdgeImage->widthStep);
	}

	for (int j = 1; j < nEdgeHeight; j++)		//����ͼ����Ԫ��
	{
		for (int i = 1; i < nEdgeWidth; i++)
		{
			*(pTable + j * nEdgeWidth + i) = (uchar)*(pEdgeImage->imageData + j * pEdgeImage->widthStep + i) +
				*(pTable + (j - 1) * nEdgeWidth + i) -
				*(pTable + (j - 1) * nEdgeWidth + (i - 1)) +
				*(pTable + j * nEdgeWidth + i - 1);
		}
	}


	//����ͼ��
	//��Ԫ��
	*(pOutputImg->imageData) = char(*(pTable + n2OffSet * nEdgeWidth + n2OffSet) / nArea);
	//�������
	for (int i = 1; i < nWidth; i++)
	{
		nSum = *(pTable + n2OffSet * nEdgeWidth + i + n2OffSet) -
			*(pTable + n2OffSet * nEdgeWidth + i - 1);
		*(pOutputImg->imageData + i) = char(nSum / nArea);
	}


	for (int j = 1; j < nHeight; j++)
	{
		//�������
		nSum = *(pTable + (j + n2OffSet) * nEdgeWidth + n2OffSet) -
			*(pTable + (j - 1) * nEdgeWidth + n2OffSet);
		*(pOutputImg->imageData + j * pOutputImg->widthStep) = char(nSum / nArea);
		//����Ԫ�����
		for (int i = 1; i < nWidth ; i++)
		{
			nSum = *(pTable + (j + n2OffSet) * nEdgeWidth + i + n2OffSet) -
				*(pTable + (j + n2OffSet) * nEdgeWidth + i - 1) +	//i - 1 = x - nOffset - 1
				*(pTable + (j - 1) * nEdgeWidth + i - 1) -			//j - 1 = y - nOffset - 1
				*(pTable + (j - 1) * nEdgeWidth + i + n2OffSet);
			*(pOutputImg->imageData + j * pOutputImg->widthStep + i) = char(nSum / nArea);
		}
	}

	delete[] pTable;		//�ͷŻ���ͼ
	cvReleaseImage(&pEdgeImage);
	return bValid;

}


/**
*	function:		Median_Blur_Sort
*	description:	��ֵ�˲�(��������)
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
nSize:�˲�ģ���С
pSortFunc:ָ��������������ָ��
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::Median_Blur_Sort(IplImage *pInputImg, IplImage *pOutputImg, int nSize, void (CImgProcessing::*pSortFunc)(uchar*, unsigned int))
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//��ͨ��ͼ���з��봦��
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//��ʱ�洢������
		IplImage *pTemp;	//ָ�뽻���м����

		//����ͨ��
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blueͨ������
		bValidB = Median_Blur_Sort(pBlueChannel, pTempImg, nSize, pSortFunc);		//Ƕ�׵��ú���
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//��������pTempImg,ָ�뽻��
		//Greenͨ������
		bValidG = Median_Blur_Sort(pGreenChannel, pTempImg, nSize, pSortFunc);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Redͨ������
		bValidR = Median_Blur_Sort(pRedChannel, pTempImg, nSize, pSortFunc);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//�ϳ�ͨ��
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//��ͨ��ֱ�Ӵ���
	bool bValid = TRUE;
	int nOffSet = nSize / 2;
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;
	unsigned int nArea = nSize * nSize;
	int nMedian = nArea / 2;

	//�߽���չ
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);

	uchar *pchKernel = new uchar[nArea];	//���ģ�����ݽ�������
	uchar *pKernel_Index = pchKernel;		//��������
	char *pDataOut = pOutputImg->imageData; // ָ��ǰ���������

	int x, y;
	for (int j = 0; j < nHeight; j++)	//ͼ���ж�λ
	{
		for (int i = 0; i < nWidth; i++)	//ͼ���ж�λ
		{
			pKernel_Index = pchKernel;
			for (int m = -nOffSet; m <= nOffSet; m++)	//ģ���ж�λ
			{
				for (int n = -nOffSet; n <= nOffSet; n++)	//ģ���ж�λ
				{
					y = j + nOffSet + m;
					x = i + nOffSet + n;
					*(pKernel_Index++) = (uchar)*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x);
				}
			}
			(this->*pSortFunc)(pchKernel, nArea);
			*(pDataOut + i) = char(*(pchKernel + nMedian));	//ȡ��ֵ
		}
		pDataOut += pOutputImg->widthStep;
	}

	delete[] pchKernel;
	cvReleaseImage(&pEdgeImage);
	return bValid;
}



/**
*	function:		Median_Blur_Hist
*	description:	��ֵ�˲�(����ֱ��ͼ)
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
nSize:�˲�ģ���С
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::Median_Blur_Hist(IplImage *pInputImg, IplImage *pOutputImg, int nSize)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//��ͨ��ͼ���з��봦��
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//��ʱ�洢������
		IplImage *pTemp;	//ָ�뽻���м����

		//����ͨ��
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blueͨ������
		bValidB = Median_Blur_Hist(pBlueChannel, pTempImg, nSize);		//Ƕ�׵��ú���
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//��������pTempImg,ָ�뽻��
		//Greenͨ������
		bValidG = Median_Blur_Hist(pGreenChannel, pTempImg, nSize);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Redͨ������
		bValidR = Median_Blur_Hist(pRedChannel, pTempImg, nSize);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//�ϳ�ͨ��
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//��ͨ��ֱ�Ӵ���
	bool bValid = TRUE;
	int nOffSet = nSize / 2;
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;
	unsigned int nArea = nSize * nSize;
	unsigned int nMedian = nArea / 2;

	//�߽���չ
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);

	int x, y;
	char *pDataOut = pOutputImg->imageData; // ָ��ǰ���������


	unsigned int **pnHist = new unsigned int*[nWidth];	//Ϊÿһ�з���һ��ֱ��ͼ���ݿ�
	if (pnHist == NULL)
	{
		return FALSE;
	}


	//��ʼ���׸�ֱ��ͼ(0�У�0��)
	*(pnHist) = new unsigned int[256];	//����ֱ��ͼ����
	memset(*(pnHist), 0, sizeof(unsigned int) * 256);
	for (int m = -nOffSet; m <= nOffSet; m++)	//ģ���ж�λ
	{
		for (int n = -nOffSet; n <= nOffSet; n++)	//ģ���ж�λ
		{
			y = m + nOffSet;
			x = n + nOffSet;
			//��Ӧֱ��ͼ����Ӧ�Ҷȼ�����
			(*(*(pnHist) + uchar(*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x))))++;
		}
	}
	*(pDataOut) = char(Get_Hist_Med(*(pnHist),nMedian));	//ȡ��ֵ

	//��ʼ������ʣ��ֱ��ͼ
	for (int i = 1; i < nWidth; i++)	//ͼ���ж�λ
	{
		*(pnHist + i) = new unsigned int[256];	//����ֱ��ͼ����
		memcpy(*(pnHist + i), *(pnHist + i - 1), sizeof(unsigned int) * 256);	//�������ֱ��ͼ����
		for (int m = -nOffSet; m <= nOffSet; m++)	//ģ���ж�λ
		{
			y = m + nOffSet;
			//��ȥ������������
			x = i - 1; //i - 1 - nOffSet + nOffSet
			(*(*(pnHist + i) + (uchar)*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x)))--;

			//���ϵ�ǰ���������
			x = i + nOffSet + nOffSet;
			(*(*(pnHist + i) + (uchar)*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x)))++;
		}
		*(pDataOut + i) = char(Get_Hist_Med(*(pnHist + i), nMedian));	//ȡ��ֵ
	}

	pDataOut += pOutputImg->widthStep;

	
	for (int j = 1; j < nHeight; j++)	//ͼ���ж�λ
	{
		for (int i = 0; i < nWidth; i++)	//ͼ���ж�λ
		{

			for (int n = -nOffSet; n <= nOffSet; n++)	//ģ���ж�λ
			{
				
				x = i + n + nOffSet;
				//��ȥ��һ��������
				y = j - 1; //j - 1 - nOffSet + nOffSet
				(*(*(pnHist + i) + (uchar)*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x)))--;

				//���ϵ�ǰ��������
				y = j + nOffSet + nOffSet;
				(*(*(pnHist + i) + (uchar)*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x)))++;
			}
			*(pDataOut + i) = char(Get_Hist_Med(*(pnHist + i), nMedian));	//ȡ��ֵ
		}
		pDataOut += pOutputImg->widthStep;
	}


	for (int i = 0; i < nWidth; i++)
	{
		delete[] * (pnHist + i);		//�ͷ�ֱ��ͼ����
	}

	delete[] pnHist;		//�ͷ�ֱ��ͼָ������
	cvReleaseImage(&pEdgeImage);

	return bValid;
}





/**
*	function:		Smooth_Gaussian
*	description:	��˹�˲�
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
nSize:�˲�ģ���С
fSigma:��˹�ֲ�����
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::Smooth_Gaussian(IplImage *pInputImg, IplImage *pOutputImg, int nSize,float fSigma)
{ 
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//��ͨ��ͼ���з��봦��
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//��ʱ�洢������
		IplImage *pTemp;	//ָ�뽻���м����

		//����ͨ��
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blueͨ������
		bValidB = Smooth_Gaussian(pBlueChannel, pTempImg, nSize, fSigma);	//Ƕ�׵��ú���
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//��������pTempImg,ָ�뽻��
		//Greenͨ������
		bValidG = Smooth_Gaussian(pGreenChannel, pTempImg, nSize, fSigma);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Redͨ������
		bValidR = Smooth_Gaussian(pRedChannel, pTempImg, nSize, fSigma);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//�ϳ�ͨ��
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//��ͨ��ֱ�Ӵ���
	bool bValid = TRUE;

	int nOffSet = nSize / 2;                //�����˲��������ĵ�����  
	int nArea = nSize * nSize;
	float *pfKernel = new float[nArea];
	float  fSum = 0.0f;                       //��ͣ����й�һ��������Ȩֵ    
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;

	//��ǰ��������ݣ����ټ�����
	float fSigma_2pow = pow(fSigma, 2.0f);	//Sigmaƽ��
	float f_2PiSigma_2pow = 2.0f * 3.1415926f * fSigma_2pow;	//2*Pi*Sigmaƽ��
	float fGaussianCoeff = -0.5f / fSigma_2pow;	//��˹ϵ��
	float fResult = 0.0f;	//��ʱ��Ž��

	//�߽���չ
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);

	//���ɶ�ά�ĸ�˹�˲�ϵ��  
	////////////////////��ά��˹������ʽ/////////////////////////////
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
			fSum += fResult;	//�����˹ϵ���ͣ����ڼ���Ȩֵ
		}
	}
	//���й�һ��,����Ȩֵ
	for (int i = 0; i < nArea; i++)
	{
		*(pfKernel + i) /= fSum;
	}

	char *pDataOut = pOutputImg->imageData; // ָ��ǰ���������
	int x, y;	//�С��ж�λֵ
	fResult = 0;
	for (int j = 0; j < nHeight; j++)	//ͼ���ж�λ
	{
		for (int i = 0; i < nWidth; i++)	//ͼ���ж�λ
		{
			for (int m = -nOffSet; m <= nOffSet; m++)	//ģ���ж�λ
			{
				for (int n = -nOffSet; n <= nOffSet; n++)	//ģ���ж�λ
				{
					y = j + nOffSet + m;
					x = i + nOffSet + n;
					fResult += *(pfKernel + nOffSet + n + (nOffSet + m) * nSize) * 
						(uchar)*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x);
				}
			}
			//ǰ���Ѿ����˹�һ������ֱ��������
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
*	description:	˫���˲�
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
nSize:�˲�ģ���С
fSigmaColor:˫���˲���ɫSigma
fSigmaSpace:˫���˲��ؼ�Sigma
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::BilateralFilter(IplImage *pInputImg, IplImage *pOutputImg, int nSize, float fSigmaColor, float fSigmaSpace)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//��ͨ��ͼ���з��봦��
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//��ʱ�洢������
		IplImage *pTemp;	//ָ�뽻���м����

		//����ͨ��
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blueͨ������
		bValidB = BilateralFilter(pBlueChannel, pTempImg, nSize, fSigmaColor, fSigmaSpace);			//Ƕ�׵��ú���
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//��������pTempImg,ָ�뽻��
		//Greenͨ������
		bValidG = BilateralFilter(pGreenChannel, pTempImg, nSize, fSigmaColor, fSigmaSpace);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Redͨ������
		bValidR = BilateralFilter(pRedChannel, pTempImg, nSize, fSigmaColor, fSigmaSpace);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//�ϳ�ͨ��
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//��ͨ��ֱ�Ӵ���
	bool bValid = TRUE;

	int nOffSet = nSize / 2;                //�����˲��������ĵ�����  
	int nArea = nSize * nSize;
	float  fSum = 0.0f;                       //��ͣ����й�һ��������Ȩֵ    
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;

	//�߽���չ
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);


	float* fDistDiff = new float[nArea]; //��������м��ļ��ξ���  
	float* fColorDiff = new float[nArea]; //�������ĵ㵽��ǰ��ĻҶȲ�  

	//��ǰ��������ݣ����ټ�����
	float fGaussianCoeff_Space = -0.5f / pow(fSigmaSpace, 2.0f);
	float fGaussianCoeff_Color = -0.5f / pow(fSigmaColor, 2.0f);


	unsigned int nIndex;

	nIndex = 0;
	for (int y = -nOffSet; y <= nOffSet; y++)
	{
		for (int x = -nOffSet; x <= nOffSet; x++)
		{
			*(fDistDiff + nIndex++) = exp(fGaussianCoeff_Space * (x * x + y * y));   //C����  
		}
	}
	
	
	//�������Ҷ�ֵ�Ĳ�  
	uchar *pDataOut = (uchar*)(pOutputImg->imageData); // ָ��ǰ���������
	int x, y;
	for (int j = 0; j < nHeight; j++)
	{
		for (int i = 0; i < nWidth; i++)
		{
			float fColor = (float)uchar(*(pEdgeImage->imageData + (j + nOffSet) * pEdgeImage->widthStep + i + nOffSet));  //��ǰ��ĻҶ�ֵ   
			float fData = 0.0f;
			fSum = 0.0f;                      //���ڽ��й�һ��   

			nIndex = 0;
			for (int m = -nOffSet; m <= nOffSet; m++)	//ģ���ж�λ
			{
				for (int n = -nOffSet; n <= nOffSet; n++)	//ģ���ж�λ
				{
					y = j + nOffSet + m;
					x = i + nOffSet + n;

					float fColor1 = (float)uchar(*(pEdgeImage->imageData + y * pEdgeImage->widthStep + x));
					float fColorDis = fColor - fColor1;
					*(fColorDiff + nIndex) = exp(fGaussianCoeff_Color * fColorDis * fColorDis); //S����  

					//if (m || n)
					{
						float fColorDiffxDistDiff = *(fColorDiff + nIndex) * *(fDistDiff + nIndex);
						fData += fColor1 * fColorDiffxDistDiff; //C��S�����ۺ�  
						fSum += fColorDiffxDistDiff;    //��Ȩϵ����ͣ����й�һ��  
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
*	description:	�ɷ����˹�˲�(����λ��˹�˲����һά������)
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
nSize:�˲�ģ���С
fSigma:��˹�ֲ�����
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::Smooth_Gaussian_SepFilter(IplImage *pInputImg, IplImage *pOutputImg, int nSize, float fSigma)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//��ͨ��ͼ���з��봦��
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//��ʱ�洢������
		IplImage *pTemp;	//ָ�뽻���м����

		//����ͨ��
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blueͨ������
		bValidB = Smooth_Gaussian_SepFilter(pBlueChannel, pTempImg, nSize, fSigma);			//Ƕ�׵��ú���
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//��������pTempImg,ָ�뽻��
		//Greenͨ������
		bValidG = Smooth_Gaussian_SepFilter(pGreenChannel, pTempImg, nSize, fSigma);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Redͨ������
		bValidR = Smooth_Gaussian_SepFilter(pRedChannel, pTempImg, nSize, fSigma);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//�ϳ�ͨ��
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//��ͨ��ֱ�Ӵ���
	bool bValid = TRUE;
	int nWidth = pInputImg->width;		//ͼ����
	int nHeight = pInputImg->height;	//ͼ��߶�
	int nOffSet = (nSize) / 2;                //�����˲��������ĵ����� 
	float *pfKernel = new float[nSize];		 //�����ڴ���˲�ģ��
	float fSum = 0.0f;		//���˲�ϵ���ͣ�����Ȩֵ
		
	//��ʱ����
	//��ǰ��������ݣ����ټ�����
	float fSigma_2pow = pow(fSigma, 2.0f);	//Sigmaƽ��
	float f_Sqrt2PiSigma = sqrt(2.0f * 3.1415926f) * fSigma;	//2*Pi*Sigmaƽ��
	float fGaussianCoeff = -0.5f / fSigma_2pow;	//��˹ϵ��
	float fResult = 0.0f;	//��ʱ��Ž��
	IplImage *pTemp_Img = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, pInputImg->nChannels);
	uchar *pDataIn;	//ָ��ǰ���������
	char *pDataOut;	//ָ��ǰ���������

	//�߽���չ
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);
	

	int nIndex;
	nIndex = 0;
	//���ɸ�˹����  
	for (int nDis = -nOffSet; nDis <= nOffSet; nDis++)
	{
		fResult = exp(fGaussianCoeff * nDis * nDis) / f_Sqrt2PiSigma;
		*(pfKernel + nIndex++) = fResult;
		fSum += fResult;	//�����˹ϵ���ͣ����ڼ���Ȩֵ
	}
	//��һ��,����Ȩֵ
	for (nIndex = 0; nIndex < nSize; nIndex++)
	{
		*(pfKernel + nIndex) /= fSum;
	}



	//�ɷ����˹�˲�
	int x, y;	//ģ������ȡ���С���
	//x�����˲�
	fResult = 0.0f;
	pDataIn = (uchar*)pEdgeImage->imageData + pEdgeImage->widthStep * nOffSet;
	pDataOut = pTemp_Img->imageData;
	for (int j = 0; j < nHeight; j++)	//ͼ���ж�λ
	{
		for (int i = 0; i < nWidth; i++)	//ͼ���ж�λ
		{
			for (int n = -nOffSet; n <= nOffSet; n++)	//ģ���ж�λ
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

	
	//y�����˲�
	cvCopyMakeBorder(pTemp_Img, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);
	for (int i = 0; i < nWidth; i++)	//ͼ���ж�λ
	{
		pDataIn = (uchar*)(pEdgeImage->imageData + i + nOffSet);	//�ȶ�λ��ǰ��
		pDataOut = (pOutputImg->imageData + i);
		for (int j = 0; j < nHeight; j++)	//ͼ���ж�λ
		{
			for (int n = -nOffSet; n <= nOffSet; n++)	//ģ���ж�λ
			{
				y = j + nOffSet + n;
				fResult += *(pfKernel + nOffSet + n) * uchar(*(pDataIn + y * pEdgeImage->widthStep));
			}
			*(pDataOut) = char(fResult);
			pDataOut += pOutputImg->widthStep;	//������һ��
			fResult = 0.0f;
		}
	}

	cvReleaseImage(&pTemp_Img);		//�ͷ���ʱͼ��
	cvReleaseImage(&pEdgeImage);	//�ͷ���չ�߽�ͼ��
	delete[] pfKernel;				//ɾ��ģ������
	return bValid;

}


/**
*	function:		Sobel
*	description:	Sobel��Ե���
*	parameter:		pInputImg,pOutputImg:���롢���ͼ��ָ��
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::Sobel(IplImage *pInputImg, IplImage *pOutputImg)
{
	if (NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//��ͨ��ͼ���з��봦��
	if (pInputImg->nChannels != 1)
	{
		bool bValidR, bValidG, bValidB;
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//��ʱ�洢������
		IplImage *pTemp;	//ָ�뽻���м����

		//����ͨ��
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blueͨ������
		bValidB = Sobel(pBlueChannel, pTempImg);			//Ƕ�׵��ú���
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//��������pTempImg,ָ�뽻��
		//Greenͨ������
		bValidG = Sobel(pGreenChannel, pTempImg);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Redͨ������
		bValidR = Sobel(pRedChannel, pTempImg);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//�ϳ�ͨ��
		cvMerge(pBlueChannel, pGreenChannel, pRedChannel, NULL, pOutputImg);
		cvReleaseImage(&pBlueChannel);
		cvReleaseImage(&pGreenChannel);
		cvReleaseImage(&pRedChannel);
		cvReleaseImage(&pTempImg);
		return (bValidR && bValidG && bValidB);
	}

	//��ͨ��ֱ�Ӵ���
	bool bValid = TRUE;
	long nSum_X = 0;	//x���������غ�
	long nSum_Y = 0;	//y���������غ�
	long nSum = 0;		//2�����������غ�
	int nWidth = pInputImg->width;
	int nHeight = pInputImg->height;
	int i, j;
	//ϵ���ʹ��ڽ�С,����ģ�壬�Ż��ٶ�
	//int pSobelX[3][3] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	//int pSobelY[3][3] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

	//�߽���չ
	int nOffSet = 1;
	int n2OffSet = nOffSet + nOffSet;
	int nEdgeWidth = nWidth + n2OffSet;
	int nEdgeHeight = nHeight + n2OffSet;
	IplImage *pEdgeImage = cvCreateImage(cvSize(nEdgeWidth, nEdgeHeight), pInputImg->depth, pInputImg->nChannels);
	cvCopyMakeBorder(pInputImg, pEdgeImage, cvPoint(nOffSet, nOffSet), Edge_Mode);


	uchar *pDataIn = (uchar*)(pEdgeImage->imageData + pEdgeImage->widthStep); // ָ��ǰ���������
	char *pDataOut = pOutputImg->imageData; // ָ��ǰ���������

	int x;
	for (j = 0; j < nHeight; j++)	//ͼ���ж�λ
	{
		//ͼ��X��Y������
		for (i = 0; i < nWidth; i++)	//ͼ���ж�λ,x������Ժ������������һ��
		{
			x = i + nOffSet;
			//X�����󵼣������ֱ��Ե
			nSum_X = (uchar)*(pDataIn + x - pEdgeImage->widthStep + 1)		//1 * ����
				+ (uchar)*(pDataIn + x + pEdgeImage->widthStep + 1)		//1	* ����
				+ (uchar)*(pDataIn + x + 1) * 2							//2 * ����	
				- uchar(*(pDataIn + x - pEdgeImage->widthStep - 1))		//-1 * ����
				- uchar(*(pDataIn + x + pEdgeImage->widthStep - 1))		//-1 * ����
				- uchar(*(pDataIn + x - 1)) * 2;							//-2 * ����

			//Y�����󵼣����ˮƽ��Ե
			nSum_Y = uchar(*(pDataIn + pEdgeImage->widthStep + x - 1))		//1 * ����
				+ uchar(*(pDataIn + pEdgeImage->widthStep + x + 1))		//1 * ����
				+ uchar(*(pDataIn + pEdgeImage->widthStep + x)) * 2		//2 * ����
				- uchar(*(pDataIn - pEdgeImage->widthStep + x - 1))		//-1 * ����
				- uchar(*(pDataIn - pEdgeImage->widthStep + x + 1))		//-1 * ����
				- uchar(*(pDataIn - pEdgeImage->widthStep + x)) * 2;		//-2 * ����

			//�����������,Ҳ�� sqrt(abs(nSum_X) * abs(nSum_X) + abs(nSum_Y) * abs(nSum_Y))
			//��abs(nSum_X) + abs(nSum_Y)���������ֵ���Ҹ���
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

	uchar *pDataSrc = (uchar*)(pImageSrc->imageData + pImageSrc->widthStep * y); // ָ��ǰ���������
	char *pDataDst = pImageDst->imageData + pImageDst->widthStep * Y; // ָ��ǰ���������

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
*	description:	�˶�����
*	parameter:		pInputImg_pre,pInputImg:��һ֡ͼ�񡢵�ǰ֡ͼ��
					pOutputImg�������˶�ʸ��ͨ����һ֡�ó���ǰ֡Ԥ����
					nSchRange:��������Χ
*	return:			boolֵ:�Ƿ���ɹ�
**/
bool CImgProcessing::Motion_Estimation(IplImage *pInputImg_pre, IplImage *pInputImg, IplImage *pOutputImg, int nSchRange)
{
	if (NULL == pInputImg_pre || NULL == pInputImg || NULL == pOutputImg)return FALSE;
	//��ͨ��ͼ���з��봦��
	if (pInputImg->nChannels != 1)
	{
		bool bValidR = false, bValidG = false, bValidB = false;
		IplImage *pBlueChannel_pre = cvCreateImage(cvGetSize(pInputImg_pre), pInputImg->depth, 1);
		IplImage *pGreenChannel_pre = cvCreateImage(cvGetSize(pInputImg_pre), pInputImg->depth, 1);
		IplImage *pRedChannel_pre = cvCreateImage(cvGetSize(pInputImg_pre), pInputImg->depth, 1);
		IplImage *pBlueChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pGreenChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pRedChannel = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);
		IplImage *pTempImg = cvCreateImage(cvGetSize(pInputImg), pInputImg->depth, 1);	//��ʱ�洢������
		IplImage *pTemp;	//ָ�뽻���м����

		//����ͨ��
		cvSplit(pInputImg_pre, pBlueChannel_pre, pGreenChannel_pre, pRedChannel_pre, NULL);
		cvSplit(pInputImg, pBlueChannel, pGreenChannel, pRedChannel, NULL);

		//Blueͨ������
		//memset(pTempImg->imageData, 0, pTempImg->imageSize);
		bValidB = Motion_Estimation(pBlueChannel_pre, pBlueChannel, pTempImg, nSchRange);			//Ƕ�׵��ú���
		pTemp = pBlueChannel; pBlueChannel = pTempImg; pTempImg = pTemp;	//��������pTempImg,ָ�뽻��
		//Greenͨ������
		//memset(pTempImg->imageData, 0, pTempImg->imageSize);
		bValidG = Motion_Estimation(pGreenChannel_pre, pGreenChannel, pTempImg, nSchRange);
		pTemp = pGreenChannel; pGreenChannel = pTempImg; pTempImg = pTemp;
		//Redͨ������
		//memset(pTempImg->imageData, 0, pTempImg->imageSize);
		bValidR = Motion_Estimation(pRedChannel_pre, pRedChannel, pTempImg, nSchRange);
		pTemp = pRedChannel; pRedChannel = pTempImg; pTempImg = pTemp;

		//�ϳ�ͨ��
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

	//��ͨ��ֱ�Ӵ���
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
						//������ǰһ֡ͼ����ӽ��Ŀ�
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
		if (flag)	//flagΪTRUE���Ѿ���˳���źã���������
		{
			break;
		}
	}
}

void CImgProcessing::Selection_Sort(uchar* pDataList, unsigned int nSize)
{
	unsigned int min_index;
	//ѡ����С����������ǰ
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
	uchar nCompare;	//�������������ǰ���������Ƚ�
	unsigned int nData_Index;		//���������������
	unsigned int nInser_Index;		//���������

	for (nData_Index = 1; nData_Index < nSize; nData_Index++)
	{
		nCompare = *(pDataList + nData_Index);
		nInser_Index = nData_Index;

		//��ǰ�����бȴ��������������Ƶ�����
		while (nInser_Index > 0 && *(pDataList + nInser_Index - 1) > nCompare)
		{
			*(pDataList + nInser_Index) = *(pDataList + nInser_Index - 1);
			nInser_Index--;
		}
		//�ҵ�С�ڵ��ڸ�����λ�ã�����
		*(pDataList + nInser_Index) = nCompare;
	}
}

void CImgProcessing::Quick_Sort(uchar* pDataList, unsigned int nSize)
{
	bool cross = FALSE;
	unsigned int end = nSize - 1;	//����ĩ����
	unsigned int left = 1, right = end;
	if (nSize != 1)								//�}��һ�����c�r�������� 
	{
		while (1)
		{
			while (*(pDataList + left) <= *(pDataList))		//���������ь��Ȼ���ֵ��Ĕ� 
			{
				if (left == right)				//�c��ָ�˽��棬ӛ��� 
				{
					cross = TRUE;
					break;
				}
				left++;
			}
			while (*(pDataList + right) > *(pDataList))		//���������ь�С춵�춻���ֵ�Ĕ� 
			{
				if (left == right)
				{
					cross = TRUE;
				}
				right--;
			}
			if (cross)
			{
				break;							//���l���˽��棬�˕r��ָ�˼�����߅��ֵ��С춻���ֵ����߅��춻���ֵ�� 
			}									//�˕r��������Ȧ������ָ��ָ���ֵ�c����ֵ���Q 
			exchangeNum((pDataList + left), (pDataList + right));			//���]���棬��ָ���c��ָ��ָ���ֵ���Q 
		}
		exchangeNum(pDataList, (pDataList + right));
		if (0 != right)						//�˕r��ָ��ָ���ֵ�����_λ�ã���߅��ֵ������С����߅������ 
		{										//�f���ٴΌ����҃�߅�M��quick sort 
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
	//����,ÿ��2�����в��������ٷֳ�ÿ��4��...����
	unsigned int incr = nSize / 2;
	unsigned int i;
	int j;
	while (incr > 0)
	{
		for (i = incr; i < nSize; i++)	//���Ʋ�������ɴӵ�1����ʼ���в���,�����ÿ���1����ʼ
		{
			j = i - incr;	//ѡȡ������һ�����бȽ�
			while (j >= 0)
			{
				if (*(pDataList + j) > *(pDataList + j + incr))
				{
					exchangeNum((pDataList + j), (pDataList + j + incr));
					j -= incr;
				}
				else
				{
					break;		//ÿ��ÿ�ζ�ֻ��һ����
				}
			}
		}		//���������ĸ�����ʵ�λ�ã�ԭ��ͬ��������
		incr /= 2;
	}
}

void CImgProcessing::RadixMSD_Sort(uchar* pDataList, unsigned int nSize)
{
	unsigned int radix = 0;
	unsigned int nData_Index;		//Ѱ�һ�����ͬ�����ݵ�����
	unsigned int nInser_Index = 0;		//���������
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