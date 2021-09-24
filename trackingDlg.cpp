
// trackingDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "tracking.h"
#include "trackingDlg.h"
#include "afxdialogex.h"
#include "CShowPic.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtrackingDlg 对话框



CtrackingDlg::CtrackingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRACKING_DIALOG, pParent)
	, m_Thresh(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtrackingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOWPIC, m_Img);
	DDX_Text(pDX, IDC_THRESH_EDIT, m_Thresh);
	DDV_MinMaxInt(pDX, m_Thresh, 0, 255);
}

BEGIN_MESSAGE_MAP(CtrackingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPENIMG, &CtrackingDlg::OnBnClickedButtonOpenimg)
	ON_BN_CLICKED(IDC_BUTTON_OPENVIDEO, &CtrackingDlg::OnBnClickedButtonOpenvideo)
	ON_BN_CLICKED(IDC_BUTTON_PLAYVIDEO, &CtrackingDlg::OnBnClickedButtonPlayvideo)
	ON_EN_CHANGE(IDC_THRESH_EDIT, &CtrackingDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CtrackingDlg 消息处理程序

BOOL CtrackingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	/// TODO: 在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtrackingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtrackingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CString str = L"65";
		SetDlgItemText(IDC_THRESH_EDIT, str);///阈值初始化
		m_Thresh = 65;
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtrackingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
BOOL CtrackingDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		UpdateData(TRUE);	
		m_Img.chageThresh();
		UpdateData(FALSE);
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

void CtrackingDlg::OnBnClickedButtonOpenimg()
{
	GetDlgItem(IDC_BUTTON_PLAYVIDEO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAYVIDEO)->SetWindowText(_T("播放视频"));
	///停止视频播放线程
	m_Img.stopThread();
	/// TODO: 在此添加控件通知处理程序代码
	bool a = m_Img.openImage();
}


void CtrackingDlg::OnBnClickedButtonOpenvideo()
{
	/// TODO: 在此添加控件通知处理程序代码
	///结束播放与追踪
	m_Img.stopThread();
	GetDlgItem(IDC_BUTTON_PLAYVIDEO)->SetWindowText(_T("播放视频"));
	/// 设置过滤器   
	TCHAR szFilter[] = _T("文本文件(*.mp4)|*.mp4|所有文件(*.*)|*.*||");

	// /构造打开文件对话框   
	CFileDialog fileDlg(TRUE, NULL, NULL, 0, szFilter, this);

	/// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		///视频文件路径
		CString strFilePath = fileDlg.GetPathName();
		VideoPathName = (LPCSTR)CStringA(strFilePath);

		///允许播放激活 
		m_Img.startThread();

		///开始读取视频
		m_Img.playVideo(VideoPathName);
	}
	else
	{
		MessageBox(_T("No File"));		
	}
	m_Img.m_Play = false;
	m_Img.m_Stop = true;

	GetDlgItem(IDC_BUTTON_PLAYVIDEO)->EnableWindow(TRUE);
}


void CtrackingDlg::OnBnClickedButtonPlayvideo()
{
	/// TODO: 在此添加控件通知处理程序代码
	Mat nowframemat = m_Img.getNowFrame();
	GetDlgItemTextW(IDC_BUTTON_PLAYVIDEO, m_btn_tracking_select);
	if (m_btn_tracking_select == _T("播放视频") && !nowframemat.empty())
	{
		GetDlgItem(IDC_BUTTON_PLAYVIDEO)->SetWindowText(_T("暂停视频"));
		m_Img.m_Play = true;
		m_Img.m_Stop = false;
	}
	else if (m_btn_tracking_select == _T("暂停视频") && !nowframemat.empty())
	{
		GetDlgItem(IDC_BUTTON_PLAYVIDEO)->SetWindowText(_T("播放视频"));
		m_Img.m_Play = false;
		m_Img.m_Stop = true;
	}
}

void CtrackingDlg::updatePointInfo1()
{
	CString str;
	str.Format(L"质心坐标:(%.3f,%.3f)", m_Img.m_centroid.x, m_Img.m_centroid.y);
	SetDlgItemText(IDC_TEXT_POS, str);
	/*if (m_Img.m_centroid.x != 0 && m_Img.m_centroid.y != 0)
	{
		Mat nowframemat = m_Img.getNowFrame();
		line(nowframemat, Point(m_Img.m_centroid.x - 5, m_Img.m_centroid.y), Point(m_Img.m_centroid.x + 10, m_Img.m_centroid.y), Scalar(0, 255, 0), 1, 8, 0);
	}*/
}


void CtrackingDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	// TODO:  在此添加控件通知处理程序代码
}
