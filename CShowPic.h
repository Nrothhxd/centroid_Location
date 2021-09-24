#pragma once
#include <afxwin.h>

class CShowPic :	public CStatic
{
	DECLARE_DYNAMIC(CShowPic)

public:
	CShowPic();
	virtual ~CShowPic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

public:
	void CreatBmpInfo(Mat &img);						///����bmp��Ϣͷ��
	void GetVideoInfo();									   ///Ԥ����
	bool openImage();											///��ͼƬ
	Point2d centroid_Extraction1(Mat img);
	Point2d centroid_Extraction2(Mat& img);
	Point2d get_Centroid(Point2d p);                 ///��ȡ��������
	void pos();                  ///��ʮ��
	void chageThresh();///������ֵ
private:
	LPBITMAPINFO		m_pBmpInfo;						///bmp��Ϣͷ��
	CRect			        	m_rt;									///�ؼ�����
	int					        m_FrameHeight;					///��֡ͼ���
	int							m_FrameWidth;					///��֡ͼ���
	int							m_RtHeight;							///�ؼ���
	int							m_RtWidth;							///�ؼ���
	float							m_rate;									///����
	int							m_InternalH;						///�ؼ��ڲ�ͼ��ĸ�
	int							m_InternalW;						///�ؼ��ڲ�ͼ��Ŀ�
                      
	double						m_VideoFps;						///Video FPS
	long							m_VideoFrames;					///֡��
	VideoCapture			m_Video;								///��Ƶ
	long							m_nowFrame;						///��Ƶ��ǰ֡

	Mat							m_Frame;								///��֡����
	Mat							m_FrameGray;						///��֡�ҶȾ���
	Mat                         m_threshImg;

public:
	int                            m_thresh=65;                     ///��ֵ����ֵ
	Point2d                    m_centroid;                        ///��������


public:
	void	                        playVideo(String videoPathName);  ///����Ƶ �����߳�
	void			     			 GetVideoData();					///��ȡ��Ƶ����
	void							selectFunctions();				///ѡ�񲥷Ż���--��ͣ--
	Mat							getNowFrame();				///��ȡ��ǰ֡

	void							startThread();					///�����߳�
	void							stopThread();					///�����̳߳�ʼ��

	CEvent						m_Live;							///����
	CEvent						m_Exit;							///�˳�

	BOOL						m_Play;							///��������ͣ
	BOOL						m_Stop;						///׷��;

private:
	CWinThread *			m_thread;
};

