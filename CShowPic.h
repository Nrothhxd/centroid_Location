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
	void CreatBmpInfo(Mat &img);						///创造bmp信息头等
	void GetVideoInfo();									   ///预处理
	bool openImage();											///打开图片
	Point2d centroid_Extraction1(Mat img);
	Point2d centroid_Extraction2(Mat& img);
	Point2d get_Centroid(Point2d p);                 ///获取质心坐标
	void pos();                  ///划十字
	void chageThresh();///更改阈值
private:
	LPBITMAPINFO		m_pBmpInfo;						///bmp信息头等
	CRect			        	m_rt;									///控件矩形
	int					        m_FrameHeight;					///单帧图像高
	int							m_FrameWidth;					///单帧图像宽
	int							m_RtHeight;							///控件高
	int							m_RtWidth;							///控件宽
	float							m_rate;									///比率
	int							m_InternalH;						///控件内部图像的高
	int							m_InternalW;						///控件内部图像的宽
                      
	double						m_VideoFps;						///Video FPS
	long							m_VideoFrames;					///帧数
	VideoCapture			m_Video;								///视频
	long							m_nowFrame;						///视频当前帧

	Mat							m_Frame;								///单帧矩阵
	Mat							m_FrameGray;						///单帧灰度矩阵
	Mat                         m_threshImg;

public:
	int                            m_thresh=65;                     ///二值化阈值
	Point2d                    m_centroid;                        ///质心坐标


public:
	void	                        playVideo(String videoPathName);  ///打开视频 创建线程
	void			     			 GetVideoData();					///获取视频数据
	void							selectFunctions();				///选择播放或者--暂停--
	Mat							getNowFrame();				///获取当前帧

	void							startThread();					///激活线程
	void							stopThread();					///结束线程初始化

	CEvent						m_Live;							///激活
	CEvent						m_Exit;							///退出

	BOOL						m_Play;							///播放与暂停
	BOOL						m_Stop;						///追踪;

private:
	CWinThread *			m_thread;
};

