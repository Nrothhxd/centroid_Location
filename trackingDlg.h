
// trackingDlg.h: 头文件
//

#pragma once
#include "CShowPic.h"

// CtrackingDlg 对话框
class CtrackingDlg : public CDialogEx
{
// 构造
public:
	CtrackingDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRACKING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	BOOL PreTranslateMessage(MSG * pMsg);  ///截断回车和ESC消息
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpenimg();
	String VideoPathName;
	afx_msg void OnBnClickedButtonOpenvideo();
	CShowPic m_Img;
	afx_msg void OnBnClickedButtonPlayvideo();
	void updatePointInfo1();
	void updatePointInfo2();
	CString m_btn_tracking_select;
	afx_msg void OnEnChangeEdit1();
	int m_Thresh;
};
