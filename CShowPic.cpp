#include "pch.h"
#include "CShowPic.h"
#include "trackingDlg.h"
#include "resource.h"
IMPLEMENT_DYNAMIC(CShowPic,CStatic)

CShowPic::CShowPic():m_Live(FALSE, TRUE, NULL, NULL)
, m_Exit(FALSE, TRUE, NULL, NULL)
{
	m_VideoFrames = 0;
	m_nowFrame = 0;
	m_FrameHeight = 0;
	m_FrameWidth = 0;
	m_RtHeight = 0;
	m_RtWidth = 0;
	m_InternalH = 0;
	m_InternalW = 0;
	m_Stop = true;
	m_Play = false;
}

CShowPic::~CShowPic()
{
}

BEGIN_MESSAGE_MAP(CShowPic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//˫������ʾ
void CShowPic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
	///CPen pen(PS_SOLID��10, RGB(0, 0, 0));
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap memBmp, *pOldBmp;
	memBmp.CreateCompatibleBitmap(&dc, m_RtWidth, m_RtHeight);
	pOldBmp = memDC.SelectObject(&memBmp);
	
	
	dc.MoveTo(100, 100);
	dc.LineTo(200, 200);

	// ͼƬ�ȱ�����ʾ
	SetStretchBltMode(memDC.GetSafeHdc(), COLORONCOLOR);
	if (m_FrameWidth >= m_FrameHeight)
	{
		m_rate = m_RtWidth * 1.0f / m_FrameWidth;
		m_InternalH = int(m_FrameHeight*m_rate);
		StretchDIBits(memDC.GetSafeHdc(), 0, (m_RtHeight - m_InternalH) / 2, m_RtWidth, m_InternalH
			, 0, 0, m_FrameWidth, m_FrameHeight, m_threshImg.data, m_pBmpInfo
			, DIB_RGB_COLORS, SRCCOPY);
	}
	else if(m_FrameWidth < m_FrameHeight)
	{
		m_rate = m_RtHeight * 1.0f / m_FrameHeight;
		m_InternalW = int(m_FrameWidth*m_rate);
		StretchDIBits(memDC.GetSafeHdc(), (m_RtWidth - m_InternalW) / 2, 0, m_InternalW, m_RtHeight
			, 0, 0, m_FrameWidth, m_FrameHeight, m_threshImg.data, m_pBmpInfo
			, DIB_RGB_COLORS, SRCCOPY);
	}
	dc.BitBlt(m_rt.left, m_rt.top, m_RtWidth, m_RtHeight, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBmp.DeleteObject();

	///��ʾ��������ֵ
	CtrackingDlg* pMDlg = (CtrackingDlg*)(AfxGetApp()->GetMainWnd());
	pMDlg->updatePointInfo1();
}

///��ͼƬ
bool CShowPic::openImage()
{
	LPCTSTR lpszFilter = L"(JPG)|*.jpg|(BMP)|*.bmp|(PNG)|*.png||";
	CFileDialog fileDlg(TRUE,											// TRUE for FileOpen, FALSE for FileSaveAs
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		lpszFilter);
	INT_PTR  result = fileDlg.DoModal();
	CString filePathName = L"Nothing to be selected!";

	if (result == IDOK)
	{
		filePathName = L"";
		filePathName = fileDlg.GetPathName();

		USES_CONVERSION;
		cv::String cvImgName(W2A(filePathName));
		Mat img;
		img = imread(cvImgName, IMREAD_UNCHANGED);
		///����
		GetClientRect(&m_rt);
		m_RtHeight = m_rt.Height();
		m_RtWidth = m_rt.Width();

		if (img.data == NULL)
		{
			AfxMessageBox(_T("ͼƬ��"));
			return false;
		}
		/// �ֽڶ�������
		if (img.cols % 4 != 0)
		{
			m_Frame = img(Rect(0, 0, (img.cols - 3) / 4 * 4, img.rows)).clone();
			m_FrameHeight = m_Frame.rows;
			m_FrameWidth = m_Frame.cols;
			///CreatBmpInfo(m_Frame);
		}
		else
		{
			m_Frame = img.clone();
			m_FrameHeight = m_Frame.rows;
			m_FrameWidth = m_Frame.cols;
			///CreatBmpInfo(m_Frame);
		}
		m_threshImg = m_Frame.clone();
		m_centroid = centroid_Extraction2(m_threshImg);///��������
		pos();
		CreatBmpInfo(m_threshImg);
		RedrawWindow();
	}
	return true;
}

void CShowPic::chageThresh()
{
	m_threshImg = m_Frame.clone();
	m_centroid = centroid_Extraction2(m_threshImg);///��������
	pos();
	CreatBmpInfo(m_threshImg);
	RedrawWindow();
}

///���ļ���
Point2d CShowPic::centroid_Extraction1(Mat img)
{
	if (img.channels() != 1)
	{
		cvtColor(img, img, COLOR_BGR2GRAY);
	}
	CtrackingDlg* pMDlg = (CtrackingDlg*)(AfxGetApp()->GetMainWnd());	
	m_thresh = pMDlg->m_Thresh;
	threshold(img, img, m_thresh, 255, THRESH_TOZERO);

	int row = img.rows;
	int col = img.cols;
	double sum_Value = 0.0;
	double x_Value = 0.0;
	double y_Value = 0.0;

	for (int i = 0; i < row; i++)
	{
		uchar *p = img.ptr<uchar>(i);
		for (int j = 0; j < col; j++)
		{
			int value = p[j];
			sum_Value += pow(value, 2.0);
			x_Value += pow(value, 2.0)*j;
			y_Value += pow(value, 2.0)*i;
		}
	}
	return Point2d(x_Value / sum_Value, y_Value / sum_Value);
}

Point2d CShowPic::centroid_Extraction2(Mat& img)
{
	if (img.channels() != 1)
	{
		cvtColor(img, img, COLOR_BGR2GRAY);
	}

	CtrackingDlg* pMDlg = (CtrackingDlg*)(AfxGetApp()->GetMainWnd());
	m_thresh = pMDlg->m_Thresh;

	if (m_thresh < 0 || m_thresh>255)
		return m_centroid;
	threshold(img, img, m_thresh, 255, THRESH_TOZERO);

	int row = img.rows;
	int col = img.cols;
	double sum_Value = 0.0;
	double x_Value = 0.0;
	double y_Value = 0.0;

	for (int i = 0; i < row; i++)
	{
		uchar *p = img.ptr<uchar>(i);
		for (int j = 0; j < col; j++)
		{
			int value = p[j];
			sum_Value += pow(value, 2.0);
			x_Value += pow(value, 2.0)*j;
			y_Value += pow(value, 2.0)*i;
		}
	}
	return Point2d(x_Value / sum_Value, y_Value / sum_Value);
}

Point2d CShowPic::get_Centroid(Point2d p)
{
	return p;
}

void CShowPic::pos()
{
	line(m_threshImg, Point(m_centroid.x - 20, m_centroid.y), Point(m_centroid.x + 20, m_centroid.y), Scalar(0, 255, 0),1);
	line(m_threshImg, Point(m_centroid.x, m_centroid.y - 20), Point(m_centroid.x, m_centroid.y + 20), Scalar(0, 255, 0),1);
}


///Ԥ����
void CShowPic::GetVideoInfo()
{
	//��ȡ��Ƶ��֡��������
	m_VideoFrames = m_Video.get(CAP_PROP_FRAME_COUNT);
	m_VideoFps = m_Video.get(CAP_PROP_FPS);

	m_Video.set(CAP_PROP_POS_FRAMES, 1);
	m_Video >> m_Frame;
	cvtColor(m_Frame, m_FrameGray, COLOR_BGR2GRAY);

	//����
	GetClientRect(&m_rt);
	m_FrameHeight = m_Frame.rows;
	m_FrameWidth = m_Frame.cols;
	m_RtHeight = m_rt.Height();
	m_RtWidth = m_rt.Width();
	///�����һ֡����
	m_threshImg = m_Frame.clone();
	m_centroid = centroid_Extraction2(m_threshImg);
	pos();

	CreatBmpInfo(m_threshImg);  ///��ʾ��ͼ��֡
	RedrawWindow();
}

///����bmp��Ϣͷ��
void CShowPic::CreatBmpInfo(Mat &img)
{
	Mat src_play = img.clone();
	img.copyTo(src_play);

	if (m_pBmpInfo != NULL)
	{
		delete[] m_pBmpInfo;
		m_pBmpInfo = NULL;
	}

	if (img.channels() == 3)
	{
		m_pBmpInfo = (LPBITMAPINFO) new char[sizeof(BITMAPINFOHEADER)];

	}
	else if (img.channels() == 1)
	{
		m_pBmpInfo = (LPBITMAPINFO) new char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
		for (int i = 0; i < 256; i++)
		{
			m_pBmpInfo->bmiColors[i].rgbRed = i;
			m_pBmpInfo->bmiColors[i].rgbGreen = i;
			m_pBmpInfo->bmiColors[i].rgbBlue = i;
			m_pBmpInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	m_pBmpInfo->bmiHeader.biSizeImage = img.cols*img.rows* img.channels();
	m_pBmpInfo->bmiHeader.biBitCount = 8 * img.channels();
	m_pBmpInfo->bmiHeader.biSize = 40;
	m_pBmpInfo->bmiHeader.biWidth = img.cols;
	m_pBmpInfo->bmiHeader.biHeight = -img.rows;
	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
}

///�߳�
UINT  WriteThread(LPVOID lparam)
{
	CShowPic *  pOperation = (CShowPic *)lparam;
	pOperation->GetVideoData();

	return 0;
}

///����Ƶ�������߳�
void CShowPic::playVideo(String videoPathName)
{
	///����Ƶ�ļ�
	m_Video.open(videoPathName);

	//�ж���Ƶ�Ƿ���Դ�
	if (!m_Video.isOpened())
	{
		AfxMessageBox(_T("��ȡ��Ƶ����"));
		return;
	}
	///������Ƶ�����߳� �����߳�
	m_thread = AfxBeginThread(WriteThread, this);
}

/// ��ȡ��Ƶ����--���ú���
void CShowPic::GetVideoData()
{
	///�������ͣ
	HANDLE controlhanle[2] = { m_Live.m_hObject , m_Exit.m_hObject };

	///������Ƶ��һ֡,���Ԥ����
	GetVideoInfo();

	while (m_VideoFrames - m_nowFrame - 1)
	{
		//ѡ��handle
		DWORD  controlresulut = WaitForMultipleObjects(2, controlhanle, FALSE, INFINITE);
		//ѡ�񼤻����˳�
		switch (controlresulut)
		{
		case WAIT_OBJECT_0:	///�����Բ���ʱ
			///����ѡ��
			selectFunctions(); /// ������
			break;
		case WAIT_OBJECT_0 + 1:				///�˳�
			m_threshImg.release();
			this->RedrawWindow();
			return;
		}
		///��ȡ��ǰ֡
		m_nowFrame = m_Video.get(CAP_PROP_POS_FRAMES);
	}
	//��Ƶ���ź󷵻�
	m_nowFrame = 0;
	this->RedrawWindow();
}

/// ����ѡ����
void CShowPic::selectFunctions()
{
	///STOP
	if (m_Stop&&!m_Play)
	{
		//m_Video >> m_Frame;
		//this->RedrawWindow();
	}

	///PLAY
	else if (m_Play&&!m_Stop)
	{
		m_Video >> m_Frame;

		///��������
		m_threshImg = m_Frame.clone();
	    m_centroid=centroid_Extraction2(m_threshImg);

		cvtColor(m_Frame, m_FrameGray, COLOR_RGB2GRAY);
		Sleep(1000 / m_VideoFps);
		///��ʾͼ��
		pos();
		CreatBmpInfo(m_threshImg);
		this->RedrawWindow();
	}
}
///��ȡ��ǰ��Ƶ֡����
Mat CShowPic::getNowFrame()
{
	Mat frame;
	frame = m_Frame.clone();
	return frame;
}

void CShowPic::startThread()
{
	///������
	m_Live.SetEvent();  ///���¼�����Ϊ֪ͨ״̬
	m_Exit.ResetEvent();///���¼�����Ϊδ֪ͨ״̬
}

void CShowPic::stopThread()
{
	///��ȡ���Ի���ָ��
	CtrackingDlg* pMDlg = (CtrackingDlg*)(AfxGetApp()->GetMainWnd());

	m_Live.ResetEvent();
	m_Exit.SetEvent();
	m_Stop = FALSE;
	m_Play = FALSE;
	Sleep(50);
}